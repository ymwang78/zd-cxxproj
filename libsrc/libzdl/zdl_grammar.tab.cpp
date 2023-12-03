/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 9 "zdl_grammar.y"


/* ***************************************************************
 *  version:  1.0   date: 01/03/2007
 *  -------------------------------------------------------------
 *  Yongming Wang(wangym@gmail.com)
 *  -------------------------------------------------------------
 *  Copyright (C) 2007 - All Rights Reserved
 * ***************************************************************
 * 
 * ***************************************************************/
#include <zce/zce_config.h>
#include <zce/zce_api.h>
#include <string>
#include "zdl/zdl_utils.h"
#include "zdl/zdl_parser.h"
#include "zdl/zdl_parser_context.h"

#define YYINITDEPTH 256
#define YYSTYPE std::string

class zdl_parser;

typedef void* yyscan_t;

void yyerror (yyscan_t yyscanner, zdl_parser* parser, char const* s)
{
    parser->error(s);
};
extern int yylex (YYSTYPE* yylvalp, yyscan_t yyscanner, zdl_parser* parser);

#line 103 "zdl_grammar.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "zdl_grammar.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_UIDL_CHAR = 3,                  /* UIDL_CHAR  */
  YYSYMBOL_UIDL_UCHAR = 4,                 /* UIDL_UCHAR  */
  YYSYMBOL_UIDL_SHORT = 5,                 /* UIDL_SHORT  */
  YYSYMBOL_UIDL_USHORT = 6,                /* UIDL_USHORT  */
  YYSYMBOL_UIDL_INT32 = 7,                 /* UIDL_INT32  */
  YYSYMBOL_UIDL_UINT32 = 8,                /* UIDL_UINT32  */
  YYSYMBOL_UIDL_INT64 = 9,                 /* UIDL_INT64  */
  YYSYMBOL_UIDL_UINT64 = 10,               /* UIDL_UINT64  */
  YYSYMBOL_UIDL_FLOAT = 11,                /* UIDL_FLOAT  */
  YYSYMBOL_UIDL_DOUBLE = 12,               /* UIDL_DOUBLE  */
  YYSYMBOL_UIDL_ASTRING = 13,              /* UIDL_ASTRING  */
  YYSYMBOL_UIDL_TSTRING = 14,              /* UIDL_TSTRING  */
  YYSYMBOL_UIDL_WSTRING = 15,              /* UIDL_WSTRING  */
  YYSYMBOL_UIDL_SERIAL = 16,               /* UIDL_SERIAL  */
  YYSYMBOL_UIDL_DATETIME = 17,             /* UIDL_DATETIME  */
  YYSYMBOL_UIDL_ASTRVEC = 18,              /* UIDL_ASTRVEC  */
  YYSYMBOL_UIDL_TSTRVEC = 19,              /* UIDL_TSTRVEC  */
  YYSYMBOL_UIDL_WSTRVEC = 20,              /* UIDL_WSTRVEC  */
  YYSYMBOL_UIDL_ENUM = 21,                 /* UIDL_ENUM  */
  YYSYMBOL_UIDL_STRUCT = 22,               /* UIDL_STRUCT  */
  YYSYMBOL_UIDL_NAMESPACE = 23,            /* UIDL_NAMESPACE  */
  YYSYMBOL_UIDL_IDENTIFIER = 24,           /* UIDL_IDENTIFIER  */
  YYSYMBOL_UIDL_INTEGER_LITERAL = 25,      /* UIDL_INTEGER_LITERAL  */
  YYSYMBOL_UIDL_STRING_LITERAL = 26,       /* UIDL_STRING_LITERAL  */
  YYSYMBOL_UIDL_COMMENT = 27,              /* UIDL_COMMENT  */
  YYSYMBOL_UIDL_USER_START = 28,           /* UIDL_USER_START  */
  YYSYMBOL_29_ = 29,                       /* '{'  */
  YYSYMBOL_30_ = 30,                       /* '}'  */
  YYSYMBOL_31_ = 31,                       /* ';'  */
  YYSYMBOL_32_ = 32,                       /* ','  */
  YYSYMBOL_33_ = 33,                       /* '['  */
  YYSYMBOL_34_ = 34,                       /* ']'  */
  YYSYMBOL_35_ = 35,                       /* '<'  */
  YYSYMBOL_36_ = 36,                       /* '>'  */
  YYSYMBOL_37_ = 37,                       /* '*'  */
  YYSYMBOL_38_ = 38,                       /* '~'  */
  YYSYMBOL_39_ = 39,                       /* '='  */
  YYSYMBOL_40_ = 40,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_start = 42,                     /* start  */
  YYSYMBOL_comments = 43,                  /* comments  */
  YYSYMBOL_namespace_def = 44,             /* namespace_def  */
  YYSYMBOL_45_1 = 45,                      /* $@1  */
  YYSYMBOL_definitions = 46,               /* definitions  */
  YYSYMBOL_definition = 47,                /* definition  */
  YYSYMBOL_meta_key = 48,                  /* meta_key  */
  YYSYMBOL_meta_val = 49,                  /* meta_val  */
  YYSYMBOL_meta_data = 50,                 /* meta_data  */
  YYSYMBOL_meta_data_set = 51,             /* meta_data_set  */
  YYSYMBOL_num = 52,                       /* num  */
  YYSYMBOL_limit_arg = 53,                 /* limit_arg  */
  YYSYMBOL_template_args = 54,             /* template_args  */
  YYSYMBOL_template = 55,                  /* template  */
  YYSYMBOL_type_id = 56,                   /* type_id  */
  YYSYMBOL_struct_id = 57,                 /* struct_id  */
  YYSYMBOL_struct_def = 58,                /* struct_def  */
  YYSYMBOL_59_2 = 59,                      /* $@2  */
  YYSYMBOL_struct_exports = 60,            /* struct_exports  */
  YYSYMBOL_struct_export = 61,             /* struct_export  */
  YYSYMBOL_data_member = 62,               /* data_member  */
  YYSYMBOL_enum_id = 63,                   /* enum_id  */
  YYSYMBOL_enum_def = 64,                  /* enum_def  */
  YYSYMBOL_65_3 = 65,                      /* $@3  */
  YYSYMBOL_enumerator_list = 66,           /* enumerator_list  */
  YYSYMBOL_enumerator = 67,                /* enumerator  */
  YYSYMBOL_type = 68,                      /* type  */
  YYSYMBOL_basetype = 69,                  /* basetype  */
  YYSYMBOL_usertype = 70,                  /* usertype  */
  YYSYMBOL_keyword = 71                    /* keyword  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   171

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  117

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   283


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    37,     2,    32,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    40,    31,
      35,    39,    36,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,    34,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,     2,    30,    38,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    81,    81,    88,   100,   108,   107,   115,   121,   124,
     128,   133,   139,   143,   150,   156,   160,   165,   170,   178,
     182,   186,   191,   197,   206,   214,   218,   225,   230,   238,
     243,   254,   261,   271,   278,   287,   286,   298,   302,   307,
     313,   323,   327,   331,   339,   344,   353,   352,   362,   365,
     369,   375,   380,   384,   393,   396,   401,   405,   409,   413,
     417,   421,   425,   429,   433,   437,   441,   445,   449,   453,
     457,   461,   465,   469,   476,   480,   484,   490,   491,   492
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "UIDL_CHAR",
  "UIDL_UCHAR", "UIDL_SHORT", "UIDL_USHORT", "UIDL_INT32", "UIDL_UINT32",
  "UIDL_INT64", "UIDL_UINT64", "UIDL_FLOAT", "UIDL_DOUBLE", "UIDL_ASTRING",
  "UIDL_TSTRING", "UIDL_WSTRING", "UIDL_SERIAL", "UIDL_DATETIME",
  "UIDL_ASTRVEC", "UIDL_TSTRVEC", "UIDL_WSTRVEC", "UIDL_ENUM",
  "UIDL_STRUCT", "UIDL_NAMESPACE", "UIDL_IDENTIFIER",
  "UIDL_INTEGER_LITERAL", "UIDL_STRING_LITERAL", "UIDL_COMMENT",
  "UIDL_USER_START", "'{'", "'}'", "';'", "','", "'['", "']'", "'<'",
  "'>'", "'*'", "'~'", "'='", "':'", "$accept", "start", "comments",
  "namespace_def", "$@1", "definitions", "definition", "meta_key",
  "meta_val", "meta_data", "meta_data_set", "num", "limit_arg",
  "template_args", "template", "type_id", "struct_id", "struct_def", "$@2",
  "struct_exports", "struct_export", "data_member", "enum_id", "enum_def",
  "$@3", "enumerator_list", "enumerator", "type", "basetype", "usertype",
  "keyword", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-102)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-40)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -8,   -13,    17,  -102,  -102,  -102,    -1,     4,    -4,    81,
       5,     0,     1,     2,     9,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,    12,    14,
    -102,  -102,  -102,  -102,  -102,  -102,     7,  -102,     4,  -102,
     103,    13,    15,  -102,   -28,    -5,    11,  -102,    34,     6,
     -14,   125,    -2,    31,  -102,  -102,  -102,  -102,     6,     6,
    -102,   147,    16,    41,   -21,    43,    42,  -102,  -102,  -102,
    -102,    44,    39,    23,  -102,  -102,    12,    14,  -102,     6,
      46,  -102,   -14,    52,  -102,  -102,   125,    -2,  -102,    34,
    -102,    45,    55,  -102,  -102,     6,  -102,  -102,  -102,    -2,
    -102,  -102,   -24,  -102,    -2,    48,  -102
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       7,     0,     0,     2,     5,     1,     0,     0,     0,     0,
       0,     0,     9,    20,     0,    35,    12,    46,    13,    10,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    72,    73,    69,    70,    71,    78,    79,
      44,    77,    54,    55,    45,    14,    17,     6,     0,    19,
       0,     0,     0,    76,    28,     0,     0,     8,    28,     4,
      20,    50,    22,     0,    74,    16,    15,    18,     4,     4,
      34,     0,     0,    38,     4,     0,    49,    53,    24,    21,
      23,    26,     0,     0,    33,     3,     0,     0,    41,     4,
      43,    36,    20,     0,    52,    47,    50,    22,    27,    28,
      40,    29,     0,    42,    37,     4,    48,    25,    75,    22,
      30,    51,     0,    32,    22,     0,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -102,  -102,   -65,  -102,  -102,    56,  -102,  -102,  -102,  -102,
     -12,  -102,  -101,    29,   -56,  -102,  -102,  -102,  -102,    78,
    -102,  -102,  -102,  -102,  -102,    75,  -102,    77,  -102,  -102,
      -9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    70,     3,     6,    11,    12,    46,    56,    13,
      14,    80,    81,    82,    64,    88,    15,    16,    51,    72,
      73,    89,    17,    18,    52,    75,    76,    41,    42,    43,
      77
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      44,    49,    68,    84,    85,     8,    69,    62,   112,    94,
     113,     4,    63,   115,   114,     1,   -39,     5,    93,    10,
      65,    66,    78,    79,   100,     9,   -20,    19,     7,    45,
      47,    50,    48,    69,   -11,    10,    53,    10,    54,    55,
     111,    59,    60,   108,    61,    67,    91,    99,    71,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    62,
     101,    83,    92,    95,    96,    98,    97,   105,   109,   110,
      71,   103,   116,   102,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    57,    40,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   107,    58,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    90,    74,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    86,    87,
     104,   106
};

static const yytype_int8 yycheck[] =
{
       9,    13,    58,    68,    69,     1,    27,    35,   109,    74,
      34,    24,    40,   114,    38,    23,    30,     0,    39,    33,
      25,    26,    24,    25,    89,    21,    22,    31,    29,    24,
      30,    22,    31,    27,    30,    33,    24,    33,    24,    32,
     105,    50,    29,    99,    29,    34,    30,    24,    60,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    35,
      24,    40,    31,    30,    32,    36,    32,    25,    33,    24,
      92,    90,    34,    37,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    48,    24,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    97,    24,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    71,    24,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      92,    96
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    23,    42,    44,    24,     0,    45,    29,     1,    21,
      33,    46,    47,    50,    51,    57,    58,    63,    64,    31,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      24,    68,    69,    70,    71,    24,    48,    30,    31,    51,
      22,    59,    65,    24,    24,    32,    49,    46,    24,    71,
      29,    29,    35,    40,    55,    25,    26,    34,    55,    27,
      43,    51,    60,    61,    24,    66,    67,    71,    24,    25,
      52,    53,    54,    40,    43,    43,    21,    22,    56,    62,
      68,    30,    31,    39,    43,    30,    32,    32,    36,    24,
      43,    24,    37,    71,    60,    25,    66,    54,    55,    33,
      24,    43,    53,    34,    38,    53,    34
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    45,    44,    44,    46,    46,
      46,    46,    47,    47,    48,    49,    49,    49,    50,    51,
      51,    52,    52,    53,    53,    54,    54,    55,    55,    56,
      56,    56,    56,    57,    57,    59,    58,    60,    60,    60,
      61,    62,    62,    62,    63,    63,    65,    64,    66,    66,
      66,    67,    67,    67,    68,    68,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    70,    70,    70,    71,    71,    71
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     0,     6,     0,     3,     1,
       2,     0,     1,     1,     1,     2,     2,     0,     4,     2,
       0,     1,     0,     1,     1,     3,     1,     3,     0,     2,
       3,     7,     5,     5,     4,     0,     5,     3,     1,     0,
       3,     1,     2,     1,     2,     2,     0,     5,     3,     1,
       0,     4,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     6,     2,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (scanner, parser, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, scanner, parser); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, yyscan_t scanner, zdl_parser* parser)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (scanner);
  YY_USE (parser);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, yyscan_t scanner, zdl_parser* parser)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, scanner, parser);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, yyscan_t scanner, zdl_parser* parser)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], scanner, parser);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner, parser); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, yyscan_t scanner, zdl_parser* parser)
{
  YY_USE (yyvaluep);
  YY_USE (scanner);
  YY_USE (parser);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner, zdl_parser* parser)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, scanner, parser);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: namespace_def  */
