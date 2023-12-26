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

#ifndef YY_YY_ZDL_GRAMMAR_TAB_HPP_INCLUDED
# define YY_YY_ZDL_GRAMMAR_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "zdl_grammar.y"


class zdl_parser;

typedef void* yyscan_t;


#line 57 "zdl_grammar.tab.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    UIDL_CHAR = 258,               /* UIDL_CHAR  */
    UIDL_UCHAR = 259,              /* UIDL_UCHAR  */
    UIDL_SHORT = 260,              /* UIDL_SHORT  */
    UIDL_USHORT = 261,             /* UIDL_USHORT  */
    UIDL_INT32 = 262,              /* UIDL_INT32  */
    UIDL_UINT32 = 263,             /* UIDL_UINT32  */
    UIDL_INT64 = 264,              /* UIDL_INT64  */
    UIDL_UINT64 = 265,             /* UIDL_UINT64  */
    UIDL_FLOAT = 266,              /* UIDL_FLOAT  */
    UIDL_DOUBLE = 267,             /* UIDL_DOUBLE  */
    UIDL_ASTRING = 268,            /* UIDL_ASTRING  */
    UIDL_TSTRING = 269,            /* UIDL_TSTRING  */
    UIDL_WSTRING = 270,            /* UIDL_WSTRING  */
    UIDL_SERIAL = 271,             /* UIDL_SERIAL  */
    UIDL_DATETIME = 272,           /* UIDL_DATETIME  */
    UIDL_ASTRVEC = 273,            /* UIDL_ASTRVEC  */
    UIDL_TSTRVEC = 274,            /* UIDL_TSTRVEC  */
    UIDL_WSTRVEC = 275,            /* UIDL_WSTRVEC  */
    UIDL_ANY = 276,                /* UIDL_ANY  */
    UIDL_ENUM = 277,               /* UIDL_ENUM  */
    UIDL_STRUCT = 278,             /* UIDL_STRUCT  */
    UIDL_NAMESPACE = 279,          /* UIDL_NAMESPACE  */
    UIDL_IDENTIFIER = 280,         /* UIDL_IDENTIFIER  */
    UIDL_INTEGER_LITERAL = 281,    /* UIDL_INTEGER_LITERAL  */
    UIDL_DOUBLE_LITERAL = 282,     /* UIDL_DOUBLE_LITERAL  */
    UIDL_STRING_LITERAL = 283,     /* UIDL_STRING_LITERAL  */
    UIDL_COMMENT = 284,            /* UIDL_COMMENT  */
    UIDL_USER_START = 285          /* UIDL_USER_START  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif




int yyparse (yyscan_t scanner, zdl_parser* parser);


#endif /* !YY_YY_ZDL_GRAMMAR_TAB_HPP_INCLUDED  */
