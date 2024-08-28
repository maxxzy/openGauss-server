/* -------------------------------------------------------------------------
 *
 * freelist.cpp
 *	  routines for managing the buffer pool's replacement strategy.
 *
 *
 * Portions Copyright (c) 2020 Huawei Technologies Co.,Ltd.
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/gausskernel/storage/buffer/freelist.cpp
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"
#include "utils/atomic.h"
#include "access/xlog.h"
#include "storage/buf/buf_internals.h"
#include "storage/buf/bufmgr.h"
#include "storage/proc.h"
#include "postmaster/aiocompleter.h" /* this is for the function AioCompltrIsReady() */
#include "postmaster/bgwriter.h"
#include "postmaster/pagewriter.h"
#include "postmaster/postmaster.h"
#include "access/double_write.h"
#include "gstrace/gstrace_infra.h"
#include "gstrace/storage_gstrace.h"
#include "pgstat.h"
#include <unordered_map>
#include <list>

#define INT_ACCESS_ONCE(var) ((int)(*((volatile int *)&(var))))

/*
 * The shared freelist control information.
 */
typedef struct BufferStrategyControl {
    /* Spinlock: protects the values below */
    slock_t buffer_strategy_lock;

    /*
     * Clock sweep hand: index of next buffer to consider grabbing. Note that
     * this isn't a concrete buffer - we only ever increase the value. So, to
     * get an actual buffer, it needs to be used modulo NBuffers.
     */
    pg_atomic_uint32 nextVictimBuffer;

    /*
     * Statistics.	These counters should be wide enough that they can't
     * overflow during a single bgwriter cycle.
     */
    uint32 completePasses;            /* Complete cycles of the clock sweep */
    pg_atomic_uint32 numBufferAllocs; /* Buffers allocated since last reset */

    /*
     * Bgworker process to be notified upon activity or -1 if none. See
     * StrategyNotifyBgWriter.
     */
    int bgwprocno;

    std::list<int> cold_list;
    int cold_size;
    //std::unordered_map<int, std::list<int>::iterator> cold_map;
    slock_t cold_list_lock;

    std::list<BufferTag> history_list;
    int history_size;
    std::unordered_map<BufferTag, std::list<BufferTag>::iterator> history_map;
    std::unordered_map<BufferTag, int> history_hitcount_map;//buf被淘汰后会用于新buf,buf_tag对应的hitcount需要在history_list中维护
    slock_t history_list_lock;

    std::list<int> hot_list;
    int hot_size;
    //std::unordered_map<int, std::list<int>::iterator> hot_map;
    slock_t hot_list_lock;

    uint32 bottom;
    uint32 level_num;
    uint32 history_capacity;
    uint32 capacity;

    bool in_compaction;
    slock_t compaction_lock;
} BufferStrategyControl;

typedef struct {
    int64 retry_times;
    int cur_delay_time;
} StrategyDelayStatus;

const int MIN_DELAY_RETRY = 100;
const int MAX_DELAY_RETRY = 1000;
const int MAX_RETRY_TIMES = 1000;
const float NEED_DELAY_RETRY_GET_BUF = 0.8;

enum NodeType : uint8_t {
    NONE = 1,
    A1_IN_TYPE = 2,
    A1_OUT_TYPE = 3,
    AM_TYPE = 4
};

/* Prototypes for internal functions */
static BufferDesc* GetBufferFromRing(BufferAccessStrategy strategy, uint32* buf_state);
static void AddBufferToRing(BufferAccessStrategy strategy, volatile BufferDesc* buf);
void PageListBackWrite(uint32* bufList, int32 n,
    /* buffer list, bufs to scan, */
    uint32 flags = 0,                 /* opt flags */
    SMgrRelation use_smgrReln = NULL, /* opt relation */
    int32* bufs_written = NULL,       /* opt written count returned */
    int32* bufs_reusable = NULL);     /* opt reusable count returned */
static BufferDesc* get_buf_from_candidate_list(BufferAccessStrategy strategy, uint32* buf_state);

static void perform_delay(StrategyDelayStatus *status)
{
    if (++(status->retry_times) > MAX_RETRY_TIMES &&
        get_dirty_page_num() > g_instance.attr.attr_storage.NBuffers * NEED_DELAY_RETRY_GET_BUF) {
        if (status->cur_delay_time == 0) {
            status->cur_delay_time = MIN_DELAY_RETRY;
        }
        pg_usleep(status->cur_delay_time);

        /* increase delay by a random fraction between 1X and 2X */
        status->cur_delay_time += (int)(status->cur_delay_time * ((double)random() / (double)MAX_RANDOM_VALUE) + 0.5);
        if (status->cur_delay_time > MAX_DELAY_RETRY) {
            status->cur_delay_time = MIN_DELAY_RETRY;
        }
    }
    return;
}


/*
 * ClockSweepTick - Helper routine for StrategyGetBuffer()
 *
 * Move the clock hand one buffer ahead of its current position and return the
 * id of the buffer now under the hand.
 */
static inline uint32 ClockSweepTick(int max_nbuffer_can_use)
{
    uint32 victim;

    /*
     * Atomically move hand ahead one buffer - if there's several processes
     * doing this, this can lead to buffers being returned slightly out of
     * apparent order.
     */
    victim = pg_atomic_fetch_add_u32(&t_thrd.storage_cxt.StrategyControl->nextVictimBuffer, 1);
    if (victim >= (uint32)max_nbuffer_can_use) {
        uint32 original_victim = victim;

        /* always wrap what we look up in BufferDescriptors */
        victim = victim % max_nbuffer_can_use;

        /*
         * If we're the one that just caused a wraparound, force
         * completePasses to be incremented while holding the spinlock. We
         * need the spinlock so StrategySyncStart() can return a consistent
         * value consisting of nextVictimBuffer and completePasses.
         */
        if (victim == 0) {
            uint32 expected;
            uint32 wrapped;
            bool success = false;

            expected = original_victim + 1;

            while (!success) {
                /*
                 * Acquire the spinlock while increasing completePasses. That
                 * allows other readers to read nextVictimBuffer and
                 * completePasses in a consistent manner which is required for
                 * StrategySyncStart().  In theory delaying the increment
                 * could lead to a overflow of nextVictimBuffers, but that's
                 * highly unlikely and wouldn't be particularly harmful.
                 */
                SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->buffer_strategy_lock);

                wrapped = expected % max_nbuffer_can_use;

                success = pg_atomic_compare_exchange_u32(&t_thrd.storage_cxt.StrategyControl->nextVictimBuffer,
                                                         &expected, wrapped);
                if (success)
                    t_thrd.storage_cxt.StrategyControl->completePasses++;
                SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->buffer_strategy_lock);
            }
        }
    }
    return victim;
}