#line 82 "zdl_grammar.y"
{
}
#line 1268 "zdl_grammar.tab.cpp"
    break;

  case 3: /* comments: UIDL_COMMENT comments  */
#line 89 "zdl_grammar.y"
{
    if (yyvsp[0] != "")
    {
        yyval = yyvsp[-1] + "\n" + yyvsp[0];
    }
    else
    {
        yyval = yyvsp[-1];
    }	
}
#line 1283 "zdl_grammar.tab.cpp"
    break;

  case 4: /* comments: %empty  */
#line 100 "zdl_grammar.y"
{
    yyval = "";
}
#line 1291 "zdl_grammar.tab.cpp"
    break;

  case 5: /* $@1: %empty  */
#line 108 "zdl_grammar.y"
{
    parser->context_ptr()->set_namespace(yyvsp[0]);
}
#line 1299 "zdl_grammar.tab.cpp"
    break;

  case 6: /* namespace_def: UIDL_NAMESPACE UIDL_IDENTIFIER $@1 '{' definitions '}'  */
#line 112 "zdl_grammar.y"
{
}
#line 1306 "zdl_grammar.tab.cpp"
    break;

  case 7: /* namespace_def: %empty  */
#line 115 "zdl_grammar.y"
{
}
#line 1313 "zdl_grammar.tab.cpp"
    break;

  case 8: /* definitions: definition ';' definitions  */
