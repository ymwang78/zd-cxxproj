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

#include "zdl/zdl_builtin.h"
#include "zdl/zdl_visitor.h"
#include "zdl_grammar.tab.hpp"

std::string zdl_builtin::builtin_name(int tpid)
{
    switch (tpid)
    {
    case UIDL_CHAR:
        return "zce_char";
    case UIDL_UCHAR:
        return "zce_byte";
    case UIDL_SHORT:
        return "zce_int16";
    case UIDL_USHORT:
        return "zce_uint16";
    case UIDL_INT32:
        return "zce_int32";
    case UIDL_UINT32:
        return "zce_uint32";
    case UIDL_INT64:
        return "zce_int64";
    case UIDL_UINT64:
        return "zce_uint64";
    case UIDL_ASTRING:
        return "zce_astring";
    case UIDL_TSTRING:
        return "zce_string";
    case UIDL_WSTRING:
        return "zce_wstring";
    case UIDL_ASTRVEC:
        return "zce_astr";
    case UIDL_TSTRVEC:
        return "zce_str";
    case UIDL_WSTRVEC:
        return "zce_wstr";
    case UIDL_FLOAT:
        return "zce_float";
    case UIDL_DOUBLE:
        return "zce_double";
    case UIDL_SERIAL:
        return "zce_serial";
    }
    return "";
}

zdl_builtin::zdl_builtin(int tpid)
:zdl_type(tpid, builtin_name(tpid))
{
}

void zdl_builtin::visit(const zdl_visitor_ptr& visitor) const
{
    visitor->visit_builtin(this);
}