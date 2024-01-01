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
/*****************************************************************
 * @file zdl_visitor.h
 * @brief
 *****************************************************************/

#ifndef __zdl_visitor_h__
#define __zdl_visitor_h__

#include <zce/zce_object.h>

class zdl_module;
class zdl_enum;
class zdl_struct;
class zdl_member;
class zdl_builtin;

class zdl_visitor : public zce_object
{
protected:
    typedef zce_smartptr<zdl_module>  zdl_module_ptr;
    typedef zce_smartptr<zdl_enum>    zdl_enum_ptr;
    typedef zce_smartptr<zdl_struct>  zdl_struct_ptr;
    typedef zce_smartptr<zdl_member>  zdl_member_ptr;

public:
    virtual bool visit_module_start(const zdl_module_ptr& module){ return true; };
    virtual bool visit_module_end(const zdl_module_ptr& module) { return true; };

    virtual bool visit_enum_start(const zdl_enum* t_enum) { return true; };
    virtual bool visit_enum_end(const zdl_enum* t_enum) { return true; };

    virtual bool visit_struct_start(const zdl_struct* t_struct) { return true; };
    virtual bool visit_struct_end(const zdl_struct* t_struct) { return true; };

    virtual bool visit_builtin(const zdl_builtin* t_buildin) { return true; };

    virtual void visit_member(const zdl_member* member, bool is_last) {};
};
typedef zce_smartptr<zdl_visitor> zdl_visitor_ptr;

#endif /*__zdl_visitor_h__*/
