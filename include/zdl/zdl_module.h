/* ***************************************************************
 *  Copyright (C) 2006 -  Yongming Wang(wangym@gmail.com)
 *  All Rights Reserved
 * ***************************************************************/
#pragma once

#include <zce/zce_object.h>
#include <zdl/zdl_type.h>
#include <map>

class zdl_type;
class zdl_visitor;

class zdl_module : public zce_object
{
    std::string namespace_name_;
    std::map<std::string, zce_smartptr<zdl_type>> type_dict_;
    std::vector<zce_smartptr<zdl_type>> type_vec_;

public:
    zdl_module(const std::string& ns);

    const std::map<std::string, zce_smartptr<zdl_type>>& type_dict() const noexcept {
        return type_dict_;
    }

    const std::string& name_space() const noexcept {
        return namespace_name_;
    }

    zce_smartptr<zdl_type> create_type(const std::string& name, zdl_type::zdl_type_e e);

    zce_smartptr<zdl_type> get_builtin_type(int tpid) const;

    zce_smartptr<zdl_type> get_type(const std::string& name) const;

    int add_type(const zce_smartptr<zdl_type>& type_ptr);

    void visit(const zce_smartptr<zdl_visitor>& visitor) const;

    void visit(const zce_smartptr<zdl_visitor>& visitor, const std::string& meta, bool include) const;

    void visit_type_meta(const zce_smartptr<zdl_visitor>& visitor, const std::string& meta, bool include) const;
};

typedef zce_smartptr<zdl_module> zdl_module_ptr;