#line 122 "zdl_grammar.y"
{
}
#line 1320 "zdl_grammar.tab.cpp"
    break;

  case 9: /* definitions: definition  */
#line 125 "zdl_grammar.y"
{
    yyerror(scanner, parser, "';' missing after definition");
}
#line 1328 "zdl_grammar.tab.cpp"
    break;

  case 10: /* definitions: error ';'  */
#line 129 "zdl_grammar.y"
{
    yyerror(scanner, parser, "error got");
}
#line 1336 "zdl_grammar.tab.cpp"
    break;

  case 11: /* definitions: %empty  */
#line 133 "zdl_grammar.y"
{
}
#line 1343 "zdl_grammar.tab.cpp"
    break;

  case 12: /* definition: struct_def  */
#line 140 "zdl_grammar.y"
{
    parser->context_ptr()->add_type_end(yyvsp[0]);
}
#line 1351 "zdl_grammar.tab.cpp"
    break;

  case 13: /* definition: enum_def  */
#line 144 "zdl_grammar.y"
{
    parser->context_ptr()->add_type_end(yyvsp[0]);
}
#line 1359 "zdl_grammar.tab.cpp"
    break;

  case 14: /* meta_key: UIDL_IDENTIFIER  */
#line 151 "zdl_grammar.y"
{
  yyval = yyvsp[0];
}
#line 1367 "zdl_grammar.tab.cpp"
    break;

  case 15: /* meta_val: ',' UIDL_STRING_LITERAL  */