/**
 * @description:hotlist中buf小于bottom降级至coldlist,未出现降级则提高bottom重试
 * @return {*}
 * @use:
 * @author: xzy
 */
void compaction() {
    BufferDesc *buf = NULL;
    uint32 local_buf_state = 0;
    bool if_demote = false;
    auto Controller = t_thrd.storage_cxt.StrategyControl;
    auto bottom = Controller->bottom;
retry:
    auto iter = Controller->hot_list.begin();
    while(++iter != Controller->hot_list.end()) {
        int buf_id = *iter;
        buf = GetBufferDescriptor(buf_id);
        local_buf_state = LockBufHdr(buf);
        if (buf->hitcount == bottom) {
            SpinLockAcquire(&Controller->hot_list_lock);
            Controller->hot_size--;
            Controller->hot_list.erase(iter);
            SpinLockRelease(&Controller->hot_list_lock);

            SpinLockAcquire(&Controller->cold_list_lock);
            Controller->cold_size++;
            Controller->cold_list.emplace_back(buf_id);
            buf->iter = Controller->cold_list.end() - 1;
            SpinLockRelease(Controller->cold_list_lock);

            buf->buftype = NodeType::A1_IN_TYPE;
            buf->hitcount = 1;
            if_demote = true;
        }
        UnlockBufHdr(buf, local_buf_state);
    }
    if (!if_demote) {
        bottom = (bottom + 1) % Controller->level_num;
        goto retry;
    }
    Controller->in_compaction = false;
}

bool check_compaction() {
    auto Controller = t_thrd.storage_cxt.StrategyControl;
    if (Controller->in_compaction) {
        return true;
    }
    if (Controller->cold_size + Controller->hot_size >= Controller->capacity) {
        Controller->in_compaction = true;
        compaction();
        return false;
    }
}

/**
 * @description: 检查historylist长度,超过上限则删除记录(fifo)
 * @return {*}
 * @use:
 * @author: xzy
 */
void CheckHistoryListSize() {
    auto Controller = t_thrd.storage_cxt.StrategyControl;
    while (Controller->history_size > Controller->history_capacity){
        Controller->history_map.erase(Controller->history_list.front());
        Controller->history_hitcount_map.erase(Controller->history_list.front());
        Controller->history_list.pop_front();
        Controller->history_size--;
    }
}

/**
 * @description:当buf在缓冲区中(在coldlist或hotlist中)执行该函数
 * 1.buf位于coldlist,若需要或正在compaction则只hitcount++,否则从coldlist中移入hotlist
 * 2.buf位于hotlist修改hitcount
 * @param {int} buf_id
 * @return {*}
 * @use:bufmgr.cpp:2712
 * @author: xzy
 */
void HitBuffer(int buf_id){
    BufferDesc *buf = NULL;
    BufferStrategyControl* Controller = t_thrd.storage_cxt.StrategyControl;

    uint32 local_buf_state = 0;
    buf = GetBufferDescriptor(buf_id);

    local_buf_state = LockBufHdr(buf);

    if (buf->buftype == NodeType::A1_IN_TYPE) {
        if (check_compaction()) {
            buf->hitcount++;
        } else {
            SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->cold_list_lock);
            Controller->cold_size--;
            Controller->cold_list.erase(buf->iter);
            SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->cold_list_lock);

            SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->hot_list_lock);
            Controller->hot_size++;
            Controller->hot_list.emplace_back(buf_id);
            buf->iter = Controller->hot_list.end() - 1;
            SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->hot_list_lock);

            buf->buftype = NodeType::AM_TYPE;
            buf->hitcount = (Controller->bottom + buf->hitcount - 1) % Controller->level_num;
        }
        UnlockBufHdr(buf, local_buf_state);
        return;
    } else if (buf->buftype == NodeType::AM_TYPE) {
        auto top = (Controller->bottom - 1) % Controller->level_num;
        if (buf->hitcount != top) {
            buf->hitcount++;
            if (buf->hitcount >= Controller->level_num) {
                buf->hitcount -= Controller->level_num;
            }
        }
        UnlockBufHdr(buf, local_buf_state);
        return;
    } else {
        UnlockBufHdr(buf, local_buf_state);
        ereport(ERROR, (errcode(ERRCODE_INVALID_BUFFER), (errmsg("buffertype is wrong"))));
    }

    UnlockBufHdr(buf, local_buf_state);
}

/**
 * @description:新的buf加入缓冲区(coldlist和hotlist)
 * 先判断是否在hitorylist中,若不在则加入coldlist
 * 若buf_tag在historylist中,则加入hotlist,将historylist中维护的hitcount+1并交给当前buf,在historylist中移除buf_tag
 * 执行BufferAdmit时buf应该已经是锁住的状态了
 * @param {BufferDesc} *buf
 * @return {*}
 * @use: bufmgr.cpp:3065
 * @author: xzy
 */
