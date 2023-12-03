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

struct equal_typename
{
    equal_typename(const std::string& type_name)
        :type_name_(type_name)
    {
    }
    bool operator()(const zdl_type_ptr& ptr)
    {
        if (ptr == 0)
            return false;
        return ptr->name() == type_name_;
    }

    const std::string& type_name_;
};

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

    case UIDL_SERIAL:
    case UIDL_DATETIME:
        return type_builtin_e;
    case UIDL_ASTRVEC:
    case UIDL_TSTRVEC:
    case UIDL_WSTRVEC:
        return type_builtinvec_e;
    case UIDL_ENUM:
        return type_enum_e;
    case UIDL_STRUCT:
        return type_struct_e;
    default:
        return type_unknow_e;
    }
};

zdl_type_container::zdl_type_container()
{
    int tpid = UIDL_CHAR;
    while(tpid < UIDL_ENUM){
        zdl_builtin_ptr bt(new zdl_builtin(tpid));
        add_type(zdl_type_ptr::__dynamic_cast(bt));
        tpid ++;
    }
}

int zdl_type_container::add_type(const zdl_type_ptr& type_ptr)
{
    if (type_ptr->name() == "IE_HPY_SIT_REQ")
    {
        int i = 1;
    }
    zdl_type_ptr tp = get_type(type_ptr->name());
    if (tp){
        ZCE_ERROR((ZLOG_ERROR, "type  %s already exists\n", type_ptr->name().c_str()));
        return -1;
    }
    types_.push_back(type_ptr);
    //types_.insert(std::make_pair(type_ptr->name(), type_ptr));
    return 0;
}

zdl_type_ptr zdl_type_container::get_type(const std::string& name)
{
    std::vector<zdl_type_ptr>::iterator iter 
        = std::find_if(types_.begin(), types_.end(), equal_typename(name));
    if (iter!=types_.end())
        return *iter;
    return zdl_type_ptr((zdl_type*)0);
    //std::map<std::string, zdl_type_ptr>::iterator pos = types_.find(name);
    //if (pos != types_.end())
    //    return pos->second;
    //return zdl_type_ptr((zdl_type*)0);
}

zdl_type_ptr zdl_type_container::get_builtin_type(int tpid)
{
    return get_type(zdl_builtin::builtin_name(tpid));
}

void zdl_type_container::visit(const zdl_visitor_ptr& visitor) const
{
    std::vector<zdl_type_ptr>::const_iterator pos;
    for(pos = types_.begin(); pos != types_.end(); ++pos)
    {
        (*pos)->visit(visitor);
    }
}

void zdl_type_container::visit(const zdl_visitor_ptr& visitor, const std::string& meta, bool include) const
{
    std::vector<zdl_type_ptr>::const_iterator pos;
    for(pos = types_.begin(); pos != types_.end(); ++pos)
    {
        (*pos)->visit(visitor, meta, include);
    }
}

void zdl_type_container::visit_type_meta(const zdl_visitor_ptr& visitor, const std::string& meta, bool include) const 
{
    std::vector<zdl_type_ptr>::const_iterator pos;
    for(pos = types_.begin(); pos != types_.end(); ++pos)
    {
        const void* meta_val = (*pos)->fetch_meta(meta, (const char*) -1);
        if (include && meta_val!=(const char*) -1)
        {
            (*pos)->visit(visitor);
        }
        else if (!include && meta_val == (const char*) -1)
        {
            (*pos)->visit(visitor);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

zdl_type_ptr zdl_type_container::create_ns_type(const std::string& ns, 
	const std::string& name, zdl_type::zdl_type_e e)
{
    std::string nstype_key = ns + "::" + name;
    auto iter = ns_types_.find(ns);
	if (iter != ns_types_.end()) {
		auto iter2 = iter->second.find(name);
		if (iter2 != iter->second.end()) {
			return iter2->second;
		}
	}
	else {
		auto pb = ns_types_.insert(std::make_pair(ns, std::map<std::string, zdl_type_ptr>()));
		iter = pb.first;
	}
    switch (e)
    {
    case zdl_type::type_unknow_e:
        break;
    case zdl_type::type_builtin_e:
        break;
    case zdl_type::type_enum_e:
    {
        zdl_type_ptr ptr(new zdl_enum(nstype_key));
		iter->second.insert(std::make_pair(name, ptr));
        return ptr;
    }

        break;
    case zdl_type::type_struct_e:
    {
        zdl_type_ptr ptr(new zdl_struct(zdl_module_ptr(0), nstype_key));
		iter->second.insert(std::make_pair(name, ptr));
        return ptr;
    }
    default:
        break;
    }
    return zdl_type_ptr(0);
}