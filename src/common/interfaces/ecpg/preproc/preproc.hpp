/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_BASE_YY_PREPROC_HPP_INCLUDED
# define YY_BASE_YY_PREPROC_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int base_yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SQL_ALLOCATE = 258,            /* SQL_ALLOCATE  */
    SQL_AUTOCOMMIT = 259,          /* SQL_AUTOCOMMIT  */
    SQL_BOOL = 260,                /* SQL_BOOL  */
    SQL_BREAK = 261,               /* SQL_BREAK  */
    SQL_CALL = 262,                /* SQL_CALL  */
    SQL_CARDINALITY = 263,         /* SQL_CARDINALITY  */
    SQL_COUNT = 264,               /* SQL_COUNT  */
    SQL_DATETIME_INTERVAL_CODE = 265, /* SQL_DATETIME_INTERVAL_CODE  */
    SQL_DATETIME_INTERVAL_PRECISION = 266, /* SQL_DATETIME_INTERVAL_PRECISION  */
    SQL_DESCRIBE = 267,            /* SQL_DESCRIBE  */
    SQL_DESCRIPTOR = 268,          /* SQL_DESCRIPTOR  */
    SQL_FOUND = 269,               /* SQL_FOUND  */
    SQL_FREE = 270,                /* SQL_FREE  */
    SQL_GET = 271,                 /* SQL_GET  */
    SQL_GO = 272,                  /* SQL_GO  */
    SQL_GOTO = 273,                /* SQL_GOTO  */
    SQL_IDENTIFIED = 274,          /* SQL_IDENTIFIED  */
    SQL_INDICATOR = 275,           /* SQL_INDICATOR  */
    SQL_KEY_MEMBER = 276,          /* SQL_KEY_MEMBER  */
    SQL_LENGTH = 277,              /* SQL_LENGTH  */
    SQL_LONG = 278,                /* SQL_LONG  */
    SQL_NULLABLE = 279,            /* SQL_NULLABLE  */
    SQL_OCTET_LENGTH = 280,        /* SQL_OCTET_LENGTH  */
    SQL_OPEN = 281,                /* SQL_OPEN  */
    SQL_OUTPUT = 282,              /* SQL_OUTPUT  */
    SQL_REFERENCE = 283,           /* SQL_REFERENCE  */
    SQL_RETURNED_LENGTH = 284,     /* SQL_RETURNED_LENGTH  */
    SQL_RETURNED_OCTET_LENGTH = 285, /* SQL_RETURNED_OCTET_LENGTH  */
    SQL_SCALE = 286,               /* SQL_SCALE  */
    SQL_SECTION = 287,             /* SQL_SECTION  */
    SQL_SHORT = 288,               /* SQL_SHORT  */
    SQL_SIGNED = 289,              /* SQL_SIGNED  */
    SQL_SQL = 290,                 /* SQL_SQL  */
    SQL_SQLERROR = 291,            /* SQL_SQLERROR  */
    SQL_SQLPRINT = 292,            /* SQL_SQLPRINT  */
    SQL_SQLWARNING = 293,          /* SQL_SQLWARNING  */
    SQL_START = 294,               /* SQL_START  */
    SQL_STOP = 295,                /* SQL_STOP  */
    SQL_STRUCT = 296,              /* SQL_STRUCT  */
    SQL_UNSIGNED = 297,            /* SQL_UNSIGNED  */
    SQL_VAR = 298,                 /* SQL_VAR  */
    SQL_WHENEVER = 299,            /* SQL_WHENEVER  */
    S_ADD = 300,                   /* S_ADD  */
    S_AND = 301,                   /* S_AND  */
    S_ANYTHING = 302,              /* S_ANYTHING  */
    S_AUTO = 303,                  /* S_AUTO  */
    S_CONST = 304,                 /* S_CONST  */
    S_DEC = 305,                   /* S_DEC  */
    S_DIV = 306,                   /* S_DIV  */
    S_DOTPOINT = 307,              /* S_DOTPOINT  */
    S_EQUAL = 308,                 /* S_EQUAL  */
    S_EXTERN = 309,                /* S_EXTERN  */
    S_INC = 310,                   /* S_INC  */
    S_LSHIFT = 311,                /* S_LSHIFT  */
    S_MEMPOINT = 312,              /* S_MEMPOINT  */
    S_MEMBER = 313,                /* S_MEMBER  */
    S_MOD = 314,                   /* S_MOD  */
    S_MUL = 315,                   /* S_MUL  */
    S_NEQUAL = 316,                /* S_NEQUAL  */
    S_OR = 317,                    /* S_OR  */
    S_REGISTER = 318,              /* S_REGISTER  */
    S_RSHIFT = 319,                /* S_RSHIFT  */
    S_STATIC = 320,                /* S_STATIC  */
    S_SUB = 321,                   /* S_SUB  */
    S_VOLATILE = 322,              /* S_VOLATILE  */
    S_TYPEDEF = 323,               /* S_TYPEDEF  */
    CSTRING = 324,                 /* CSTRING  */
    CVARIABLE = 325,               /* CVARIABLE  */
    CPP_LINE = 326,                /* CPP_LINE  */
    SQL_IP = 327,                  /* SQL_IP  */
    DOLCONST = 328,                /* DOLCONST  */
    ECONST = 329,                  /* ECONST  */
    NCONST = 330,                  /* NCONST  */
    UCONST = 331,                  /* UCONST  */
    UIDENT = 332,                  /* UIDENT  */
    IDENT = 333,                   /* IDENT  */
    FCONST = 334,                  /* FCONST  */
    SCONST = 335,                  /* SCONST  */
    BCONST = 336,                  /* BCONST  */
    VCONST = 337,                  /* VCONST  */
    XCONST = 338,                  /* XCONST  */
    Op = 339,                      /* Op  */
    CmpOp = 340,                   /* CmpOp  */
    CmpNullOp = 341,               /* CmpNullOp  */
    COMMENTSTRING = 342,           /* COMMENTSTRING  */
    SET_USER_IDENT = 343,          /* SET_USER_IDENT  */
    SET_IDENT = 344,               /* SET_IDENT  */
    UNDERSCORE_CHARSET = 345,      /* UNDERSCORE_CHARSET  */
    ICONST = 346,                  /* ICONST  */
    PARAM = 347,                   /* PARAM  */
    TYPECAST = 348,                /* TYPECAST  */
    ORA_JOINOP = 349,              /* ORA_JOINOP  */
    DOT_DOT = 350,                 /* DOT_DOT  */
    COLON_EQUALS = 351,            /* COLON_EQUALS  */
    PARA_EQUALS = 352,             /* PARA_EQUALS  */
    SET_IDENT_SESSION = 353,       /* SET_IDENT_SESSION  */
    SET_IDENT_GLOBAL = 354,        /* SET_IDENT_GLOBAL  */
    ABORT_P = 355,                 /* ABORT_P  */
    ABSOLUTE_P = 356,              /* ABSOLUTE_P  */
    ACCESS = 357,                  /* ACCESS  */
    ACCOUNT = 358,                 /* ACCOUNT  */
    ACTION = 359,                  /* ACTION  */
    ADD_P = 360,                   /* ADD_P  */
    ADMIN = 361,                   /* ADMIN  */
    AFTER = 362,                   /* AFTER  */
    AGGREGATE = 363,               /* AGGREGATE  */
    ALGORITHM = 364,               /* ALGORITHM  */
    ALL = 365,                     /* ALL  */
    ALSO = 366,                    /* ALSO  */
    ALTER = 367,                   /* ALTER  */
    ALWAYS = 368,                  /* ALWAYS  */
    ANALYSE = 369,                 /* ANALYSE  */
    ANALYZE = 370,                 /* ANALYZE  */
    AND = 371,                     /* AND  */
    ANY = 372,                     /* ANY  */
    APP = 373,                     /* APP  */
    APPEND = 374,                  /* APPEND  */
    ARCHIVE = 375,                 /* ARCHIVE  */
    ARRAY = 376,                   /* ARRAY  */
    AS = 377,                      /* AS  */
    ASC = 378,                     /* ASC  */
    ASSERTION = 379,               /* ASSERTION  */
    ASSIGNMENT = 380,              /* ASSIGNMENT  */
    ASYMMETRIC = 381,              /* ASYMMETRIC  */
    AT = 382,                      /* AT  */
    ATTRIBUTE = 383,               /* ATTRIBUTE  */
    AUDIT = 384,                   /* AUDIT  */
    AUTHID = 385,                  /* AUTHID  */
    AUTHORIZATION = 386,           /* AUTHORIZATION  */
    AUTOEXTEND = 387,              /* AUTOEXTEND  */
    AUTOMAPPED = 388,              /* AUTOMAPPED  */
    AUTO_INCREMENT = 389,          /* AUTO_INCREMENT  */
    BACKWARD = 390,                /* BACKWARD  */
    BARRIER = 391,                 /* BARRIER  */
    BEFORE = 392,                  /* BEFORE  */
    BEGIN_NON_ANOYBLOCK = 393,     /* BEGIN_NON_ANOYBLOCK  */
    BEGIN_P = 394,                 /* BEGIN_P  */
    BETWEEN = 395,                 /* BETWEEN  */
    BIGINT = 396,                  /* BIGINT  */
    BINARY = 397,                  /* BINARY  */
    BINARY_DOUBLE = 398,           /* BINARY_DOUBLE  */
    BINARY_INTEGER = 399,          /* BINARY_INTEGER  */
    BIT = 400,                     /* BIT  */
    BLANKS = 401,                  /* BLANKS  */
    BLOB_P = 402,                  /* BLOB_P  */
    BLOCKCHAIN = 403,              /* BLOCKCHAIN  */
    BODY_P = 404,                  /* BODY_P  */
    BOGUS = 405,                   /* BOGUS  */
    BOOLEAN_P = 406,               /* BOOLEAN_P  */
    BOTH = 407,                    /* BOTH  */
    BUCKETCNT = 408,               /* BUCKETCNT  */
    BUCKETS = 409,                 /* BUCKETS  */
    BY = 410,                      /* BY  */
    BYTEAWITHOUTORDER = 411,       /* BYTEAWITHOUTORDER  */
    BYTEAWITHOUTORDERWITHEQUAL = 412, /* BYTEAWITHOUTORDERWITHEQUAL  */
    CACHE = 413,                   /* CACHE  */
    CALL = 414,                    /* CALL  */
    CALLED = 415,                  /* CALLED  */
    CANCELABLE = 416,              /* CANCELABLE  */
    CASCADE = 417,                 /* CASCADE  */
    CASCADED = 418,                /* CASCADED  */
    CASE = 419,                    /* CASE  */
    CAST = 420,                    /* CAST  */
    CATALOG_P = 421,               /* CATALOG_P  */
    CATALOG_NAME = 422,            /* CATALOG_NAME  */
    CHAIN = 423,                   /* CHAIN  */
    CHANGE = 424,                  /* CHANGE  */
    CHAR_P = 425,                  /* CHAR_P  */
    CHARACTER = 426,               /* CHARACTER  */
    CHARACTERISTICS = 427,         /* CHARACTERISTICS  */
    CHARACTERSET = 428,            /* CHARACTERSET  */
    CHARSET = 429,                 /* CHARSET  */
    CHECK = 430,                   /* CHECK  */
    CHECKPOINT = 431,              /* CHECKPOINT  */
    CLASS = 432,                   /* CLASS  */
    CLASS_ORIGIN = 433,            /* CLASS_ORIGIN  */
    CLEAN = 434,                   /* CLEAN  */
    CLIENT = 435,                  /* CLIENT  */
    CLIENT_MASTER_KEY = 436,       /* CLIENT_MASTER_KEY  */
    CLIENT_MASTER_KEYS = 437,      /* CLIENT_MASTER_KEYS  */
    CLOB = 438,                    /* CLOB  */
    CLOSE = 439,                   /* CLOSE  */
    CLUSTER = 440,                 /* CLUSTER  */
    COALESCE = 441,                /* COALESCE  */
    COLLATE = 442,                 /* COLLATE  */
    COLLATION = 443,               /* COLLATION  */
    COLUMN = 444,                  /* COLUMN  */
    COLUMN_ENCRYPTION_KEY = 445,   /* COLUMN_ENCRYPTION_KEY  */
    COLUMN_ENCRYPTION_KEYS = 446,  /* COLUMN_ENCRYPTION_KEYS  */
    COLUMN_NAME = 447,             /* COLUMN_NAME  */
    COLUMNS = 448,                 /* COLUMNS  */
    COMMENT = 449,                 /* COMMENT  */
    COMMENTS = 450,                /* COMMENTS  */
    COMMIT = 451,                  /* COMMIT  */
    COMMITTED = 452,               /* COMMITTED  */
    COMPACT = 453,                 /* COMPACT  */
    COMPATIBLE_ILLEGAL_CHARS = 454, /* COMPATIBLE_ILLEGAL_CHARS  */
    COMPLETE = 455,                /* COMPLETE  */
    COMPLETION = 456,              /* COMPLETION  */
    COMPRESS = 457,                /* COMPRESS  */
    CONCURRENTLY = 458,            /* CONCURRENTLY  */
    CONDITION = 459,               /* CONDITION  */
    CONFIGURATION = 460,           /* CONFIGURATION  */
    CONNECTION = 461,              /* CONNECTION  */
    CONSISTENT = 462,              /* CONSISTENT  */
    CONSTANT = 463,                /* CONSTANT  */
    CONSTRAINT = 464,              /* CONSTRAINT  */
    CONSTRAINT_CATALOG = 465,      /* CONSTRAINT_CATALOG  */
    CONSTRAINT_NAME = 466,         /* CONSTRAINT_NAME  */
    CONSTRAINT_SCHEMA = 467,       /* CONSTRAINT_SCHEMA  */
    CONSTRAINTS = 468,             /* CONSTRAINTS  */
    CONTENT_P = 469,               /* CONTENT_P  */
    CONTINUE_P = 470,              /* CONTINUE_P  */
    CONTVIEW = 471,                /* CONTVIEW  */
    CONVERSION_P = 472,            /* CONVERSION_P  */
    CONVERT_P = 473,               /* CONVERT_P  */
    CONNECT = 474,                 /* CONNECT  */
    COORDINATOR = 475,             /* COORDINATOR  */
    COORDINATORS = 476,            /* COORDINATORS  */
    COPY = 477,                    /* COPY  */
    COST = 478,                    /* COST  */
    CREATE = 479,                  /* CREATE  */
    CROSS = 480,                   /* CROSS  */
    CSN = 481,                     /* CSN  */
    CSV = 482,                     /* CSV  */
    CUBE = 483,                    /* CUBE  */
    CURRENT_P = 484,               /* CURRENT_P  */
    CURRENT_CATALOG = 485,         /* CURRENT_CATALOG  */
    CURRENT_DATE = 486,            /* CURRENT_DATE  */
    CURRENT_ROLE = 487,            /* CURRENT_ROLE  */
    CURRENT_SCHEMA = 488,          /* CURRENT_SCHEMA  */
    CURRENT_TIME = 489,            /* CURRENT_TIME  */
    CURRENT_TIMESTAMP = 490,       /* CURRENT_TIMESTAMP  */
    CURRENT_USER = 491,            /* CURRENT_USER  */
    CURSOR = 492,                  /* CURSOR  */
    CURSOR_NAME = 493,             /* CURSOR_NAME  */
    CYCLE = 494,                   /* CYCLE  */
    SHRINK = 495,                  /* SHRINK  */
    USE_P = 496,                   /* USE_P  */
    DATA_P = 497,                  /* DATA_P  */
    DATABASE = 498,                /* DATABASE  */
    DATAFILE = 499,                /* DATAFILE  */
    DATANODE = 500,                /* DATANODE  */
    DATANODES = 501,               /* DATANODES  */
    DATATYPE_CL = 502,             /* DATATYPE_CL  */
    DATE_P = 503,                  /* DATE_P  */
    DATE_FORMAT_P = 504,           /* DATE_FORMAT_P  */
    DAY_P = 505,                   /* DAY_P  */
    DAY_HOUR_P = 506,              /* DAY_HOUR_P  */
    DAY_MINUTE_P = 507,            /* DAY_MINUTE_P  */
    DAY_SECOND_P = 508,            /* DAY_SECOND_P  */
    DBCOMPATIBILITY_P = 509,       /* DBCOMPATIBILITY_P  */
    DEALLOCATE = 510,              /* DEALLOCATE  */
    DEC = 511,                     /* DEC  */
    DECIMAL_P = 512,               /* DECIMAL_P  */
    DECLARE = 513,                 /* DECLARE  */
    DECODE = 514,                  /* DECODE  */
    DEFAULT = 515,                 /* DEFAULT  */
    DEFAULTS = 516,                /* DEFAULTS  */
    DEFERRABLE = 517,              /* DEFERRABLE  */
    DEFERRED = 518,                /* DEFERRED  */
    DEFINER = 519,                 /* DEFINER  */
    DELETE_P = 520,                /* DELETE_P  */
    DELIMITER = 521,               /* DELIMITER  */
    DELIMITERS = 522,              /* DELIMITERS  */
    DELTA = 523,                   /* DELTA  */
    DELTAMERGE = 524,              /* DELTAMERGE  */
    DESC = 525,                    /* DESC  */
    DETERMINISTIC = 526,           /* DETERMINISTIC  */
    DIAGNOSTICS = 527,             /* DIAGNOSTICS  */
    DICTIONARY = 528,              /* DICTIONARY  */
    DIRECT = 529,                  /* DIRECT  */
    DIRECTORY = 530,               /* DIRECTORY  */
    DISABLE_P = 531,               /* DISABLE_P  */
    DISCARD = 532,                 /* DISCARD  */
    DISTINCT = 533,                /* DISTINCT  */
    DISTRIBUTE = 534,              /* DISTRIBUTE  */
    DISTRIBUTION = 535,            /* DISTRIBUTION  */
    DO = 536,                      /* DO  */
    DOCUMENT_P = 537,              /* DOCUMENT_P  */
    DOMAIN_P = 538,                /* DOMAIN_P  */
    DOUBLE_P = 539,                /* DOUBLE_P  */
    DROP = 540,                    /* DROP  */
    DUPLICATE = 541,               /* DUPLICATE  */
    DISCONNECT = 542,              /* DISCONNECT  */
    DUMPFILE = 543,                /* DUMPFILE  */
    EACH = 544,                    /* EACH  */
    ELASTIC = 545,                 /* ELASTIC  */
    ELSE = 546,                    /* ELSE  */
    ENABLE_P = 547,                /* ENABLE_P  */
    ENCLOSED = 548,                /* ENCLOSED  */
    ENCODING = 549,                /* ENCODING  */
    ENCRYPTED = 550,               /* ENCRYPTED  */
    ENCRYPTED_VALUE = 551,         /* ENCRYPTED_VALUE  */
    ENCRYPTION = 552,              /* ENCRYPTION  */
    ENCRYPTION_TYPE = 553,         /* ENCRYPTION_TYPE  */
    END_P = 554,                   /* END_P  */
    ENDS = 555,                    /* ENDS  */
    ENFORCED = 556,                /* ENFORCED  */
    ENUM_P = 557,                  /* ENUM_P  */
    ERRORS = 558,                  /* ERRORS  */
    ESCAPE = 559,                  /* ESCAPE  */
    EOL = 560,                     /* EOL  */
    ESCAPING = 561,                /* ESCAPING  */
    EVENT = 562,                   /* EVENT  */
    EVENTS = 563,                  /* EVENTS  */
    EVERY = 564,                   /* EVERY  */
    EXCEPT = 565,                  /* EXCEPT  */
    EXCHANGE = 566,                /* EXCHANGE  */
    EXCLUDE = 567,                 /* EXCLUDE  */
    EXCLUDED = 568,                /* EXCLUDED  */
    EXCLUDING = 569,               /* EXCLUDING  */
    EXCLUSIVE = 570,               /* EXCLUSIVE  */
    EXECUTE = 571,                 /* EXECUTE  */
    EXISTS = 572,                  /* EXISTS  */
    EXPIRED_P = 573,               /* EXPIRED_P  */
    EXPLAIN = 574,                 /* EXPLAIN  */
    EXTENSION = 575,               /* EXTENSION  */
    EXTERNAL = 576,                /* EXTERNAL  */
    EXTRACT = 577,                 /* EXTRACT  */
    ESCAPED = 578,                 /* ESCAPED  */
    FALSE_P = 579,                 /* FALSE_P  */
    FAMILY = 580,                  /* FAMILY  */
    FAST = 581,                    /* FAST  */
    FENCED = 582,                  /* FENCED  */
    FETCH = 583,                   /* FETCH  */
    FIELDS = 584,                  /* FIELDS  */
    FILEHEADER_P = 585,            /* FILEHEADER_P  */
    FILL_MISSING_FIELDS = 586,     /* FILL_MISSING_FIELDS  */
    FILLER = 587,                  /* FILLER  */
    FILTER = 588,                  /* FILTER  */
    FIRST_P = 589,                 /* FIRST_P  */
    FIXED_P = 590,                 /* FIXED_P  */
    FLOAT_P = 591,                 /* FLOAT_P  */
    FOLLOWING = 592,               /* FOLLOWING  */
    FOLLOWS_P = 593,               /* FOLLOWS_P  */
    FOR = 594,                     /* FOR  */
    FORCE = 595,                   /* FORCE  */
    FOREIGN = 596,                 /* FOREIGN  */
    FORMATTER = 597,               /* FORMATTER  */
    FORWARD = 598,                 /* FORWARD  */
    FEATURES = 599,                /* FEATURES  */
    FREEZE = 600,                  /* FREEZE  */
    FROM = 601,                    /* FROM  */
    FULL = 602,                    /* FULL  */
    FUNCTION = 603,                /* FUNCTION  */
    FUNCTIONS = 604,               /* FUNCTIONS  */
    GENERATED = 605,               /* GENERATED  */
    GET = 606,                     /* GET  */
    GLOBAL = 607,                  /* GLOBAL  */
    GRANT = 608,                   /* GRANT  */
    GRANTED = 609,                 /* GRANTED  */
    GREATEST = 610,                /* GREATEST  */
    GROUP_P = 611,                 /* GROUP_P  */
    GROUPING_P = 612,              /* GROUPING_P  */
    GROUPPARENT = 613,             /* GROUPPARENT  */
    HANDLER = 614,                 /* HANDLER  */
    HAVING = 615,                  /* HAVING  */
    HDFSDIRECTORY = 616,           /* HDFSDIRECTORY  */
    HEADER_P = 617,                /* HEADER_P  */
    HOLD = 618,                    /* HOLD  */
    HOUR_P = 619,                  /* HOUR_P  */
    HOUR_MINUTE_P = 620,           /* HOUR_MINUTE_P  */
    HOUR_SECOND_P = 621,           /* HOUR_SECOND_P  */
    IDENTIFIED = 622,              /* IDENTIFIED  */
    IDENTITY_P = 623,              /* IDENTITY_P  */
    IF_P = 624,                    /* IF_P  */
    IGNORE = 625,                  /* IGNORE  */
    IGNORE_EXTRA_DATA = 626,       /* IGNORE_EXTRA_DATA  */
    ILIKE = 627,                   /* ILIKE  */
    IMMEDIATE = 628,               /* IMMEDIATE  */
    IMMUTABLE = 629,               /* IMMUTABLE  */
    IMPLICIT_P = 630,              /* IMPLICIT_P  */
    IN_P = 631,                    /* IN_P  */
    INCLUDE = 632,                 /* INCLUDE  */
    INCLUDING = 633,               /* INCLUDING  */
    INCREMENT = 634,               /* INCREMENT  */
    INCREMENTAL = 635,             /* INCREMENTAL  */
    INDEX = 636,                   /* INDEX  */
    INDEXES = 637,                 /* INDEXES  */
    INFILE = 638,                  /* INFILE  */
    INHERIT = 639,                 /* INHERIT  */
    INHERITS = 640,                /* INHERITS  */
    INITIAL_P = 641,               /* INITIAL_P  */
    INITIALLY = 642,               /* INITIALLY  */
    INITRANS = 643,                /* INITRANS  */
    INLINE_P = 644,                /* INLINE_P  */
    INNER_P = 645,                 /* INNER_P  */
    INOUT = 646,                   /* INOUT  */
    INPUT_P = 647,                 /* INPUT_P  */
    INSENSITIVE = 648,             /* INSENSITIVE  */
    INSERT = 649,                  /* INSERT  */
    INSTEAD = 650,                 /* INSTEAD  */
    INT_P = 651,                   /* INT_P  */
    INTEGER = 652,                 /* INTEGER  */
    INTERNAL = 653,                /* INTERNAL  */
    INTERSECT = 654,               /* INTERSECT  */
    INTERVAL = 655,                /* INTERVAL  */
    INTO = 656,                    /* INTO  */
    INVISIBLE = 657,               /* INVISIBLE  */
    INVOKER = 658,                 /* INVOKER  */
    IP = 659,                      /* IP  */
    IS = 660,                      /* IS  */
    ISNULL = 661,                  /* ISNULL  */
    ISOLATION = 662,               /* ISOLATION  */
    JOIN = 663,                    /* JOIN  */
    KEY = 664,                     /* KEY  */
    KILL = 665,                    /* KILL  */
    KEY_PATH = 666,                /* KEY_PATH  */
    KEY_STORE = 667,               /* KEY_STORE  */
    LABEL = 668,                   /* LABEL  */
    LANGUAGE = 669,                /* LANGUAGE  */
    LARGE_P = 670,                 /* LARGE_P  */
    LAST_P = 671,                  /* LAST_P  */
    LC_COLLATE_P = 672,            /* LC_COLLATE_P  */
    LC_CTYPE_P = 673,              /* LC_CTYPE_P  */
    LEADING = 674,                 /* LEADING  */
    LEAKPROOF = 675,               /* LEAKPROOF  */
    LINES = 676,                   /* LINES  */
    LEAST = 677,                   /* LEAST  */
    LESS = 678,                    /* LESS  */
    LEFT = 679,                    /* LEFT  */
    LEVEL = 680,                   /* LEVEL  */
    LIKE = 681,                    /* LIKE  */
    LIMIT = 682,                   /* LIMIT  */
    LIST = 683,                    /* LIST  */
    LISTEN = 684,                  /* LISTEN  */
    LOAD = 685,                    /* LOAD  */
    LOCAL = 686,                   /* LOCAL  */
    LOCALTIME = 687,               /* LOCALTIME  */
    LOCALTIMESTAMP = 688,          /* LOCALTIMESTAMP  */
    LOCATION = 689,                /* LOCATION  */
    LOCK_P = 690,                  /* LOCK_P  */
    LOCKED = 691,                  /* LOCKED  */
    LOG_P = 692,                   /* LOG_P  */
    LOGGING = 693,                 /* LOGGING  */
    LOGIN_ANY = 694,               /* LOGIN_ANY  */
    LOGIN_FAILURE = 695,           /* LOGIN_FAILURE  */
    LOGIN_SUCCESS = 696,           /* LOGIN_SUCCESS  */
    LOGOUT = 697,                  /* LOGOUT  */
    LOOP = 698,                    /* LOOP  */
    MAPPING = 699,                 /* MAPPING  */
    MASKING = 700,                 /* MASKING  */
    MASTER = 701,                  /* MASTER  */
    MATCH = 702,                   /* MATCH  */
    MATERIALIZED = 703,            /* MATERIALIZED  */
    MATCHED = 704,                 /* MATCHED  */
    MAXEXTENTS = 705,              /* MAXEXTENTS  */
    MAXSIZE = 706,                 /* MAXSIZE  */
    MAXTRANS = 707,                /* MAXTRANS  */
    MAXVALUE = 708,                /* MAXVALUE  */
    MERGE = 709,                   /* MERGE  */
    MESSAGE_TEXT = 710,            /* MESSAGE_TEXT  */
    MINUS_P = 711,                 /* MINUS_P  */
    MINUTE_P = 712,                /* MINUTE_P  */
    MINUTE_SECOND_P = 713,         /* MINUTE_SECOND_P  */
    MINVALUE = 714,                /* MINVALUE  */
    MINEXTENTS = 715,              /* MINEXTENTS  */
    MODE = 716,                    /* MODE  */
    MODEL = 717,                   /* MODEL  */
    MODIFY_P = 718,                /* MODIFY_P  */
    MONTH_P = 719,                 /* MONTH_P  */
    MOVE = 720,                    /* MOVE  */
    MOVEMENT = 721,                /* MOVEMENT  */
    MYSQL_ERRNO = 722,             /* MYSQL_ERRNO  */
    NAME_P = 723,                  /* NAME_P  */
    NAMES = 724,                   /* NAMES  */
    NATIONAL = 725,                /* NATIONAL  */
    NATURAL = 726,                 /* NATURAL  */
    NCHAR = 727,                   /* NCHAR  */
    NEXT = 728,                    /* NEXT  */
    NO = 729,                      /* NO  */
    NOCOMPRESS = 730,              /* NOCOMPRESS  */
    NOCYCLE = 731,                 /* NOCYCLE  */
    NODE = 732,                    /* NODE  */
    NOLOGGING = 733,               /* NOLOGGING  */
    NOMAXVALUE = 734,              /* NOMAXVALUE  */
    NOMINVALUE = 735,              /* NOMINVALUE  */
    NONE = 736,                    /* NONE  */
    NOT = 737,                     /* NOT  */
    NOTHING = 738,                 /* NOTHING  */
    NOTIFY = 739,                  /* NOTIFY  */
    NOTNULL = 740,                 /* NOTNULL  */
    NOWAIT = 741,                  /* NOWAIT  */
    NULL_P = 742,                  /* NULL_P  */
    NULLCOLS = 743,                /* NULLCOLS  */
    NULLIF = 744,                  /* NULLIF  */
    NULLS_P = 745,                 /* NULLS_P  */
    NUMBER_P = 746,                /* NUMBER_P  */
    NUMERIC = 747,                 /* NUMERIC  */
    NUMSTR = 748,                  /* NUMSTR  */
    NVARCHAR = 749,                /* NVARCHAR  */
    NVARCHAR2 = 750,               /* NVARCHAR2  */
    NVL = 751,                     /* NVL  */
    OBJECT_P = 752,                /* OBJECT_P  */
    OF = 753,                      /* OF  */
    OFF = 754,                     /* OFF  */
    OFFSET = 755,                  /* OFFSET  */
    OIDS = 756,                    /* OIDS  */
    ON = 757,                      /* ON  */
    ONLY = 758,                    /* ONLY  */
    OPERATOR = 759,                /* OPERATOR  */
    OPTIMIZATION = 760,            /* OPTIMIZATION  */
    OPTION = 761,                  /* OPTION  */
    OPTIONALLY = 762,              /* OPTIONALLY  */
    OPTIONS = 763,                 /* OPTIONS  */
    OR = 764,                      /* OR  */
    ORDER = 765,                   /* ORDER  */
    OUT_P = 766,                   /* OUT_P  */
    OUTER_P = 767,                 /* OUTER_P  */
    OVER = 768,                    /* OVER  */
    OVERLAPS = 769,                /* OVERLAPS  */
    OVERLAY = 770,                 /* OVERLAY  */
    OWNED = 771,                   /* OWNED  */
    OWNER = 772,                   /* OWNER  */
    OUTFILE = 773,                 /* OUTFILE  */
    PACKAGE = 774,                 /* PACKAGE  */
    PACKAGES = 775,                /* PACKAGES  */
    PARSER = 776,                  /* PARSER  */
    PARTIAL = 777,                 /* PARTIAL  */
    PARTITION = 778,               /* PARTITION  */
    PARTITIONS = 779,              /* PARTITIONS  */
    PASSING = 780,                 /* PASSING  */
    PASSWORD = 781,                /* PASSWORD  */
    PCTFREE = 782,                 /* PCTFREE  */
    PER_P = 783,                   /* PER_P  */
    PERCENT = 784,                 /* PERCENT  */
    PERFORMANCE = 785,             /* PERFORMANCE  */
    PERM = 786,                    /* PERM  */
    PLACING = 787,                 /* PLACING  */
    PLAN = 788,                    /* PLAN  */
    PLANS = 789,                   /* PLANS  */
    POLICY = 790,                  /* POLICY  */
    POSITION = 791,                /* POSITION  */
    POOL = 792,                    /* POOL  */
    PRECEDING = 793,               /* PRECEDING  */
    PRECISION = 794,               /* PRECISION  */
    PREDICT = 795,                 /* PREDICT  */
    PREFERRED = 796,               /* PREFERRED  */
    PREFIX = 797,                  /* PREFIX  */
    PRESERVE = 798,                /* PRESERVE  */
    PREPARE = 799,                 /* PREPARE  */
    PREPARED = 800,                /* PREPARED  */
    PRIMARY = 801,                 /* PRIMARY  */
    PRECEDES_P = 802,              /* PRECEDES_P  */
    PRIVATE = 803,                 /* PRIVATE  */
    PRIOR = 804,                   /* PRIOR  */
    PRIORER = 805,                 /* PRIORER  */
    PRIVILEGES = 806,              /* PRIVILEGES  */
    PRIVILEGE = 807,               /* PRIVILEGE  */
    PROCEDURAL = 808,              /* PROCEDURAL  */
    PROCEDURE = 809,               /* PROCEDURE  */
    PROFILE = 810,                 /* PROFILE  */
    PUBLICATION = 811,             /* PUBLICATION  */
    PUBLISH = 812,                 /* PUBLISH  */
    PURGE = 813,                   /* PURGE  */
    QUERY = 814,                   /* QUERY  */
    QUOTE = 815,                   /* QUOTE  */
    RANDOMIZED = 816,              /* RANDOMIZED  */
    RANGE = 817,                   /* RANGE  */
    RATIO = 818,                   /* RATIO  */
    RAW = 819,                     /* RAW  */
    READ = 820,                    /* READ  */
    REAL = 821,                    /* REAL  */
    REASSIGN = 822,                /* REASSIGN  */
    REBUILD = 823,                 /* REBUILD  */
    RECHECK = 824,                 /* RECHECK  */
    RECURSIVE = 825,               /* RECURSIVE  */
    RECYCLEBIN = 826,              /* RECYCLEBIN  */
    REDISANYVALUE = 827,           /* REDISANYVALUE  */
    REF = 828,                     /* REF  */
    REFERENCES = 829,              /* REFERENCES  */
    REFRESH = 830,                 /* REFRESH  */
    REINDEX = 831,                 /* REINDEX  */
    REJECT_P = 832,                /* REJECT_P  */
    RELATIVE_P = 833,              /* RELATIVE_P  */
    RELEASE = 834,                 /* RELEASE  */
    RELOPTIONS = 835,              /* RELOPTIONS  */
    REMOTE_P = 836,                /* REMOTE_P  */
    REMOVE = 837,                  /* REMOVE  */
    RENAME = 838,                  /* RENAME  */
    REPEAT = 839,                  /* REPEAT  */
    REPEATABLE = 840,              /* REPEATABLE  */
    REPLACE = 841,                 /* REPLACE  */
    REPLICA = 842,                 /* REPLICA  */
    RESET = 843,                   /* RESET  */
    RESIZE = 844,                  /* RESIZE  */
    RESOURCE = 845,                /* RESOURCE  */
    RESTART = 846,                 /* RESTART  */
    RESTRICT = 847,                /* RESTRICT  */
    RETURN = 848,                  /* RETURN  */
    RETURNED_SQLSTATE = 849,       /* RETURNED_SQLSTATE  */
    RETURNING = 850,               /* RETURNING  */
    RETURNS = 851,                 /* RETURNS  */
    REUSE = 852,                   /* REUSE  */
    REVOKE = 853,                  /* REVOKE  */
    RIGHT = 854,                   /* RIGHT  */
    ROLE = 855,                    /* ROLE  */
    ROLES = 856,                   /* ROLES  */
    ROLLBACK = 857,                /* ROLLBACK  */
    ROLLUP = 858,                  /* ROLLUP  */
    ROTATION = 859,                /* ROTATION  */
    ROW = 860,                     /* ROW  */
    ROW_COUNT = 861,               /* ROW_COUNT  */
    ROWNUM = 862,                  /* ROWNUM  */
    ROWS = 863,                    /* ROWS  */
    ROWTYPE_P = 864,               /* ROWTYPE_P  */
    RULE = 865,                    /* RULE  */
    SAMPLE = 866,                  /* SAMPLE  */
    SAVEPOINT = 867,               /* SAVEPOINT  */
    SCHEDULE = 868,                /* SCHEDULE  */
    SCHEMA = 869,                  /* SCHEMA  */
    SCHEMA_NAME = 870,             /* SCHEMA_NAME  */
    SCROLL = 871,                  /* SCROLL  */
    SEARCH = 872,                  /* SEARCH  */
    SECOND_P = 873,                /* SECOND_P  */
    SECURITY = 874,                /* SECURITY  */
    SELECT = 875,                  /* SELECT  */
    SEPARATOR_P = 876,             /* SEPARATOR_P  */
    SEQUENCE = 877,                /* SEQUENCE  */
    SEQUENCES = 878,               /* SEQUENCES  */
    SERIALIZABLE = 879,            /* SERIALIZABLE  */
    SERVER = 880,                  /* SERVER  */
    SESSION = 881,                 /* SESSION  */
    SESSION_USER = 882,            /* SESSION_USER  */
    SET = 883,                     /* SET  */
    SETS = 884,                    /* SETS  */
    SETOF = 885,                   /* SETOF  */
    SHARE = 886,                   /* SHARE  */
    SHIPPABLE = 887,               /* SHIPPABLE  */
    SHOW = 888,                    /* SHOW  */
    SHUTDOWN = 889,                /* SHUTDOWN  */
    SIBLINGS = 890,                /* SIBLINGS  */
    SIMILAR = 891,                 /* SIMILAR  */
    SIMPLE = 892,                  /* SIMPLE  */
    SIZE = 893,                    /* SIZE  */
    SKIP = 894,                    /* SKIP  */
    SLAVE = 895,                   /* SLAVE  */
    SLICE = 896,                   /* SLICE  */
    SMALLDATETIME = 897,           /* SMALLDATETIME  */
    SMALLDATETIME_FORMAT_P = 898,  /* SMALLDATETIME_FORMAT_P  */
    SMALLINT = 899,                /* SMALLINT  */
    SNAPSHOT = 900,                /* SNAPSHOT  */
    SOME = 901,                    /* SOME  */
    SOURCE_P = 902,                /* SOURCE_P  */
    SPACE = 903,                   /* SPACE  */
    SPILL = 904,                   /* SPILL  */
    SPLIT = 905,                   /* SPLIT  */
    STABLE = 906,                  /* STABLE  */
    STACKED_P = 907,               /* STACKED_P  */
    STANDALONE_P = 908,            /* STANDALONE_P  */
    START = 909,                   /* START  */
    STARTS = 910,                  /* STARTS  */
    STARTWITH = 911,               /* STARTWITH  */
    STATEMENT = 912,               /* STATEMENT  */
    STATEMENT_ID = 913,            /* STATEMENT_ID  */
    STATISTICS = 914,              /* STATISTICS  */
    STDIN = 915,                   /* STDIN  */
    STDOUT = 916,                  /* STDOUT  */
    STORAGE = 917,                 /* STORAGE  */
    STORE_P = 918,                 /* STORE_P  */
    STORED = 919,                  /* STORED  */
    STRATIFY = 920,                /* STRATIFY  */
    STREAM = 921,                  /* STREAM  */
    STRICT_P = 922,                /* STRICT_P  */
    STRIP_P = 923,                 /* STRIP_P  */
    SUBCLASS_ORIGIN = 924,         /* SUBCLASS_ORIGIN  */
    SUBPARTITION = 925,            /* SUBPARTITION  */
    SUBPARTITIONS = 926,           /* SUBPARTITIONS  */
    SUBSCRIPTION = 927,            /* SUBSCRIPTION  */
    SUBSTRING = 928,               /* SUBSTRING  */
    SYMMETRIC = 929,               /* SYMMETRIC  */
    SYNONYM = 930,                 /* SYNONYM  */
    SYSDATE = 931,                 /* SYSDATE  */
    SYSID = 932,                   /* SYSID  */
    SYSTEM_P = 933,                /* SYSTEM_P  */
    SYS_REFCURSOR = 934,           /* SYS_REFCURSOR  */
    STARTING = 935,                /* STARTING  */
    SQL_P = 936,                   /* SQL_P  */
    TABLE = 937,                   /* TABLE  */
    TABLE_NAME = 938,              /* TABLE_NAME  */
    TABLES = 939,                  /* TABLES  */
    TABLESAMPLE = 940,             /* TABLESAMPLE  */
    TABLESPACE = 941,              /* TABLESPACE  */
    TARGET = 942,                  /* TARGET  */
    TEMP = 943,                    /* TEMP  */
    TEMPLATE = 944,                /* TEMPLATE  */
    TEMPORARY = 945,               /* TEMPORARY  */
    TERMINATED = 946,              /* TERMINATED  */
    TEXT_P = 947,                  /* TEXT_P  */
    THAN = 948,                    /* THAN  */
    THEN = 949,                    /* THEN  */
    TIME = 950,                    /* TIME  */
    TIME_FORMAT_P = 951,           /* TIME_FORMAT_P  */
    TIMECAPSULE = 952,             /* TIMECAPSULE  */
    TIMESTAMP = 953,               /* TIMESTAMP  */
    TIMESTAMP_FORMAT_P = 954,      /* TIMESTAMP_FORMAT_P  */
    TIMESTAMPDIFF = 955,           /* TIMESTAMPDIFF  */
    TINYINT = 956,                 /* TINYINT  */
    TO = 957,                      /* TO  */
    TRAILING = 958,                /* TRAILING  */
    TRANSACTION = 959,             /* TRANSACTION  */
    TRANSFORM = 960,               /* TRANSFORM  */
    TREAT = 961,                   /* TREAT  */
    TRIGGER = 962,                 /* TRIGGER  */
    TRIM = 963,                    /* TRIM  */
    TRUE_P = 964,                  /* TRUE_P  */
    TRUNCATE = 965,                /* TRUNCATE  */
    TRUSTED = 966,                 /* TRUSTED  */
    TSFIELD = 967,                 /* TSFIELD  */
    TSTAG = 968,                   /* TSTAG  */
    TSTIME = 969,                  /* TSTIME  */
    TYPE_P = 970,                  /* TYPE_P  */
    TYPES_P = 971,                 /* TYPES_P  */
    UNBOUNDED = 972,               /* UNBOUNDED  */
    UNCOMMITTED = 973,             /* UNCOMMITTED  */
    UNENCRYPTED = 974,             /* UNENCRYPTED  */
    UNION = 975,                   /* UNION  */
    UNIQUE = 976,                  /* UNIQUE  */
    UNKNOWN = 977,                 /* UNKNOWN  */
    UNLIMITED = 978,               /* UNLIMITED  */
    UNLISTEN = 979,                /* UNLISTEN  */
    UNLOCK = 980,                  /* UNLOCK  */
    UNLOGGED = 981,                /* UNLOGGED  */
    UNTIL = 982,                   /* UNTIL  */
    UNUSABLE = 983,                /* UNUSABLE  */
    UPDATE = 984,                  /* UPDATE  */
    USEEOF = 985,                  /* USEEOF  */
    USER = 986,                    /* USER  */
    USING = 987,                   /* USING  */
    VACUUM = 988,                  /* VACUUM  */
    VALID = 989,                   /* VALID  */
    VALIDATE = 990,                /* VALIDATE  */
    VALIDATION = 991,              /* VALIDATION  */
    VALIDATOR = 992,               /* VALIDATOR  */
    VALUE_P = 993,                 /* VALUE_P  */
    VALUES = 994,                  /* VALUES  */
    VARCHAR = 995,                 /* VARCHAR  */
    VARCHAR2 = 996,                /* VARCHAR2  */
    VARIABLES = 997,               /* VARIABLES  */
    VARIADIC = 998,                /* VARIADIC  */
    VARRAY = 999,                  /* VARRAY  */
    VARYING = 1000,                /* VARYING  */
    VCGROUP = 1001,                /* VCGROUP  */
    VERBOSE = 1002,                /* VERBOSE  */
    VERIFY = 1003,                 /* VERIFY  */
    VERSION_P = 1004,              /* VERSION_P  */
    VIEW = 1005,                   /* VIEW  */
    VISIBLE = 1006,                /* VISIBLE  */
    VOLATILE = 1007,               /* VOLATILE  */
    WAIT = 1008,                   /* WAIT  */
    WARNINGS = 1009,               /* WARNINGS  */
    WEAK = 1010,                   /* WEAK  */
    WHEN = 1011,                   /* WHEN  */
    WHERE = 1012,                  /* WHERE  */
    WHILE_P = 1013,                /* WHILE_P  */
    WHITESPACE_P = 1014,           /* WHITESPACE_P  */
    WINDOW = 1015,                 /* WINDOW  */
    WITH = 1016,                   /* WITH  */
    WITHIN = 1017,                 /* WITHIN  */
    WITHOUT = 1018,                /* WITHOUT  */
    WORK = 1019,                   /* WORK  */
    WORKLOAD = 1020,               /* WORKLOAD  */
    WRAPPER = 1021,                /* WRAPPER  */
    WRITE = 1022,                  /* WRITE  */
    XML_P = 1023,                  /* XML_P  */
    XMLATTRIBUTES = 1024,          /* XMLATTRIBUTES  */
    XMLCONCAT = 1025,              /* XMLCONCAT  */
    XMLELEMENT = 1026,             /* XMLELEMENT  */
    XMLEXISTS = 1027,              /* XMLEXISTS  */
    XMLFOREST = 1028,              /* XMLFOREST  */
    XMLPARSE = 1029,               /* XMLPARSE  */
    XMLPI = 1030,                  /* XMLPI  */
    XMLROOT = 1031,                /* XMLROOT  */
    XMLSERIALIZE = 1032,           /* XMLSERIALIZE  */
    YEAR_P = 1033,                 /* YEAR_P  */
    YEAR_MONTH_P = 1034,           /* YEAR_MONTH_P  */
    YES_P = 1035,                  /* YES_P  */
    ZONE = 1036,                   /* ZONE  */
    NULLS_FIRST = 1037,            /* NULLS_FIRST  */
    NULLS_LAST = 1038,             /* NULLS_LAST  */
    WITH_TIME = 1039,              /* WITH_TIME  */
    INCLUDING_ALL = 1040,          /* INCLUDING_ALL  */
    RENAME_PARTITION = 1041,       /* RENAME_PARTITION  */
    PARTITION_FOR = 1042,          /* PARTITION_FOR  */
    SUBPARTITION_FOR = 1043,       /* SUBPARTITION_FOR  */
    ADD_PARTITION = 1044,          /* ADD_PARTITION  */
    DROP_PARTITION = 1045,         /* DROP_PARTITION  */
    REBUILD_PARTITION = 1046,      /* REBUILD_PARTITION  */
    MODIFY_PARTITION = 1047,       /* MODIFY_PARTITION  */
    ADD_SUBPARTITION = 1048,       /* ADD_SUBPARTITION  */
    DROP_SUBPARTITION = 1049,      /* DROP_SUBPARTITION  */
    NOT_ENFORCED = 1050,           /* NOT_ENFORCED  */
    VALID_BEGIN = 1051,            /* VALID_BEGIN  */
    DECLARE_CURSOR = 1052,         /* DECLARE_CURSOR  */
    ON_UPDATE_TIME = 1053,         /* ON_UPDATE_TIME  */
    START_WITH = 1054,             /* START_WITH  */
    CONNECT_BY = 1055,             /* CONNECT_BY  */
    SHOW_ERRORS = 1056,            /* SHOW_ERRORS  */
    END_OF_INPUT = 1057,           /* END_OF_INPUT  */
    END_OF_INPUT_COLON = 1058,     /* END_OF_INPUT_COLON  */
    END_OF_PROC = 1059,            /* END_OF_PROC  */
    EVENT_TRIGGER = 1060,          /* EVENT_TRIGGER  */
    NOT_IN = 1061,                 /* NOT_IN  */
    NOT_BETWEEN = 1062,            /* NOT_BETWEEN  */
    NOT_LIKE = 1063,               /* NOT_LIKE  */
    NOT_ILIKE = 1064,              /* NOT_ILIKE  */
    NOT_SIMILAR = 1065,            /* NOT_SIMILAR  */
    FORCE_INDEX = 1066,            /* FORCE_INDEX  */
    USE_INDEX = 1067,              /* USE_INDEX  */
    IGNORE_INDEX = 1068,           /* IGNORE_INDEX  */
    PARTIAL_EMPTY_PREC = 1069,     /* PARTIAL_EMPTY_PREC  */
    POSTFIXOP = 1070,              /* POSTFIXOP  */
    lower_than_index = 1071,       /* lower_than_index  */
    UMINUS = 1072,                 /* UMINUS  */
    EMPTY_FROM_CLAUSE = 1073       /* EMPTY_FROM_CLAUSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 564 "preproc.y"

	double	dval;
	char	*str;
	int		ival;
	struct	when		action;
	struct	index		index;
	int		tagname;
	struct	this_type	type;
	enum	ECPGttype	type_enum;
	enum	ECPGdtype	dtype_enum;
	struct	fetch_desc	descriptor;
	struct  su_symbol	struct_union;
	struct	prep		prep;

#line 897 "preproc.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE base_yylval;
extern YYLTYPE base_yylloc;

int base_yyparse (void);


#endif /* !YY_BASE_YY_PREPROC_HPP_INCLUDED  */