void BufferAdmit(BufferDesc *buf) {
    auto Controller = t_thrd.storage_cxt.StrategyControl;
    if (!Controller->history_map.find(buf->tag)) {
        SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->cold_list_lock);
        Controller->cold_size++;
        Controller->cold_list.emplace_back(buf->buf_id);
        buf->buftype = NodeType::A1_IN_TYPE;
        buf->iter = Controller->cold_list.end() - 1;
        SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->cold_list_lock);
        return;
    }

    SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->history_list);
    auto list_iter = Controller->history_map.at(buf->tag);
    Controller->history_size--;
    Controller->history_list.erase(list_iter);
    Controller->history_map.erase(buf->buf_id);
    int history_hitcount = Controller->history_hitcount_map.at(buf->tag) + 1;
    SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->history_list_lock);

    if (check_compation()) {
        SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->cold_list);
        Controller->cold_size++;
        Controller->cold_list.emplace_back(buf->buf_id);
        buf->iter = Controller->cold_list.end() - 1;
        SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->cold_list_lock);
        buf->buftype = NodeType::A1_IN_TYPE;
        buf->hitcount = history_hitcount;
        return;
    }

    SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->hot_list_lock) ;
    Controller->hot_size++;
    Controller->hot_list.emplace_back(buf->buf_id);
    buf->iter = Controller->hot_list.end() - 1;
    SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->hot_list_lock);
    buf->buftype = NodeType::AM_TYPE;
    buf->hitcount = (Controller->bottom + history_hitcount - 1) % Controller->level_num;
}

/**
 * @description:从coldlist中淘汰(fifo),如果当前buf已被pin住或不合适会考虑coldlist中其他buf
 * 找到合适的buf后从coldlist中移入historylist,historylist维护buftag和hitcount
 * buf的hitcount置0,当前置换出去的buf会被新的覆盖
 * @param {BufferAccessStrategy} strategy
 * @param {uint32*} buf_state
 * @return {BufferDesc*}
 * @author: xzy
 */
BufferDesc* StrategyGetBuffer_new(BufferAccessStrategy strategy, uint32* buf_state)
{
    BufferDesc *buf = NULL;
    int bgwproc_no;
    int try_counter;
    uint32 local_buf_state = 0; /* to avoid repeated (de-)referencing */
    int max_buffer_can_use;
    bool am_standby = RecoveryInProgress();
    StrategyDelayStatus retry_lock_status = { 0, 0 };
    StrategyDelayStatus retry_buf_status = { 0, 0 };

    /*
     * If given a strategy object, see whether it can select a buffer. We
     * assume strategy objects don't need buffer_strategy_lock.
     */
    if (strategy != NULL) {
        buf = GetBufferFromRing(strategy, buf_state);
        if (buf != NULL) {
            return buf;
        }
    }

    /*
     * If asked, we need to waken the bgwriter. Since we don't want to rely on
     * a spinlock for this we force a read from shared memory once, and then
     * set the latch based on that value. We need to go through that length
     * because otherwise bgprocno might be reset while/after we check because
     * the compiler might just reread from memory.
     *
     * This can possibly set the latch of the wrong process if the bgwriter
     * dies in the wrong moment. But since PGPROC->procLatch is never
     * deallocated the worst consequence of that is that we set the latch of
     * some arbitrary process.
     */
    bgwproc_no = INT_ACCESS_ONCE(t_thrd.storage_cxt.StrategyControl->bgwprocno);
    if (bgwproc_no != -1) {
        /* reset bgwprocno first, before setting the latch */
        t_thrd.storage_cxt.StrategyControl->bgwprocno = -1;

        /*
         * Not acquiring ProcArrayLock here which is slightly icky. It's
         * actually fine because procLatch isn't ever freed, so we just can
         * potentially set the wrong process' (or no process') latch.
         */
        SetLatch(&g_instance.proc_base_all_procs[bgwproc_no]->procLatch);
    }

    /*
     * We count buffer allocation requests so that the bgwriter can estimate
     * the rate of buffer consumption.	Note that buffers recycled by a
     * strategy object are intentionally not counted here.
     */
    (void)pg_atomic_fetch_add_u32(&t_thrd.storage_cxt.StrategyControl->numBufferAllocs, 1);

    /* Check the Candidate list */
    if (ENABLE_INCRE_CKPT && pg_atomic_read_u32(&g_instance.ckpt_cxt_ctl->current_page_writer_count) > 1) {
        if (NEED_CONSIDER_USECOUNT) {
            const uint32 MAX_RETRY_SCAN_CANDIDATE_LISTS = 5;
            const int MILLISECOND_TO_MICROSECOND = 1000;
            uint64 maxSleep = u_sess->attr.attr_storage.BgWriterDelay * MILLISECOND_TO_MICROSECOND;
            uint64 sleepTime = 1000L;
            uint32 retry_times = 0;
            while (retry_times < MAX_RETRY_SCAN_CANDIDATE_LISTS) {
                buf = get_buf_from_candidate_list(strategy, buf_state);
                if (buf != NULL) {
                    (void)pg_atomic_fetch_add_u64(&g_instance.ckpt_cxt_ctl->get_buf_num_candidate_list, 1);
                    return buf;
                }
                pg_usleep(sleepTime);
                sleepTime = Min(sleepTime * 2, maxSleep);
                retry_times++;
            }
        } else {
            buf = get_buf_from_candidate_list(strategy, buf_state);
            if (buf != NULL) {
                (void)pg_atomic_fetch_add_u64(&g_instance.ckpt_cxt_ctl->get_buf_num_candidate_list, 1);
                return buf;
            }
        }
    }

    auto Controller = t_thrd.storage_cxt.StrategyControl;
    auto iter = Controller->cold_list.begin() + 1;
    try_counter = Controller->cold_size;
retry:
    int buf_id = *iter;
    buf = GetBufferDescriptor(buf_id);

    if (!retryLockBufHdr(buf, &local_buf_state)) {
            if (++iter == Controller->cold_list.end()) {
                ereport(WARNING,
                        (errmsg("try get buf headr lock times equal to cold_size when StrategyGetBuffer")));
                iter = Controller->cold_list.begin() + 1;
            }
            perform_delay(&retry_lock_status);
            goto retry;
    }

    if (BUF_STATE_GET_REFCOUNT(local_buf_state) == 0 && !(local_buf_state & BM_IS_META) &&
        (backend_can_flush_dirty_page() || !(local_buf_state & BM_DIRTY))) {

        if (strategy != NULL) {
            AddBufferToRing(strategy, buf);
        }

        SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->cold_list_lock);
        Controller->cold_list.erase(iter);
        Controller->cold_size--;
        SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->cold_list_lock);

        SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->history_list_lock);
        Controller->history_list.emplace_back(buf->tag);
        Controller->history_size++;
        Controller->history_map.emplace(buf->tag, Controller->history_list.end() - 1);
        Controller->history_hitcount_map.emplace(buf->tag, buf->hitcount);
        buf->hitcount = 0;
        CheckHistoryListSize();
        SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->history_list_lock);

        buf->buftypef = NodeType::NONE;
        buf->iter = Controller->cold_list.head();
        buf->hitcount = 0;
        *buf_state = local_buf_state;
        return buf;
    } else if (--try_counter == 0) {
        UnlockBufHdr(buf, local_buf_state);
        ereport(ERROR, (errcode(ERRCODE_INVALID_BUFFER), (errmsg("no unpinned buffers available"))));
    }
    UnlockBufHdr(buf, local_buf_state);
    goto retry;

    return NULL;

