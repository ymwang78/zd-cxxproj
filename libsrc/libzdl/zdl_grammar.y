%code requires {

class zdl_parser;

typedef void* yyscan_t;

}

%{

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
%}

%define api.pure

%param { yyscan_t scanner }
%lex-param { zdl_parser* parser }
%parse-param { zdl_parser* parser }

%token UIDL_CHAR
%token UIDL_UCHAR
%token UIDL_SHORT
%token UIDL_USHORT
%token UIDL_INT32
%token UIDL_UINT32
%token UIDL_INT64
%token UIDL_UINT64
%token UIDL_FLOAT
%token UIDL_DOUBLE
%token UIDL_ASTRING
%token UIDL_TSTRING
%token UIDL_WSTRING
%token UIDL_SERIAL
%token UIDL_DATETIME
%token UIDL_ASTRVEC
%token UIDL_TSTRVEC
%token UIDL_WSTRVEC
%token UIDL_ANY
%token UIDL_ENUM
%token UIDL_STRUCT
%token UIDL_NAMESPACE

%token UIDL_IDENTIFIER
%token UIDL_INTEGER_LITERAL
%token UIDL_DOUBLE_LITERAL
%token UIDL_STRING_LITERAL
%token UIDL_COMMENT


%token UIDL_USER_START
%%

// ----------------------------------------------------------------------
start
:namespace_def
{
}
;

initval
:'='UIDL_INTEGER_LITERAL
{
$$=$2;
}
|'='UIDL_STRING_LITERAL
{
$$=$2;
}
|'='UIDL_DOUBLE_LITERAL
{
$$=$2;
}
|
{
$$="";
}

comments
:UIDL_COMMENT comments
{
    if ($2 != "")
    {
        $$ = $1 + "\n" + $2;
    }
    else
    {
        $$ = $1;
    }	
}
|
{
    $$ = "";
}
;

// ----------------------------------------------------------------------
namespace_def
:UIDL_NAMESPACE UIDL_IDENTIFIER 
{
    parser->context_ptr()->set_namespace($2);
}
'{' definitions '}'
{
}
|
{
}
;

// ----------------------------------------------------------------------
definitions
:definition ';' definitions
{
}
|definition
{
    yyerror(scanner, parser, "';' missing after definition");
}
|error ';'
{
    yyerror(scanner, parser, "error got");
}
| 
{
}
;

// ----------------------------------------------------------------------
definition
:struct_def
{
    parser->context_ptr()->add_type_end($1);
}
|enum_def
{
    parser->context_ptr()->add_type_end($1);
}
;

meta_key
:UIDL_IDENTIFIER
{
  $$ = $1;
}

meta_val
:',' UIDL_STRING_LITERAL
{
  $$ = std::string("\"") + $2 + "\"";
}
|',' UIDL_INTEGER_LITERAL
{
  $$ = $2;
}
|
{
  $$ = "";
}

meta_data
: '[' meta_key meta_val  ']'
{
    parser->context_ptr()->add_current_member_metadata($2, $3);
    $$ = $2;
}
;

meta_data_set
:meta_data meta_data_set
{
}
|
{
}

num
:UIDL_INTEGER_LITERAL
{
    $$ = $1;
}
|
{
    $$ = "0"; 
}

//-----------------------------------------------------------------------
limit_arg
:num
{
    int val = strtol($1.c_str(), 0, 0);
    if (val < 0){
        yyerror(scanner, parser, "invalid neg array length");
    }
    $$ = $1;
}
|
UIDL_IDENTIFIER
{
    $$ = $1 + "_";
}
;


template_args
:limit_arg ',' template_args
{
    $$ = $1 + "," + $3;
}
|limit_arg
{
    $$ = $1;
}
;

template
: '<' template_args '>'
{
    $$ = $2;
}
|
{
    $$ = "";
}
;


// ----------------------------------------------------------------------
type_id
:type UIDL_IDENTIFIER
{
    parser->context_ptr()->add_member_start($2, "1", "1");
    $$ = $2;
}
|type '*' UIDL_IDENTIFIER
{
    int val = 0;
    if (val >=0){
        parser->context_ptr()->add_member_start($3, "0", "0");
    }
    else if (val<0){
        yyerror(scanner, parser, "invalid neg array length");
    }
    $$ = $3;
}
|type UIDL_IDENTIFIER '[' limit_arg '~' limit_arg ']'
{
    //int low_val = strtol($4.c_str(), 0, 0);
    //int up_val = strtol($6.c_str(), 0, 0);
    parser->context_ptr()->add_member_start($2, $4, $6);
    $$ = $2;
}
|type UIDL_IDENTIFIER '[' limit_arg ']'
{
    //int val = strtol($4.c_str(), 0, 0);
    parser->context_ptr()->add_member_start($2, $4, $4);
    $$ = $2;
}
;

// ----------------------------------------------------------------------
struct_id
:comments meta_data_set UIDL_STRUCT UIDL_IDENTIFIER template comments
{
    parser->context_ptr()->add_type_start(UIDL_STRUCT, $4);
    parser->context_ptr()->add_current_member_metadata("_previous_comment", $1);
    parser->context_ptr()->add_type_template_arg($5);
    parser->context_ptr()->set_comment($6);
    $$ = $4;
}
|comments meta_data_set UIDL_STRUCT keyword comments
{
    yyerror(scanner, parser, "keyword cannot be used as struct name");
}
;

