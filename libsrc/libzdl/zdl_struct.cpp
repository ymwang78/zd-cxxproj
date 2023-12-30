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
#include <algorithm>
#include "zdl/zdl_struct.h"
#include "zdl/zdl_member.h"
#include "zdl/zdl_parser.h"
#include "zdl/zdl_parser_context.h"
#include "zdl_grammar.tab.hpp"
#include "zdl/zdl_visitor.h"
#pragma warning (push, 3)
#include <boost/tokenizer.hpp>
#pragma warning (pop)

zdl_struct::zdl_struct(const zdl_module_ptr& module_ptr, const std::string& name)
:zdl_type(UIDL_STRUCT, module_ptr, name), refcount_(0)
{

};

zdl_struct::~zdl_struct()
{

}

void zdl_struct::add_template_arg(const std::string& arg)
{
    boost::char_separator<char> sep(",");
    boost::tokenizer< boost::char_separator<char> > tokens(arg, sep);

    for (boost::tokenizer< boost::char_separator<char> >::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        std::string strPart = *it;
        zdl_template_arg_ptr arg_ptr(new zdl_template_arg(strPart));
        args_.push_back(arg_ptr);
    }
};

int zdl_struct::add_member(const zdl_member_ptr& member)
{
    std::vector<zdl_member_ptr>::iterator pos;
    pos = find_if(members_.begin(), members_.end(), [=](const zdl_member_ptr& ptr)->bool {
            return ptr->var_name() == member->var_name();
        });
    if (pos!=members_.end())
    {
        ZCE_ERROR((ZLOG_ERROR, "member %s already exists in %s\n", 
            member->var_name().c_str(),
            name().c_str()));
        return -1;
    }
    members_.push_back(member);

    if (!member->is_array() && member->arg_size()!=0)
    {
        template_nonarray_members_.push_back(member);
    }

    //////////////////////////////////////////////////////////////////////////
    
    if (member->type()->type_e() == type_builtin_e)
    {
        if (!member->is_string())
        {
            meta_member_map_["_builtin_digit"].push_back(member);
        }
        meta_member_map_["_builtin"].push_back(member);
    }
    else if (member->type()->type_e() == type_struct_e)
    {
        meta_member_map_["_struct"].push_back(member);
        ((zdl_struct*)member->type())->addrefcount();
    }

    //////////////////////////////////////////////////////////////////////////
    std::vector<std::pair<std::string, std::string> > vec_meta = member->vec_metas();
    if (vec_meta.empty())
    {
        std::map<std::string, std::vector<zdl_member_ptr> >::iterator iter = meta_member_map_.find("");
        if(iter != meta_member_map_.end())
        {
            iter->second.push_back(member);
        }
        else
        {
            std::vector<zdl_member_ptr> vec;
            vec.push_back(member);
            meta_member_map_.insert(std::make_pair("", vec));
        }
    }
    else
    {
        for (std::vector<std::pair<std::string, std::string> >::iterator vec_iter = vec_meta.begin();
            vec_iter!=vec_meta.end(); ++vec_iter)
        {
            std::map<std::string, std::vector<zdl_member_ptr> >::iterator iter = meta_member_map_.find(vec_iter->first);
            if(iter != meta_member_map_.end())
            {
                iter->second.push_back(member);
            }
            else
            {
                std::vector<zdl_member_ptr> vec;
                vec.push_back(member);
                meta_member_map_.insert(std::make_pair(vec_iter->first, vec));
            }
        }
    }

    return 0;
}

int zdl_struct::get_member_count() const
{
    return (int) members_.size();
}

int zdl_struct::get_member_count(const std::string& meta) const
{
    std::map<std::string, std::vector<zdl_member_ptr> >::const_iterator map_iter 
        = meta_member_map_.find(meta);
    if (map_iter == meta_member_map_.end())
        return 0;
    return (int) map_iter->second.size();
}

void zdl_struct::visit(const zdl_visitor_ptr& visitor) const
{
    int i = 0;
    bool ret = visitor->visit_struct_start(this);
    if (ret)
    {
        std::vector<zdl_member_ptr>::const_iterator iter;
        for(iter = members_.begin(); iter != members_.end(); ++iter, ++i)
        {
            visitor->visit_member(*iter, i == (members_.size()-1));
        }
        //for_each(members_.begin(), members_.end(), 
        //    bind2nd(zdl_member::zdl_member_visitor(), visitor));
        visitor->visit_struct_end(this);
    }
}

void zdl_struct::visit(const zdl_visitor_ptr& visitor, const std::string& meta, bool include) const
{
    visitor->visit_struct_start(this);
    if (include)
    {
        std::map<std::string, std::vector<zdl_member_ptr> >::const_iterator map_iter 
            = meta_member_map_.find(meta);
        if (map_iter != meta_member_map_.end())
        {
            int i = 0;
            std::vector<zdl_member_ptr>::const_iterator iter;
            for(iter = map_iter->second.begin(); iter != map_iter->second.end(); ++iter, ++i)
            {
                visitor->visit_member(*iter, i == (map_iter->second.size()-1));
            }        
        }
    }
    else
    {
        std::vector<zdl_member_ptr>::const_iterator iter = members_.begin();
        int count = 0;
        for (; iter != members_.end(); ++iter)
        {
            if ((*iter)->fetch_meta(meta))
                continue;
            ++count;
        }
        for (iter = members_.begin(); iter != members_.end(); ++iter)
        {
            if ((*iter)->fetch_meta(meta))
                continue;
            --count;
            visitor->visit_member(*iter, !count);
        }
    }
    visitor->visit_struct_end(this);
}