#line 157 "zdl_grammar.y"
{
  yyval = std::string("\"") + yyvsp[0] + "\"";
}
#line 1375 "zdl_grammar.tab.cpp"
    break;

  case 16: /* meta_val: ',' UIDL_INTEGER_LITERAL  */
#line 161 "zdl_grammar.y"
{
  yyval = yyvsp[0];
}
#line 1383 "zdl_grammar.tab.cpp"
    break;

  case 17: /* meta_val: %empty  */
#line 165 "zdl_grammar.y"
{
  yyval = "";
}
#line 1391 "zdl_grammar.tab.cpp"
    break;

  case 18: /* meta_data: '[' meta_key meta_val ']'  */
#line 171 "zdl_grammar.y"
{
    parser->context_ptr()->add_current_member_metadata(yyvsp[-2], yyvsp[-1]);
    yyval = yyvsp[-2];
}
#line 1400 "zdl_grammar.tab.cpp"
    break;

  case 19: /* meta_data_set: meta_data meta_data_set  */
#line 179 "zdl_grammar.y"
{
}
#line 1407 "zdl_grammar.tab.cpp"
    break;

  case 20: /* meta_data_set: %empty  */
#line 182 "zdl_grammar.y"
{
}
#line 1414 "zdl_grammar.tab.cpp"
    break;

  case 21: /* num: UIDL_INTEGER_LITERAL  */