/*
retry:
    int try_get_loc_times = 10;
    auto Controller = t_thrd.storage_cxt.StrategyControl;
    int buf_id = Controller->cold_list.front();
    Controller->cold_list.pop_front();
    Controller->cold_size--;
    Controller->cold_map.erase(buf_id);

    buf = GetBufferDescriptor(buf_id);
    local_buf_state = LockBufHdr(buf);

    if (BUF_STATE_GET_REFCOUNT(local_buf_state) == 0 && !(local_buf_state & BM_IS_META) &&
            (backend_can_flush_dirty_page() || !(local_buf_state & BM_DIRTY))) {
            if (strategy != NULL)
                AddBufferToRing(strategy, buf);
            *buf_state = local_buf_state;
            (void)pg_atomic_fetch_add_u64(&g_instance.ckpt_cxt_ctl->get_buf_num_clock_sweep, 1);
            return buf;
    } else {
        UnlockBufHdr(buf, local_buf_state);
        ereport(ERROR, (errcode(ERRCODE_INVALID_BUFFER), (errmsg("no unpinned buffers available"))));
    }
    UnlockBufHdr(buf, local_buf_state);
    return NULL;
  */
}

/*
 * StrategyGetBuffer
 *
 *	Called by the bufmgr to get the next candidate buffer to use in
 *	BufferAlloc(). The only hard requirement BufferAlloc() has is that
 *	the selected buffer must not currently be pinned by anyone.
 *
 *	strategy is a BufferAccessStrategy object, or NULL for default strategy.
 *
 *	To ensure that no one else can pin the buffer before we do, we must
 *	return the buffer with the buffer header spinlock still held.
 *
 *  If Standby, we restrict its memory usage to shared_buffers_fraction of
 *  NBuffers, Standby will not get buffer from freelist to avoid touching all
 *  buffers and always run the "clock sweep" in shared_buffers_fraction * NBuffers.
 *  If the fraction is too small, we will increase dynamiclly to avoid elog(ERROR)
 *  in `Startup' process because of ERROR will promote to FATAL.
 */
