#pragma once
/* ***************************************************************
 *  Copyright (C) 2006  Yongming Wang(wangym@gmail.com)
 *  All Rights Reserved
 *
 *  This file is part of Ubeda project (http://www.ubeda.cn).
 *
 *  This copy of file is licensed to you under Ubeda License.
 *  You should have received a copy of the Ubeda License
 *  along with this program, if not, get it from
 *      http://www.ubeda.cn
 * ***************************************************************/
#ifndef __zdl_parser_h
#define __zdl_parser_h

#include <string>
#include <zce/zce_object.h>

class zdl_parser_context;

class zdl_parser : public zce_object
{
    typedef zce_smartptr<zdl_parser_context> zdl_parser_context_ptr;
public:
    zdl_parser();
    ~zdl_parser();

    void error(const char* s);
    void warning(const char* s);
    void info(const char* s);
    void read_cr();
    int parse(const char* filename, const zdl_parser_context_ptr& parser_context);
    zdl_parser_context_ptr context_ptr() const { return parser_context_; };

private:
    zdl_parser_context_ptr parser_context_;
    int line_;
    char* filename_;
};

typedef zce_smartptr<zdl_parser> zdl_parser_ptr;

#endif //__zdl_parser_h