#line 187 "zdl_grammar.y"
{
    yyval = yyvsp[0];
}
#line 1422 "zdl_grammar.tab.cpp"
    break;

  case 22: /* num: %empty  */
#line 191 "zdl_grammar.y"
{
    yyval = "0"; 
}
#line 1430 "zdl_grammar.tab.cpp"
    break;

  case 23: /* limit_arg: num  */
#line 198 "zdl_grammar.y"
{
    int val = strtol(yyvsp[0].c_str(), 0, 0);
    if (val < 0){
        yyerror(scanner, parser, "invalid neg array length");
    }
    yyval = yyvsp[0];
}
#line 1442 "zdl_grammar.tab.cpp"
    break;

  case 24: /* limit_arg: UIDL_IDENTIFIER  */
#line 207 "zdl_grammar.y"
{
    yyval = yyvsp[0] + "_";
}
#line 1450 "zdl_grammar.tab.cpp"
    break;

  case 25: /* template_args: limit_arg ',' template_args  */
#line 215 "zdl_grammar.y"
{
    yyval = yyvsp[-2] + "," + yyvsp[0];
}
#line 1458 "zdl_grammar.tab.cpp"
    break;

  case 26: /* template_args: limit_arg  */
#line 219 "zdl_grammar.y"
{
    yyval = yyvsp[0];
}
#line 1466 "zdl_grammar.tab.cpp"
    break;

  case 27: /* template: '<' template_args '>'  */
