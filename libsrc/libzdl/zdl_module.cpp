// ***************************************************************
//  zdl_member   version:  1.0   date: 04/05/2007
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_config.h>
#include "zdl/zdl_module.h"
#include "zdl/zdl_type.h"
#include "zdl/zdl_builtin.h"
#include "zdl/zdl_member.h"
#include "zdl/zdl_struct.h"
#include "zdl/zdl_enum.h"
#include "zdl/zdl_parser.h"
#include "zdl/zdl_parser_context.h"
#include "zdl_grammar.tab.hpp"

zdl_module::zdl_module(const std::string& ns)
    :namespace_name_(ns) 
{
    int tpid = UIDL_CHAR;
    while (tpid < UIDL_ENUM) {
        zdl_builtin_ptr bt(new zdl_builtin(tpid));
        add_type(zdl_type_ptr::__dynamic_cast(bt));
        tpid++;
    }
};

//引用其他模块变量
zce_smartptr<zdl_type> zdl_module::create_type(const std::string& name, zdl_type::zdl_type_e e)
{
    auto iter = type_dict_.find(name);
    if (iter != type_dict_.end()) {
        return iter->second;
    }
    switch (e) {
    case zdl_type::type_unknow_e:
        break;
    case zdl_type::type_builtin_e:
        break;
    case zdl_type::type_enum_e: {
        zdl_type_ptr ptr(new zdl_enum(zce_smartptr<zdl_module>(this), name));
        type_dict_.insert(std::make_pair(name, ptr));
        return ptr;
    }
    break;
    case zdl_type::type_struct_e: {
        zdl_type_ptr ptr(new zdl_struct(zce_smartptr<zdl_module>(this), name));
        type_dict_.insert(std::make_pair(name, ptr));
        return ptr;
    }
    default:
        break;
    }
    return zdl_type_ptr(0);

}

zdl_type_ptr zdl_module::get_builtin_type(int tpid)  const
{
    return get_type(zdl_builtin::builtin_name(tpid));
}

zdl_type_ptr zdl_module::get_type(const std::string& name)  const
{
    auto iter = std::find_if(type_vec_.begin(), type_vec_.end(),
        [=](const zdl_type_ptr& ptr)->bool {
            return ptr->full_name() == name || ptr->name() == name;
        }
    );
    if (iter != type_vec_.end())
        return *iter;
    return zdl_type_ptr((zdl_type*)0);
}

int zdl_module::add_type(const zdl_type_ptr& type_ptr)
{
    zdl_type_ptr tp = get_type(type_ptr->name());
    if (tp) {
        ZCE_ERROR((ZLOG_ERROR, "type  %s already exists\n", type_ptr->name().c_str()));
        return -1;
    }
    type_vec_.push_back(type_ptr);

    //@todo 这里可能是被别的模块引用首先创建了结构体
    auto iter = type_dict_.find(type_ptr->name());
    if (iter != type_dict_.end()) {
        return -1;
    }
    type_dict_.insert(std::make_pair(type_ptr->name(), type_ptr));
    type_vec_.push_back(type_ptr);
    return 0;
}


void zdl_module::visit(const zdl_visitor_ptr& visitor) const
{
    std::vector<zdl_type_ptr>::const_iterator pos;
    for (pos = type_vec_.begin(); pos != type_vec_.end(); ++pos) {
        (*pos)->visit(visitor);
    }
}

void zdl_module::visit(const zdl_visitor_ptr& visitor, const std::string& meta, bool include) const
{
    std::vector<zdl_type_ptr>::const_iterator pos;
    for (pos = type_vec_.begin(); pos != type_vec_.end(); ++pos) {
        (*pos)->visit(visitor, meta, include);
    }
}

void zdl_module::visit_type_meta(const zdl_visitor_ptr& visitor, const std::string& meta, bool include) const
{
    std::vector<zdl_type_ptr>::const_iterator pos;
    for (pos = type_vec_.begin(); pos != type_vec_.end(); ++pos) {
        const void* meta_val = (*pos)->fetch_meta(meta, (const char*)-1);
        if (include && meta_val != (const char*)-1) {
            (*pos)->visit(visitor);
        }
        else if (!include && meta_val == (const char*)-1) {
            (*pos)->visit(visitor);
        }
    }
}