BufferDesc* StrategyGetBuffer(BufferAccessStrategy strategy, uint32* buf_state)
{
    BufferDesc *buf = NULL;
    int bgwproc_no;
    int try_counter;
    uint32 local_buf_state = 0; /* to avoid repeated (de-)referencing */
    int max_buffer_can_use;
    bool am_standby = RecoveryInProgress();
    StrategyDelayStatus retry_lock_status = { 0, 0 };
    StrategyDelayStatus retry_buf_status = { 0, 0 };

    /*
     * If given a strategy object, see whether it can select a buffer. We
     * assume strategy objects don't need buffer_strategy_lock.
     */
    if (strategy != NULL) {
        buf = GetBufferFromRing(strategy, buf_state);
        if (buf != NULL) {
            return buf;
        }
    }

    /*
     * If asked, we need to waken the bgwriter. Since we don't want to rely on
     * a spinlock for this we force a read from shared memory once, and then
     * set the latch based on that value. We need to go through that length
     * because otherwise bgprocno might be reset while/after we check because
     * the compiler might just reread from memory.
     *
     * This can possibly set the latch of the wrong process if the bgwriter
     * dies in the wrong moment. But since PGPROC->procLatch is never
     * deallocated the worst consequence of that is that we set the latch of
     * some arbitrary process.
     */
    bgwproc_no = INT_ACCESS_ONCE(t_thrd.storage_cxt.StrategyControl->bgwprocno);
    if (bgwproc_no != -1) {
        /* reset bgwprocno first, before setting the latch */
        t_thrd.storage_cxt.StrategyControl->bgwprocno = -1;

        /*
         * Not acquiring ProcArrayLock here which is slightly icky. It's
         * actually fine because procLatch isn't ever freed, so we just can
         * potentially set the wrong process' (or no process') latch.
         */
        SetLatch(&g_instance.proc_base_all_procs[bgwproc_no]->procLatch);
    }

    /*
     * We count buffer allocation requests so that the bgwriter can estimate
     * the rate of buffer consumption.	Note that buffers recycled by a
     * strategy object are intentionally not counted here.
     */
    (void)pg_atomic_fetch_add_u32(&t_thrd.storage_cxt.StrategyControl->numBufferAllocs, 1);

    /* Check the Candidate list */
    if (ENABLE_INCRE_CKPT && pg_atomic_read_u32(&g_instance.ckpt_cxt_ctl->current_page_writer_count) > 1) {
        if (NEED_CONSIDER_USECOUNT) {
            const uint32 MAX_RETRY_SCAN_CANDIDATE_LISTS = 5;
            const int MILLISECOND_TO_MICROSECOND = 1000;
            uint64 maxSleep = u_sess->attr.attr_storage.BgWriterDelay * MILLISECOND_TO_MICROSECOND;
            uint64 sleepTime = 1000L;
            uint32 retry_times = 0;
            while (retry_times < MAX_RETRY_SCAN_CANDIDATE_LISTS) {
                buf = get_buf_from_candidate_list(strategy, buf_state);
                if (buf != NULL) {
                    (void)pg_atomic_fetch_add_u64(&g_instance.ckpt_cxt_ctl->get_buf_num_candidate_list, 1);
                    return buf;
                }
                pg_usleep(sleepTime);
                sleepTime = Min(sleepTime * 2, maxSleep);
                retry_times++;
            }
        } else {
            buf = get_buf_from_candidate_list(strategy, buf_state);
            if (buf != NULL) {
                (void)pg_atomic_fetch_add_u64(&g_instance.ckpt_cxt_ctl->get_buf_num_candidate_list, 1);
                return buf;
            }
        }
    }

retry:
    /* Nothing on the freelist, so run the "clock sweep" algorithm */
    if (am_standby)
        max_buffer_can_use = int(NORMAL_SHARED_BUFFER_NUM * u_sess->attr.attr_storage.shared_buffers_fraction);
    else
        max_buffer_can_use = NORMAL_SHARED_BUFFER_NUM;
    try_counter = max_buffer_can_use;
    int try_get_loc_times = max_buffer_can_use;
    for (;;) {
        buf = GetBufferDescriptor(ClockSweepTick(max_buffer_can_use));
        /*
         * If the buffer is pinned, we cannot use it.
         */
        if (!retryLockBufHdr(buf, &local_buf_state)) {
            if (--try_get_loc_times == 0) {
                ereport(WARNING,
                        (errmsg("try get buf headr lock times equal to maxNBufferCanUse when StrategyGetBuffer")));
                try_get_loc_times = max_buffer_can_use;
            }
            perform_delay(&retry_lock_status);
            continue;
        }

        retry_lock_status.retry_times = 0;
        if (BUF_STATE_GET_REFCOUNT(local_buf_state) == 0 && !(local_buf_state & BM_IS_META) &&
            (backend_can_flush_dirty_page() || !(local_buf_state & BM_DIRTY))) {
            /* Found a usable buffer */
            if (strategy != NULL)
                AddBufferToRing(strategy, buf);
            *buf_state = local_buf_state;
            (void)pg_atomic_fetch_add_u64(&g_instance.ckpt_cxt_ctl->get_buf_num_clock_sweep, 1);
            return buf;
        } else if (--try_counter == 0) {
            /*
             * We've scanned all the buffers without making any state changes,
             * so all the buffers are pinned (or were when we looked at them).
             * We could hope that someone will free one eventually, but it's
             * probably better to fail than to risk getting stuck in an
             * infinite loop.
             */
            UnlockBufHdr(buf, local_buf_state);

            if (am_standby && u_sess->attr.attr_storage.shared_buffers_fraction < 1.0) {
                ereport(WARNING, (errmsg("no unpinned buffers available")));
                u_sess->attr.attr_storage.shared_buffers_fraction =
                    Min(u_sess->attr.attr_storage.shared_buffers_fraction + 0.1, 1.0);
                goto retry;
            } else if (dw_page_writer_running()) {
                ereport(LOG, (errmsg("double writer is on, no buffer available, this buffer dirty is %u, "
                                     "this buffer refcount is %u, now dirty page num is %ld",
                                     (local_buf_state & BM_DIRTY), BUF_STATE_GET_REFCOUNT(local_buf_state),
                                     get_dirty_page_num())));
                perform_delay(&retry_buf_status);
                goto retry;
            } else if (t_thrd.storage_cxt.is_btree_split) {
                ereport(WARNING, (errmsg("no unpinned buffers available when btree insert parent")));
                goto retry;
            } else
                ereport(ERROR, (errcode(ERRCODE_INVALID_BUFFER), (errmsg("no unpinned buffers available"))));
        }
        UnlockBufHdr(buf, local_buf_state);
        perform_delay(&retry_buf_status);
    }

    /* not reached */
    return NULL;
}

/*
 * StrategySyncStart -- tell BufferSync where to start syncing
 *
 * The result is the buffer index of the best buffer to sync first.
 * BufferSync() will proceed circularly around the buffer array from there.
 *
 * In addition, we return the completed-pass count (which is effectively
 * the higher-order bits of nextVictimBuffer) and the count of recent buffer
 * allocs if non-NULL pointers are passed.	The alloc count is reset after
 * being read.
 */
int StrategySyncStart(uint32 *complete_passes, uint32 *num_buf_alloc)
{
    uint32 next_victim_buffer;
    int result;

    SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->buffer_strategy_lock);
    next_victim_buffer = pg_atomic_read_u32(&t_thrd.storage_cxt.StrategyControl->nextVictimBuffer);
    result = ((int) next_victim_buffer) % NORMAL_SHARED_BUFFER_NUM;

    if (complete_passes != NULL) {
        *complete_passes = t_thrd.storage_cxt.StrategyControl->completePasses;
        /*
         * Additionally add the number of wraparounds that happened before
         * completePasses could be incremented. C.f. ClockSweepTick().
         */
        *complete_passes += next_victim_buffer / (unsigned int) NORMAL_SHARED_BUFFER_NUM;
    }

    if (num_buf_alloc != NULL) {
        *num_buf_alloc = pg_atomic_exchange_u32(&t_thrd.storage_cxt.StrategyControl->numBufferAllocs, 0);
    }
    SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->buffer_strategy_lock);
    return result;
}