// ----------------------------------------------------------------------
struct_def
:struct_id
{
    //yyerror(scanner, parser, err.c_str());
    ZCE_DEBUG((ZLOG_DEBUG, "parser struct %s", $1.c_str()));
}
'{' struct_exports '}'
{
}
;

// ----------------------------------------------------------------------
struct_exports
:struct_export ';' struct_exports
{
    //printf("struct_exports :%s %s\n", $1.c_str(), $2.c_str());
}
|struct_export
{
    yyerror(scanner, parser, "';' missing after struct definition");
}
|
{
}
;

// ----------------------------------------------------------------------
struct_export
:comments meta_data_set data_member initval comments
{
    parser->context_ptr()->add_current_member_metadata("_previous_comment", $1);
    parser->context_ptr()->set_comment($5);
    parser->context_ptr()->add_current_member_metadata("_default_val", $4);
    parser->context_ptr()->add_member_end($3);
    $$=$3;
}
;

// ----------------------------------------------------------------------
data_member
:type_id
{
    $$=$1;
}
|type keyword
{
    yyerror(scanner, parser, "keyword  cannot be used as data member name");
}
|type
{
    yyerror(scanner, parser, "missing data member name");
}
;

// ----------------------------------------------------------------------
enum_id
:UIDL_ENUM UIDL_IDENTIFIER
{
    parser->context_ptr()->add_type_start(UIDL_ENUM, $2);
    $$ = $2;
}
|UIDL_ENUM keyword
{
    yyerror(scanner, parser, "keyword cannot be used as enumeration name");
}
;

// ----------------------------------------------------------------------
enum_def
:enum_id 
{
}
'{' enumerator_list '}'
{
}
;

// ----------------------------------------------------------------------
enumerator_list
:enumerator ',' enumerator_list
{
}
|enumerator
{
}
|
{
}  
;

// ----------------------------------------------------------------------
enumerator
:UIDL_IDENTIFIER '=' UIDL_INTEGER_LITERAL comments
{
    unsigned long val = strtoul($3.c_str(), 0, 0);
    parser->context_ptr()->add_enumerator(val, $1, $4);
}
|UIDL_IDENTIFIER comments
{
    parser->context_ptr()->add_enumerator($1, $2);
}
|keyword
{
    yyerror(scanner, parser, "keyword cannot be used as enumerator");
}
;


// ----------------------------------------------------------------------
type
:basetype
{
}
|usertype
{
}

basetype
:UIDL_CHAR
{
    parser->context_ptr()->set_current_member_type(UIDL_CHAR);
}
|UIDL_UCHAR
{
    parser->context_ptr()->set_current_member_type(UIDL_UCHAR);
}
|UIDL_SHORT
{
    parser->context_ptr()->set_current_member_type(UIDL_SHORT);
}
|UIDL_USHORT
{
    parser->context_ptr()->set_current_member_type(UIDL_USHORT);
}
|UIDL_INT32
{
    parser->context_ptr()->set_current_member_type(UIDL_INT32);
}
|UIDL_UINT32
{
    parser->context_ptr()->set_current_member_type(UIDL_UINT32);
}
|UIDL_INT64
{
    parser->context_ptr()->set_current_member_type(UIDL_INT64);
}
|UIDL_UINT64
{
    parser->context_ptr()->set_current_member_type(UIDL_UINT64);
}
|UIDL_FLOAT
{
    parser->context_ptr()->set_current_member_type(UIDL_FLOAT);
}
|UIDL_DOUBLE
{
    parser->context_ptr()->set_current_member_type(UIDL_DOUBLE);
}
|UIDL_ASTRING
{
    parser->context_ptr()->set_current_member_type(UIDL_ASTRING);
}
|UIDL_TSTRING
{
    parser->context_ptr()->set_current_member_type(UIDL_TSTRING);
}
|UIDL_WSTRING
{
    parser->context_ptr()->set_current_member_type(UIDL_WSTRING);
}
|UIDL_ASTRVEC
{
    parser->context_ptr()->set_current_member_type(UIDL_ASTRVEC);
}
|UIDL_TSTRVEC
{
    parser->context_ptr()->set_current_member_type(UIDL_TSTRVEC);
}
|UIDL_WSTRVEC
{
    parser->context_ptr()->set_current_member_type(UIDL_WSTRVEC);
}
|UIDL_SERIAL
{
    parser->context_ptr()->set_current_member_type(UIDL_SERIAL);
}
|UIDL_DATETIME
{
    parser->context_ptr()->set_current_member_type(UIDL_DATETIME);
}
|UIDL_ANY
{
    parser->context_ptr()->set_current_member_type(UIDL_ANY);
}
;

usertype
:UIDL_STRUCT UIDL_IDENTIFIER template
{
    parser->context_ptr()->set_current_member_type(UIDL_STRUCT, $3, $2);
}
|UIDL_STRUCT UIDL_IDENTIFIER ':' ':' UIDL_IDENTIFIER template
{
    parser->context_ptr()->set_current_member_type(UIDL_STRUCT, $6, $5, $2);
}
|UIDL_ENUM UIDL_IDENTIFIER
{
    parser->context_ptr()->set_current_member_type(UIDL_ENUM, "", $2);
}
//-------------------------------------------------------
keyword
:type
|UIDL_ENUM
|UIDL_STRUCT
;

%%