#line 226 "zdl_grammar.y"
{
    yyval = yyvsp[-1];
}
#line 1474 "zdl_grammar.tab.cpp"
    break;

  case 28: /* template: %empty  */
#line 230 "zdl_grammar.y"
{
    yyval = "";
}
#line 1482 "zdl_grammar.tab.cpp"
    break;

  case 29: /* type_id: type UIDL_IDENTIFIER  */
#line 239 "zdl_grammar.y"
{
    parser->context_ptr()->add_member_start(yyvsp[0], "1", "1");
    yyval = yyvsp[0];
}
#line 1491 "zdl_grammar.tab.cpp"
    break;

  case 30: /* type_id: type '*' UIDL_IDENTIFIER  */
#line 244 "zdl_grammar.y"
{
    int val = 0;
    if (val >=0){
        parser->context_ptr()->add_member_start(yyvsp[0], "0", "0");
    }
    else if (val<0){
        yyerror(scanner, parser, "invalid neg array length");
    }
    yyval = yyvsp[0];
}
#line 1506 "zdl_grammar.tab.cpp"
    break;

  case 31: /* type_id: type UIDL_IDENTIFIER '[' limit_arg '~' limit_arg ']'  */
#line 255 "zdl_grammar.y"
{
    //int low_val = strtol($4.c_str(), 0, 0);
    //int up_val = strtol($6.c_str(), 0, 0);
    parser->context_ptr()->add_member_start(yyvsp[-5], yyvsp[-3], yyvsp[-1]);
    yyval = yyvsp[-5];
}
#line 1517 "zdl_grammar.tab.cpp"
    break;

  case 32: /* type_id: type UIDL_IDENTIFIER '[' limit_arg ']'  */
#line 262 "zdl_grammar.y"
{
    //int val = strtol($4.c_str(), 0, 0);
    parser->context_ptr()->add_member_start(yyvsp[-3], yyvsp[-1], yyvsp[-1]);
    yyval = yyvsp[-3];
}
#line 1527 "zdl_grammar.tab.cpp"
    break;

  case 33: /* struct_id: meta_data_set UIDL_STRUCT UIDL_IDENTIFIER template comments  */
#line 272 "zdl_grammar.y"
{
    parser->context_ptr()->add_type_start(UIDL_STRUCT, yyvsp[-2]);
    parser->context_ptr()->set_comment(yyvsp[0]);
    parser->context_ptr()->add_type_template_arg(yyvsp[-1]);
    yyval = yyvsp[-2];
}
#line 1538 "zdl_grammar.tab.cpp"
    break;

  case 34: /* struct_id: meta_data_set UIDL_STRUCT keyword comments  */
#line 279 "zdl_grammar.y"
{
    yyerror(scanner, parser, "keyword cannot be used as struct name");
}
#line 1546 "zdl_grammar.tab.cpp"
    break;

  case 35: /* $@2: %empty  */
#line 287 "zdl_grammar.y"
{
    auto err = zce_string_format(256, "[dbg] parser struct %s\n", yyvsp[0].c_str());
    yyerror(scanner, parser, err.c_str());
}
#line 1555 "zdl_grammar.tab.cpp"
    break;

  case 36: /* struct_def: struct_id $@2 '{' struct_exports '}'  */
#line 292 "zdl_grammar.y"
{
}
#line 1562 "zdl_grammar.tab.cpp"
    break;

  case 37: /* struct_exports: struct_export ';' struct_exports  */
#line 299 "zdl_grammar.y"
{
    //printf("struct_exports :%s %s\n", $1.c_str(), $2.c_str());
}
#line 1570 "zdl_grammar.tab.cpp"
    break;

  case 38: /* struct_exports: struct_export  */
#line 303 "zdl_grammar.y"
{
    yyerror(scanner, parser, "';' missing after struct definition");
}
#line 1578 "zdl_grammar.tab.cpp"
    break;

  case 39: /* struct_exports: %empty  */
