%{

/* ***************************************************************
 *  Copyright (C) 2006  Yongming Wang(wangym@gmail.com)
 *  All Rights Reserved
 * ***************************************************************/


#include "zdl_lex.h"
#include "zdl/zdl_utils.h"
#include "zdl/zdl_parser.h"
#include "zdl_grammar.tab.hpp"
#include "unistd.h"

zdl_keyword_map_ptr g_keyword_map_ptr(new zdl_keyword_map());

static void init_keyword_map();

void zdl_yy_user_init();

#define YYSTYPE std::string
#define YY_USER_INIT zdl_yy_user_init();
#define YY_NO_UNISTD_H
#define YY_DECL int yylex(YYSTYPE* yylvalp, yyscan_t yyscanner, zdl_parser* parser)
%}

%option reentrant
%option noyywrap
%option outfile="zdl_lex.cpp"

identifier          [[:alpha:]_][[:alnum:]_]*
integer_constant    (\+|-)?((0[0-7]+)|(0x[[:xdigit:]]+)|([[:digit:]]+))

%%

"\"" {
	// STRING
    int c;
    std::string str;
    bool escape = false;
    do{
        escape = false;
        c = yyinput(yyscanner);
        if(c == '\\'){
		  escape = true;
          c = yyinput(yyscanner);
        }
        if (c == EOF)
			break;
        if (!escape && c=='\"')
			break;
	    str += static_cast<char>(c);        
    }while(1);
    *yylvalp = str;
	return UIDL_STRING_LITERAL;	
}

"//" {  
    // C++-style comment
    int c;
    std::string comment = yytext+2;
    do{
        c = yyinput(yyscanner);
        if(c == '\n'){
            parser->read_cr();
        }
        else if (c != EOF)
	        comment += static_cast<char>(c);        
    }
    while(c != '\n' && c != EOF);
    //parser->set_comment(comment);
}

"/*" {
    // C-style comment
    std::string comment = yytext + 2;
    while(true){
        int c = yyinput(yyscanner);
        if(c == '\n'){
            comment += static_cast<char>(c);
            parser->read_cr();
        }
        else if(c == '*'){
            int next = yyinput(yyscanner);
            if(next == '/'){
                break;
            }
            else{
                comment += static_cast<char>(c);
                unput(next);
            }
        }
        else if(c == EOF){
            parser->warning("EOF in comment");
            break;
        }
        else{
            comment += static_cast<char>(c);
        }
    }
	//parser->set_comment(comment);
	*yylvalp = comment;
	return UIDL_COMMENT;
}

{identifier} {
    std::string id = yytext;
    int ret = g_keyword_map_ptr->get_identifier_by_name(id);

    *yylvalp = yytext;

    if (ret != UIDL_IDENTIFIER){
        //printf("read keyword %s %d\n", id.c_str(), ret);
        return ret;
    }
    //printf("read id %s ===\n", id.c_str());
    return UIDL_IDENTIFIER;
}

{integer_constant} {
    *yylvalp = yytext;
    return UIDL_INTEGER_LITERAL;
}

[[:space:]] {
    // Igore white-space
    if (yytext[0] == '\n'){
        parser->read_cr();
    }
}

. {
    *yylvalp = yytext;
    return yytext[0];
}

%%

zdl_keyword::zdl_keyword(const char* name, int id)
:name_(name), id_(id), payload_(0)
{
}


zdl_keyword_map::zdl_keyword_map()
{
    add_new_keyword("char",			UIDL_CHAR);
    add_new_keyword("uchar",		UIDL_UCHAR);
    add_new_keyword("byte",         UIDL_UCHAR);
    add_new_keyword("short",		UIDL_SHORT);
    add_new_keyword("ushort",		UIDL_USHORT);
    add_new_keyword("int16",        UIDL_SHORT);
    add_new_keyword("uint16",       UIDL_USHORT);
	add_new_keyword("int",			UIDL_INT32);
    add_new_keyword("int32",		UIDL_INT32);
    add_new_keyword("uint32",		UIDL_UINT32);
    add_new_keyword("int64",		UIDL_INT64);
    add_new_keyword("uint64",		UIDL_UINT64);
    add_new_keyword("float",		UIDL_FLOAT);
    add_new_keyword("double",		UIDL_DOUBLE);
    add_new_keyword("ntchars",		UIDL_TSTRING);
    add_new_keyword("string",		UIDL_TSTRING);
    add_new_keyword("tstring",		UIDL_TSTRING);
    add_new_keyword("astring",		UIDL_ASTRING);
    add_new_keyword("wstring",		UIDL_WSTRING);
    add_new_keyword("strvec",		UIDL_TSTRVEC);
    add_new_keyword("tstrvec",		UIDL_TSTRVEC);
    add_new_keyword("astrvec",		UIDL_ASTRVEC);
    add_new_keyword("wstrvec",		UIDL_WSTRVEC);
	add_new_keyword("serial",		UIDL_SERIAL);
    add_new_keyword("datetime",		UIDL_DATETIME);
    add_new_keyword("enum",			UIDL_ENUM);
    add_new_keyword("struct",		UIDL_STRUCT);
    add_new_keyword("namespace",	UIDL_NAMESPACE);
}

void zdl_keyword_map::add_new_keyword(const char* name, int id)
{
    zdl_keyword_ptr keyw(new zdl_keyword(name, id));
    name_keyword_map_.insert(std::make_pair(std::string(name), keyw));
    id_keyword_map_.insert(std::make_pair(id, keyw));
}

int zdl_keyword_map::get_identifier_by_name(const char* name)
{
    return get_identifier_by_name(std::string(name));
}

int zdl_keyword_map::get_identifier_by_name(const std::string& name)
{
    std::map<std::string, zdl_keyword_ptr>::const_iterator pos = name_keyword_map_.find(name) ;
    if(pos != name_keyword_map_.end())
    {
        return pos->second->id();
    }
    return UIDL_IDENTIFIER;
}

void zdl_yy_user_init()
{
//init nothing to do ?
}