/*
 * StrategyNotifyBgWriter -- set or clear allocation notification latch
 *
 * If bgwriterLatch isn't NULL, the next invocation of StrategyGetBuffer will
 * set that latch.	Pass NULL to clear the pending notification before it
 * happens.  This feature is used by the bgwriter process to wake itself up
 * from hibernation, and is not meant for anybody else to use.
 */
void StrategyNotifyBgWriter(int bgwproc_no)
{
    /*
     * We acquire the BufFreelistLock just to ensure that the store appears
     * atomic to StrategyGetBuffer.  The bgwriter should call this rather
     * infrequently, so there's no performance penalty from being safe.
     */
    SpinLockAcquire(&t_thrd.storage_cxt.StrategyControl->buffer_strategy_lock);
    t_thrd.storage_cxt.StrategyControl->bgwprocno = bgwproc_no;
    SpinLockRelease(&t_thrd.storage_cxt.StrategyControl->buffer_strategy_lock);
}

/*
 * StrategyShmemSize
 *
 * estimate the size of shared memory used by the freelist-related structures.
 *
 * Note: for somewhat historical reasons, the buffer lookup hashtable size
 * is also determined here.
 */
Size StrategyShmemSize(void)
{
    Size size = 0;

    /* size of lookup hash table ... see comment in StrategyInitialize */
    size = add_size(size, BufTableShmemSize(TOTAL_BUFFER_NUM + NUM_BUFFER_PARTITIONS));

    /* size of the shared replacement strategy control block */
    size = add_size(size, MAXALIGN(sizeof(BufferStrategyControl)));

    return size;
}

/*
 * StrategyInitialize -- initialize the buffer cache replacement
 *		strategy.
 *
 * Assumes: All of the buffers are already built into a linked list.
 *		Only called by postmaster and only during initialization.
 */
void StrategyInitialize(bool init)
{
    bool found = false;

    /*
     * Initialize the shared buffer lookup hashtable.
     *
     * Since we can't tolerate running out of lookup table entries, we must be
     * sure to specify an adequate table size here.  The maximum steady-state
     * usage is of course NBuffers entries, but BufferAlloc() tries to insert
     * a new entry before deleting the old.  In principle this could be
     * happening in each partition concurrently, so we could need as many as
     * NBuffers + NUM_BUFFER_PARTITIONS entries.
     */
    InitBufTable(TOTAL_BUFFER_NUM + NUM_BUFFER_PARTITIONS);

    /*
     * Get or create the shared strategy control block
     */
    t_thrd.storage_cxt.StrategyControl =
        (BufferStrategyControl *)ShmemInitStruct("Buffer Strategy Status", sizeof(BufferStrategyControl), &found);

    if (!found) {
        /*
         * Only done once, usually in postmaster
         */
        Assert(init);
        SpinLockInit(&t_thrd.storage_cxt.StrategyControl->buffer_strategy_lock);

        /* Initialize the clock sweep pointer */
        pg_atomic_init_u32(&t_thrd.storage_cxt.StrategyControl->nextVictimBuffer, 0);

        /* Clear statistics */
        t_thrd.storage_cxt.StrategyControl->completePasses = 0;
        pg_atomic_init_u32(&t_thrd.storage_cxt.StrategyControl->numBufferAllocs, 0);

        /* No pending notification */
        t_thrd.storage_cxt.StrategyControl->bgwprocno = -1;

        t_thrd.storage_cxt.StrategyControl->cold_size = 0;
        t_thrd.storage_cxt.StrategyControl->history_size = 0;
        t_thrd.storage_cxt.StrategyControl->hot_size = 0;

        for (int i = 0; i < TOTAL_BUFFER_NUM; i++) {
            buf->buftype = NodeType::NONE;
            buf->hitcount = -1;
            buf->iter = t_thrd.storage_cxt.StrategyControl->cold_list.head();
        }
    } else {
        Assert(!init);
    }
}

const int MIN_REPAIR_FILE_SLOT_NUM = 32;
/* ----------------------------------------------------------------
 *				Backend-private buffer ring management
 * ----------------------------------------------------------------
 */
/*
 * GetAccessStrategy -- create a BufferAccessStrategy object
 *
 * The object is allocated in the current memory context.
 */
BufferAccessStrategy GetAccessStrategy(BufferAccessStrategyType btype)
{
    BufferAccessStrategy strategy;
    int ring_size;

    /*
     * Select ring size to use.  See buffer/README for rationales.
     *
     * Note: if you change the ring size for BAS_BULKREAD, see also
     * SYNC_SCAN_REPORT_INTERVAL in access/heap/syncscan.c.
     */
    switch (btype) {
        case BAS_NORMAL:
            /* if someone asks for NORMAL, just give 'em a "default" object */
            return NULL;

        case BAS_BULKREAD:
            ring_size = int(int64(u_sess->attr.attr_storage.bulk_read_ring_size) * 1024 / BLCKSZ);
            break;
        case BAS_BULKWRITE:
            ring_size = (u_sess->attr.attr_storage.bulk_write_ring_size / BLCKSZ) * 1024;
            break;
        case BAS_VACUUM:
            ring_size = g_instance.attr.attr_storage.NBuffers / 32 /
                Max(g_instance.attr.attr_storage.autovacuum_max_workers, 1);
            break;
        case BAS_REPAIR:
            ring_size = Min(g_instance.attr.attr_storage.NBuffers, MIN_REPAIR_FILE_SLOT_NUM);
            break;
        default:
            ereport(ERROR, (errcode(ERRCODE_INVALID_OPERATION),
                            (errmsg("unrecognized buffer access strategy: %d", (int)btype))));
            return NULL; /* keep compiler quiet */
    }

    /* If the shared buffers is too small, make sure ring size not equal zero. */
    ring_size = Max(ring_size, 4);

    /* Make sure ring isn't an undue fraction of shared buffers */
    if (btype != BAS_BULKWRITE && btype != BAS_BULKREAD)
        ring_size = Min(g_instance.attr.attr_storage.NBuffers / 8, ring_size);
    else
        ring_size = Min(g_instance.attr.attr_storage.NBuffers / 4, ring_size);

    /* Allocate the object and initialize all elements to zeroes */
    strategy = (BufferAccessStrategy)palloc0(offsetof(BufferAccessStrategyData, buffers) + ring_size * sizeof(Buffer));

    /* Set fields that don't start out zero */
    strategy->btype = btype;
    strategy->ring_size = ring_size;
    strategy->flush_rate = Min(u_sess->attr.attr_storage.backwrite_quantity, ring_size);

    return strategy;
}

