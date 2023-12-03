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
#include <zce/zce_config.h>
#include "zdl/zdl_enum.h"
#include "zdl/zdl_parser.h"
#include "zdl_grammar.tab.hpp"
#include "zdl/zdl_visitor.h"

zdl_enum::zdl_enum(const std::string& name)
:zdl_type(UIDL_ENUM, name), current_enumerator_val_(0)
{

}

int zdl_enum::add_enumerator(unsigned long val, const std::string& emname, const std::string& comment)
{
    if (emname.substr(0,2) == "e_")
        *((char*)&emname[0]) = 'E';

    if (find_enumerator(emname)){
        ZCE_ERROR((ZLOG_ERROR, "enumerator %s already exists.\n", emname.c_str()));
        return -1;
    }
    zdl_enumerator_ptr enumorator(new zdl_enumerator(this, emname, val, comment));
    enumerators_.push_back(enumorator);
    current_enumerator_val_ = ++val;
    return 0;
}

int zdl_enum::add_enumerator(const std::string& emname, const std::string& comment)
{
    return add_enumerator(current_enumerator_val_, emname, comment);
}

zdl_enumerator_ptr zdl_enum::find_enumerator(const std::string& emname)
{
    std::vector<zdl_enumerator_ptr>::iterator pos;
    pos = find_if(enumerators_.begin(), enumerators_.end(), 
        bind2nd(zdl_enumerator::zdl_enumerator_match(), emname));
    if(pos!=enumerators().end()){
        return *pos;
    }
    return zdl_enumerator_ptr((zdl_enumerator*)0);
}

void zdl_enum::visit(const zdl_visitor_ptr& visitor) const
{
    visitor->visit_enum_start(this);
    visitor->visit_enum_end(this);
}