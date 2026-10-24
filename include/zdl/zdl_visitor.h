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
struct zdl_enumerator;

class zdl_visitor : public zce::Object
{
protected:
    typedef zce::SmartPtr<zdl_module>  zdl_module_ptr;
    typedef zce::SmartPtr<zdl_enum>    zdl_enum_ptr;
    typedef zce::SmartPtr<zdl_struct>  zdl_struct_ptr;
    typedef zce::SmartPtr<zdl_member>  zdl_member_ptr;

public:
    // zdl_module::visit() calls these once around the loop over its types, with
    // the same veto rule: a false from visit_module_start skips the types and
    // visit_module_end. The handle is a refcounted SmartPtr, so the callbacks
    // are only reported for a module that a SmartPtr actually owns; a module
    // with a zero reference count is still walked, without them.
    virtual bool visit_module_start(const zdl_module_ptr& module){ return true; };
    virtual bool visit_module_end(const zdl_module_ptr& module) { return true; };

    // Traversal contract, the same as zdl_struct::visit(): visit_enum_start,
    // then visit_enumerator once per enumerator in declaration order, then
    // visit_enum_end. A false from visit_enum_start vetoes the walk: neither
    // the enumerators nor visit_enum_end are reported. zdl_enum::visit()
    // drives that order; see libzdl.
    virtual bool visit_enum_start(const zdl_enum* t_enum) { return true; };
    virtual bool visit_enumerator(const zdl_enumerator* enumerator) { return true; };
    virtual bool visit_enum_end(const zdl_enum* t_enum) { return true; };

    virtual bool visit_struct_start(const zdl_struct* t_struct) { return true; };
    virtual bool visit_struct_end(const zdl_struct* t_struct) { return true; };

    virtual bool visit_builtin(const zdl_builtin* t_buildin) { return true; };

    virtual void visit_member(const zdl_member* member, bool is_last) {};
};
typedef zce::SmartPtr<zdl_visitor> zdl_visitor_ptr;

#endif /*__zdl_visitor_h__*/