/*
 * FreeAccessStrategy -- release a BufferAccessStrategy object
 *
 * A simple pfree would do at the moment, but we would prefer that callers
 * don't assume that much about the representation of BufferAccessStrategy.
 */
void FreeAccessStrategy(BufferAccessStrategy strategy)
{
    /* don't crash if called on a "default" strategy */
    if (strategy != NULL) {
        pfree(strategy);
        strategy = NULL;
    }
}

const int MAX_RETRY_RING_TIMES = 100;
const float MAX_RETRY_RING_PCT = 0.1;
/*
 * GetBufferFromRing -- returns a buffer from the ring, or NULL if the
 *		ring is empty.
 *
 * The bufhdr spin lock is held on the returned buffer.
 */
static BufferDesc *GetBufferFromRing(BufferAccessStrategy strategy, uint32 *buf_state)
{
    BufferDesc *buf = NULL;
    Buffer buf_num;
    uint32 local_buf_state; /* to avoid repeated (de-)referencing */
    uint16 retry_times = 0;

RETRY:
    /* Advance to next ring slot */
    if (++strategy->current >= strategy->ring_size)
        strategy->current = 0;
    retry_times++;

    ADIO_RUN()
    {
        /*
         * Flush out buffers asynchronously from behind the current slot.
         * This is a kludge because the PageListBackWrite() is not strictly
         * asynchronous and this function really shouldn't be doing the actual I/O.
         */
        if (AioCompltrIsReady() &&
            ((strategy->btype == BAS_BULKWRITE) && (strategy->current % strategy->flush_rate == 0))) {
            if (strategy->current == 0) {
                if (strategy->buffers[strategy->ring_size - strategy->flush_rate] != InvalidBuffer) {
                    PageListBackWrite((uint32 *)&strategy->buffers[strategy->ring_size - strategy->flush_rate],
                                      strategy->flush_rate, STRATEGY_BACKWRITE, NULL, NULL, NULL);
                    ereport(DEBUG1,
                            (errmodule(MOD_ADIO), errmsg("BufferRingBackWrite, start(%d) count(%d)",
                                                         strategy->buffers[strategy->ring_size - strategy->flush_rate],
                                                         strategy->flush_rate)));
                }
            } else {
                PageListBackWrite((uint32 *)&strategy->buffers[strategy->current - strategy->flush_rate],
                                  strategy->flush_rate, STRATEGY_BACKWRITE, NULL, NULL, NULL);
                ereport(DEBUG1,
                        (errmodule(MOD_ADIO),
                         errmsg("BufferRingBackWrite, start(%d) count(%d)",
                                strategy->buffers[strategy->current - strategy->flush_rate], strategy->flush_rate)));
            }
        }
    }
    ADIO_END();

    /*
     * If the slot hasn't been filled yet, tell the caller to allocate a new
     * buffer with the normal allocation strategy.	He will then fill this
     * slot by calling AddBufferToRing with the new buffer.
     */
    buf_num = strategy->buffers[strategy->current];
    if (buf_num == InvalidBuffer) {
        strategy->current_was_in_ring = false;
        return NULL;
    }

    /*
     * If the buffer is pinned we cannot use it under any circumstances.
     *
     * If usage_count is 0 or 1 then the buffer is fair game (we expect 1,
     * since our own previous usage of the ring element would have left it
     * there, but it might've been decremented by clock sweep since then). A
     * higher usage_count indicates someone else has touched the buffer, so we
     * shouldn't re-use it.
     */
    buf = GetBufferDescriptor(buf_num - 1);
    if (pg_atomic_read_u32(&buf->state) & (BM_DIRTY | BM_IS_META)) {
        if (retry_times < Min(MAX_RETRY_RING_TIMES, strategy->ring_size * MAX_RETRY_RING_PCT)) {
            goto RETRY;
        } else if (get_curr_candidate_nums(CAND_LIST_NORMAL) >= (uint32)g_instance.attr.attr_storage.NBuffers *
            u_sess->attr.attr_storage.candidate_buf_percent_target){
            strategy->current_was_in_ring = false;
            return NULL;
        }
    }

    local_buf_state = LockBufHdr(buf);
    if (BUF_STATE_GET_REFCOUNT(local_buf_state) == 0 && BUF_STATE_GET_USAGECOUNT(local_buf_state) <= 1 &&
        (backend_can_flush_dirty_page() || !(local_buf_state & BM_DIRTY)) &&
        !(local_buf_state & BM_IS_META)) {
        strategy->current_was_in_ring = true;
        *buf_state = local_buf_state;
        return buf;
    }

    UnlockBufHdr(buf, local_buf_state);
    /*
     * Tell caller to allocate a new buffer with the normal allocation
     * strategy.  He'll then replace this ring element via AddBufferToRing.
     */
    strategy->current_was_in_ring = false;
    return NULL;
}

/*
 * AddBufferToRing -- add a buffer to the buffer ring
 *
 * Caller must hold the buffer header spinlock on the buffer.  Since this
 * is called with the spinlock held, it had better be quite cheap.
 */
static void AddBufferToRing(BufferAccessStrategy strategy, volatile BufferDesc *buf)
{
    strategy->buffers[strategy->current] = BufferDescriptorGetBuffer(buf);
}

/*
 * StrategyRejectBuffer -- consider rejecting a dirty buffer
 *
 * When a nondefault strategy is used, the buffer manager calls this function
 * when it turns out that the buffer selected by StrategyGetBuffer needs to
 * be written out and doing so would require flushing WAL too.	This gives us
 * a chance to choose a different victim.
 *
 * Returns true if buffer manager should ask for a new victim, and false
 * if this buffer should be written and re-used.
 */
