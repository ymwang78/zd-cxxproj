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
#include "zdl/zdl_parser_context.h"
#include "zdl/zdl_builtin.h"
#include "zdl/zdl_struct.h"
#include "zdl/zdl_enum.h"
#include "zdl/zdl_member.h"
#include "zdl/zdl_type.h"
#include "zdl/zdl_parser.h"
#include "zdl/zdl_module.h"

zdl_parser_context::zdl_parser_context(std::map<std::string, zdl_module_ptr>& modules)
:modules_(modules)
{
}

//void zdl_parser_context::add_member_start(const std::string& varname)
//{
//    if (current_struct_ == 0){
//        ZCE_ERROR((ZLOG_ERROR, "ONLY STRUCT CAN CONTAIN MEMBERS\n"));
//        return;
//    }
//    zdl_member_ptr member(new zdl_member(current_struct_.get(), varname));
//    current_member_ = member;
//    current_member_->type(current_member_type_.get());
//}

///[minsize,maxsize], if unknow(pointer) maxsize == 0
void zdl_parser_context::add_member_start(const std::string& varname, const std::string& minsize, const std::string& maxsize)
{
    if (current_struct_ == 0){
        ZCE_ERROR((ZLOG_ERROR, "ONLY STRUCT CAN CONTAIN MEMBERS\n"));
        return;
    }
    zdl_member_ptr member (new zdl_member(current_struct_, current_struct_->get_member_count(), varname, minsize, maxsize));
    current_member_ = member; 
    current_member_->type(current_member_type_);
    if (current_member_arg_ != "")
    {
        current_member_->add_template_arg(current_member_arg_);
    }
}

void zdl_parser_context::add_member_end(const std::string& varname)
{
    if (current_struct_ == 0){
        ZCE_ERROR((ZLOG_ERROR, "ONLY STRUCT CAN CONTAIN MEMBERS\n"));
        return;
    }
    current_member_->vec_metas(current_metas_);
    current_metas_.clear();

    current_struct_->add_member(current_member_);
    current_member_ = zdl_member_ptr((zdl_member*)0);
}

void zdl_parser_context::set_current_member_type(int tpid)
{
    current_member_type_ = module_ptr_->type_container()->get_builtin_type(tpid);
}

void zdl_parser_context::set_current_member_type(int tpid,
    const std::string& tparg, const std::string& tpname, const std::string& ns)
{
    //ZCE_ERROR((ZLOG_ERROR, "set_current_member_type %s, %s\n", tparg.c_str(), tpname.c_str()));
    current_member_arg_ = tparg;
    zdl_type::zdl_type_e tp_e = zdl_type::get_type_e(tpid);
    switch(tp_e){
        case zdl_type::type_builtin_e:
        case zdl_type::type_builtinvec_e:
            set_current_member_type(tpid);
            return;
        case zdl_type::type_enum_e:
        case zdl_type::type_struct_e:
            {
                if (ns.empty())
                {
                    zdl_type_ptr type = module_ptr_->type_container()->get_type(tpname);
                    if (type == 0)
                    {
                        if (tpname == current_struct_->name())
                        {
                            current_member_type_ = current_struct_;
                            break;
                        }
                        else
                        {
                            ZCE_ERROR((ZLOG_ERROR, "type %s not defined\n", tpname.c_str()));
                            exit(1);
                        }
                    }
                    if (type->id() != tpid) {
                        ZCE_ERROR((ZLOG_ERROR, "unmathed type id(%d) and name(%s)\n", tpid, tpname.c_str()));
                        return;
                    }
                    current_member_type_ = type;
                }
                else
                {
                    zdl_type_ptr type = module_ptr_->type_container()->create_ns_type(ns, tpname, tp_e);
                    current_member_type_ = type;
                }
                //current_member_->type(type.get());
            }
            break;
        default:
            ZCE_ERROR((ZLOG_ERROR, "unknow type id(%d) and name(%s)\n", tpid, tpname.c_str()));
            break;
    }
};

void zdl_parser_context::add_current_member_metadata(const std::string& meta_key, const std::string& meta_val)
{
    current_metas_.push_back(std::make_pair(meta_key, meta_val));
}

void zdl_parser_context::add_enumerator(unsigned long val, const std::string& emname, const std::string& comment)
{
    current_enum_->add_enumerator(val, emname, comment);
}

void zdl_parser_context::add_enumerator(const std::string& emname, const std::string& comment)
{
    current_enum_->add_enumerator(emname, comment);
}


void zdl_parser_context::add_type_start(int tpid, const std::string& tpname)
{
    zdl_type::zdl_type_e tp_e = zdl_type::get_type_e(tpid);
    switch(tp_e){
        case zdl_type::type_builtin_e:
        case zdl_type::type_builtinvec_e:
            ZCE_ERROR((ZLOG_ERROR, "global builtin type id(%d) and name(%s) not support\n", tpid, tpname.c_str()));
            return;
        case zdl_type::type_enum_e:
            {
                zdl_enum_ptr em(new zdl_enum(tpname));
                current_enum_ = em;
                current_struct_ = zdl_struct_ptr((zdl_struct*)0);
            }
            return;
        case zdl_type::type_struct_e:
            {
                zdl_struct_ptr em(new zdl_struct(module_ptr(), tpname));
                current_struct_ = em;
                current_enum_ = zdl_enum_ptr((zdl_enum*)0);
                current_struct_->vec_metas(current_metas_);
            }
            break;
        default:
            ZCE_ERROR((ZLOG_ERROR, "unknow type id(%d) and name(%s)\n", tpid, tpname.c_str()));
            break;
    }
}

void zdl_parser_context::add_type_end(const std::string& tpname)
{
    if (current_struct_!=0)
        module_ptr_->type_container()->add_type(zdl_type_ptr::__dynamic_cast(current_struct_));
    else if (current_enum_ != 0)
        module_ptr_->type_container()->add_type(zdl_type_ptr::__dynamic_cast(current_enum_));
    else
        ZCE_ERROR((ZLOG_ERROR, "unknow type name(%s) to add\n", tpname.c_str()));

    current_struct_ = zdl_struct_ptr((zdl_struct*)0);
    current_enum_ = zdl_enum_ptr((zdl_enum*)0);
}

void zdl_parser_context::set_comment(const std::string& cmt)
{
    if (current_struct_ != 0)
    {
        if (current_member_ != 0)
        {
            current_member_->comment(cmt);
        }
        else
        {
            current_struct_->comment(cmt);
        }
    }
    else if (current_enum_ != 0)
    {

    }
}

void zdl_parser_context::set_namespace(const std::string& ns)
{
    auto iter = modules_.find(ns);
    if (iter != modules_.end()) {
        module_ptr_ = iter->second;
    }
    else {
        zdl_module_ptr module_ptr(new zdl_module(ns));
        modules_.insert(std::make_pair(ns, module_ptr));
        module_ptr_ = module_ptr;
    }
}

void zdl_parser_context::add_type_template_arg(const std::string& arg)
{
    if (current_struct_ != 0)
    {
        current_struct_->add_template_arg(arg);
    }
    else 
    {
        ZCE_ASSERT(false);
    }
}

void zdl_parser_context::add_member_template_arg(const std::string& arg)
{
    if (current_member_ != 0)
    {
        current_member_->add_template_arg(arg);
    }
    else 
    {
        ZCE_ASSERT(false);
    }
}

const zdl_module_ptr& zdl_parser_context::get_module_by_name(const std::string& ns)
{
    auto iter = modules_.find(ns);
    if (iter != modules_.end())
        return iter->second;
    return module_ptr_;
}

