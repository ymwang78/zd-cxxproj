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
#include "zdl/zdl_type.h"
#include "zdl/zdl_builtin.h"
#include "zdl/zdl_parser.h"
#include "zdl/zdl_builtin.h"
#include "zdl/zdl_enum.h"
#include "zdl/zdl_struct.h"
#include "zdl/zdl_parser_context.h"
#include "zdl/zdl_member.h"
#include "zdl_grammar.tab.hpp"

zdl_type::zdl_type(int tpid, const zce_smartptr<zdl_module>& module_ptr, const std::string& name)
    :type_id_(tpid), module_ptr_(module_ptr), type_name_(name) {
    if (module_ptr_)
        full_name_ = module_ptr_->name_space() + "::" + type_name_;
    else
        full_name_ = type_name_;
};

zdl_type::~zdl_type()
{
}

zdl_type::zdl_type_e zdl_type::get_type_e (int tpid)
{
    switch(tpid)
    {
    case UIDL_CHAR:
    case UIDL_UCHAR:
    case UIDL_SHORT:
    case UIDL_USHORT:
    case UIDL_INT32:
    case UIDL_UINT32:
    case UIDL_INT64:
    case UIDL_UINT64:
    case UIDL_FLOAT:
    case UIDL_DOUBLE:
    case UIDL_ASTRING:
    case UIDL_TSTRING:
    case UIDL_WSTRING:
    case UIDL_ANY:
    case UIDL_SERIAL:
    case UIDL_DATETIME:
    case UIDL_ASTRVEC:
    case UIDL_TSTRVEC:
    case UIDL_WSTRVEC:
        return type_builtin_e;
    case UIDL_ENUM:
        return type_enum_e;
    case UIDL_STRUCT:
        return type_struct_e;
    default:
        return type_unknow_e;
    }
};