bool StrategyRejectBuffer(BufferAccessStrategy strategy, BufferDesc *buf)
{
    /* We only do this in bulkread mode */
    if (strategy->btype != BAS_BULKREAD)
        return false;

    /* Don't muck with behavior of normal buffer-replacement strategy */
    if (!strategy->current_was_in_ring || strategy->buffers[strategy->current] != BufferDescriptorGetBuffer(buf))
        return false;

    /*
     * Remove the dirty buffer from the ring; necessary to prevent infinite
     * loop if all ring members are dirty.
     */
    strategy->buffers[strategy->current] = InvalidBuffer;

    return true;
}

void StrategyGetRingPrefetchQuantityAndTrigger(BufferAccessStrategy strategy, int *quantity, int *trigger)
{
    int threshold;
    int prefetch_trigger = u_sess->attr.attr_storage.prefetch_quantity;

    if (strategy == NULL || strategy->btype != BAS_BULKREAD) {
        return;
    }
    threshold = strategy->ring_size / 4;
    if (quantity != NULL) {
        *quantity = (threshold > u_sess->attr.attr_storage.prefetch_quantity)
                        ? u_sess->attr.attr_storage.prefetch_quantity
                        : threshold;
    }
    if (trigger != NULL) {
        *trigger = (threshold > prefetch_trigger) ? prefetch_trigger : threshold;
    }
}

void wakeup_pagewriter_thread()
{
    PageWriterProc *pgwr = &g_instance.ckpt_cxt_ctl->pgwr_procs.writer_proc[0];
    /* The current candidate list is empty, wake up the buffer writer. */
    if (pgwr->proc != NULL) {
        SetLatch(&pgwr->proc->procLatch);
    }
    return;
}

const int CANDIDATE_DIRTY_LIST_LEN = 100;
const float HIGH_WATER = 0.75;
static BufferDesc* get_buf_from_candidate_list(BufferAccessStrategy strategy, uint32* buf_state)
{
    BufferDesc* buf = NULL;
    uint32 local_buf_state;
    int buf_id = 0;
    int list_num = g_instance.ckpt_cxt_ctl->pgwr_procs.sub_num;
    int list_id = 0;
    volatile PgBackendStatus* beentry = t_thrd.shemem_ptr_cxt.MyBEEntry;
    Buffer *candidate_dirty_list = NULL;
    int dirty_list_num = 0;
    bool enable_available = false;
    bool need_push_dirst_list = false;
    bool need_scan_dirty =
        (g_instance.ckpt_cxt_ctl->actual_dirty_page_num / (float)(g_instance.attr.attr_storage.NBuffers) > HIGH_WATER)
        && backend_can_flush_dirty_page();
    if (need_scan_dirty) {
        /*Not return the dirty page when there are few dirty pages */
        candidate_dirty_list = (Buffer*)palloc0(sizeof(Buffer) * CANDIDATE_DIRTY_LIST_LEN);
    }

    list_id = beentry->st_tid > 0 ? (beentry->st_tid % list_num) : (beentry->st_sessionid % list_num);

    for (int i = 0; i < list_num; i++) {
        /* the pagewriter sub thread store normal buffer pool, sub thread starts from 1 */
        int thread_id = (list_id + i) % list_num + 1;
        Assert(thread_id > 0 && thread_id <= list_num);
        while (candidate_buf_pop(&g_instance.ckpt_cxt_ctl->pgwr_procs.writer_proc[thread_id].normal_list, &buf_id)) {
            Assert(buf_id < SegmentBufferStartID);
            buf = GetBufferDescriptor(buf_id);
            local_buf_state = LockBufHdr(buf);

            if (g_instance.ckpt_cxt_ctl->candidate_free_map[buf_id]) {
                g_instance.ckpt_cxt_ctl->candidate_free_map[buf_id] = false;
                enable_available = BUF_STATE_GET_REFCOUNT(local_buf_state) == 0 && !(local_buf_state & BM_IS_META);
                need_push_dirst_list = need_scan_dirty && dirty_list_num < CANDIDATE_DIRTY_LIST_LEN &&
                        free_space_enough(buf_id);
                if (enable_available) {
                    if (NEED_CONSIDER_USECOUNT && BUF_STATE_GET_USAGECOUNT(local_buf_state) != 0) {
                        local_buf_state -= BUF_USAGECOUNT_ONE;
                    } else if (!(local_buf_state & BM_DIRTY)) {
                        if (strategy != NULL) {
                            AddBufferToRing(strategy, buf);
                        }
                        *buf_state = local_buf_state;
                        if (candidate_dirty_list != NULL) {
                            pfree(candidate_dirty_list);
                        }
                        return buf;
                    } else if (need_push_dirst_list) {
                        candidate_dirty_list[dirty_list_num++] = buf_id;
                    }
                }
            }
            UnlockBufHdr(buf, local_buf_state);
        }
    }

    wakeup_pagewriter_thread();

    if (need_scan_dirty) {
        for (int i = 0; i < dirty_list_num; i++) {
            buf_id = candidate_dirty_list[i];
            buf = GetBufferDescriptor(buf_id);
            local_buf_state = LockBufHdr(buf);
            enable_available = (BUF_STATE_GET_REFCOUNT(local_buf_state) == 0) && !(local_buf_state & BM_IS_META)
                && free_space_enough(buf_id);
            if (enable_available) {
                if (strategy != NULL) {
                    AddBufferToRing(strategy, buf);
                }
                *buf_state = local_buf_state;
                pfree(candidate_dirty_list);
                return buf;
            }
            UnlockBufHdr(buf, local_buf_state);
        }
    }

    if (candidate_dirty_list != NULL) {
        pfree(candidate_dirty_list);
    }
    return NULL;
}