#line 307 "zdl_grammar.y"
{
}
#line 1585 "zdl_grammar.tab.cpp"
    break;

  case 40: /* struct_export: meta_data_set data_member comments  */
#line 314 "zdl_grammar.y"
{
    parser->context_ptr()->set_comment(yyvsp[0]);
    parser->context_ptr()->add_member_end(yyvsp[-1]);
    yyval=yyvsp[-1];
}
#line 1595 "zdl_grammar.tab.cpp"
    break;

  case 41: /* data_member: type_id  */
#line 324 "zdl_grammar.y"
{
    yyval=yyvsp[0];
}
#line 1603 "zdl_grammar.tab.cpp"
    break;

  case 42: /* data_member: type keyword  */
#line 328 "zdl_grammar.y"
{
    yyerror(scanner, parser, "keyword  cannot be used as data member name");
}
#line 1611 "zdl_grammar.tab.cpp"
    break;

  case 43: /* data_member: type  */
#line 332 "zdl_grammar.y"
{
    yyerror(scanner, parser, "missing data member name");
}
#line 1619 "zdl_grammar.tab.cpp"
    break;

  case 44: /* enum_id: UIDL_ENUM UIDL_IDENTIFIER  */
#line 340 "zdl_grammar.y"
{
    parser->context_ptr()->add_type_start(UIDL_ENUM, yyvsp[0]);
    yyval = yyvsp[0];
}
#line 1628 "zdl_grammar.tab.cpp"
    break;

  case 45: /* enum_id: UIDL_ENUM keyword  */
#line 345 "zdl_grammar.y"
{
    yyerror(scanner, parser, "keyword cannot be used as enumeration name");
}
#line 1636 "zdl_grammar.tab.cpp"
    break;

  case 46: /* $@3: %empty  */
#line 353 "zdl_grammar.y"
{
}
#line 1643 "zdl_grammar.tab.cpp"
    break;

  case 47: /* enum_def: enum_id $@3 '{' enumerator_list '}'  */
#line 356 "zdl_grammar.y"
{
}
#line 1650 "zdl_grammar.tab.cpp"
    break;

  case 48: /* enumerator_list: enumerator ',' enumerator_list  */
#line 363 "zdl_grammar.y"
{
}
#line 1657 "zdl_grammar.tab.cpp"
    break;

  case 49: /* enumerator_list: enumerator  */
#line 366 "zdl_grammar.y"
{
}
#line 1664 "zdl_grammar.tab.cpp"
    break;

  case 50: /* enumerator_list: %empty  */
#line 369 "zdl_grammar.y"
{
}
#line 1671 "zdl_grammar.tab.cpp"
    break;

  case 51: /* enumerator: UIDL_IDENTIFIER '=' UIDL_INTEGER_LITERAL comments  */
#line 376 "zdl_grammar.y"
{
    unsigned long val = strtoul(yyvsp[-1].c_str(), 0, 0);
    parser->context_ptr()->add_enumerator(val, yyvsp[-3], yyvsp[0]);
}
#line 1680 "zdl_grammar.tab.cpp"
    break;

  case 52: /* enumerator: UIDL_IDENTIFIER comments  */
#line 381 "zdl_grammar.y"
{
    parser->context_ptr()->add_enumerator(yyvsp[-1], yyvsp[0]);
}
#line 1688 "zdl_grammar.tab.cpp"
    break;

  case 53: /* enumerator: keyword  */
#line 385 "zdl_grammar.y"
{
    yyerror(scanner, parser, "keyword cannot be used as enumerator");
}
#line 1696 "zdl_grammar.tab.cpp"
    break;

  case 54: /* type: basetype  */
#line 394 "zdl_grammar.y"
{
}
#line 1703 "zdl_grammar.tab.cpp"
    break;

  case 55: /* type: usertype  */
#line 397 "zdl_grammar.y"
{
}
#line 1710 "zdl_grammar.tab.cpp"
    break;

  case 56: /* basetype: UIDL_CHAR  */
