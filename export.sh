export CODE_BASE=/home/zya/zya/work/openGauss-server
export BINARYLIBS=/home/zya/zya/work/binarylibs
export GAUSSHOME=$CODE_BASE/dest/
export GCC_PATH=$BINARYLIBS/buildtools/gcc7.3
export CC=$GCC_PATH/gcc/bin/gcc
export CXX=$GCC_PATH/gcc/bin/g++
export LD_LIBRARY_PATH=$GAUSSHOME/lib:$GCC_PATH/gcc/lib64:$GCC_PATH/isl/lib:$GCC_PATH/mpc/lib/:$GCC_PATH/mpfr/lib/:$GCC_PATH/gmp/lib/:$LD_LIBRARY_PATH
export PATH=$GAUSSHOME/bin:$GCC_PATH/gcc/bin:$PATH

sh build.sh -m release -3rd /home/zya/zya/work/binarylibs
export CODE BASE=/home/zya/zya/work/openGauss-server
export GAUSSHOME=$CODE_BASE/mppdb_temp_install/
export LD_LIBRARY_PATH=$GAUSSHOME/1ib:$LD_LIBRARY_PATH
export PATH=$GAUSSHOME/bin:$PATH