#line 402 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_CHAR);
}
#line 1718 "zdl_grammar.tab.cpp"
    break;

  case 57: /* basetype: UIDL_UCHAR  */
#line 406 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_UCHAR);
}
#line 1726 "zdl_grammar.tab.cpp"
    break;

  case 58: /* basetype: UIDL_SHORT  */
#line 410 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_SHORT);
}
#line 1734 "zdl_grammar.tab.cpp"
    break;

  case 59: /* basetype: UIDL_USHORT  */
#line 414 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_USHORT);
}
#line 1742 "zdl_grammar.tab.cpp"
    break;

  case 60: /* basetype: UIDL_INT32  */
#line 418 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_INT32);
}
#line 1750 "zdl_grammar.tab.cpp"
    break;

  case 61: /* basetype: UIDL_UINT32  */
#line 422 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_UINT32);
}
#line 1758 "zdl_grammar.tab.cpp"
    break;

  case 62: /* basetype: UIDL_INT64  */
#line 426 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_INT64);
}
#line 1766 "zdl_grammar.tab.cpp"
    break;

  case 63: /* basetype: UIDL_UINT64  */
#line 430 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_UINT64);
}
#line 1774 "zdl_grammar.tab.cpp"
    break;

  case 64: /* basetype: UIDL_FLOAT  */
#line 434 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_FLOAT);
}
#line 1782 "zdl_grammar.tab.cpp"
    break;

  case 65: /* basetype: UIDL_DOUBLE  */
#line 438 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_DOUBLE);
}
#line 1790 "zdl_grammar.tab.cpp"
    break;

  case 66: /* basetype: UIDL_ASTRING  */
#line 442 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_ASTRING);
}
#line 1798 "zdl_grammar.tab.cpp"
    break;

  case 67: /* basetype: UIDL_TSTRING  */
#line 446 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_TSTRING);
}
#line 1806 "zdl_grammar.tab.cpp"
    break;

  case 68: /* basetype: UIDL_WSTRING  */
#line 450 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_WSTRING);
}
#line 1814 "zdl_grammar.tab.cpp"
    break;

  case 69: /* basetype: UIDL_ASTRVEC  */
#line 454 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_ASTRVEC);
}
#line 1822 "zdl_grammar.tab.cpp"
    break;

  case 70: /* basetype: UIDL_TSTRVEC  */
#line 458 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_TSTRVEC);
}
#line 1830 "zdl_grammar.tab.cpp"
    break;

  case 71: /* basetype: UIDL_WSTRVEC  */
#line 462 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_WSTRVEC);
}
#line 1838 "zdl_grammar.tab.cpp"
    break;

  case 72: /* basetype: UIDL_SERIAL  */
#line 466 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_SERIAL);
}
#line 1846 "zdl_grammar.tab.cpp"
    break;

  case 73: /* basetype: UIDL_DATETIME  */
#line 470 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_DATETIME);
}
#line 1854 "zdl_grammar.tab.cpp"
    break;

  case 74: /* usertype: UIDL_STRUCT UIDL_IDENTIFIER template  */
#line 477 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_STRUCT, yyvsp[0], yyvsp[-1]);
}
#line 1862 "zdl_grammar.tab.cpp"
    break;

  case 75: /* usertype: UIDL_STRUCT UIDL_IDENTIFIER ':' ':' UIDL_IDENTIFIER template  */
#line 481 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_STRUCT, yyvsp[0], yyvsp[-1], yyvsp[-4]);
}
#line 1870 "zdl_grammar.tab.cpp"
    break;

  case 76: /* usertype: UIDL_ENUM UIDL_IDENTIFIER  */
#line 485 "zdl_grammar.y"
{
    parser->context_ptr()->set_current_member_type(UIDL_ENUM, "", yyvsp[0]);
}
#line 1878 "zdl_grammar.tab.cpp"
    break;


#line 1882 "zdl_grammar.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (scanner, parser, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner, parser);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, scanner, parser);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, parser, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner, parser);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, scanner, parser);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 495 "zdl_grammar.y"


