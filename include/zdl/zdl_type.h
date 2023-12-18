/* ***************************************************************
 *  Copyright (C) 2006 -  Yongming Wang(wangym@gmail.com)
 *  All Rights Reserved
 * ***************************************************************/
#pragma once

#include <zce/zce_object.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

class zdl_visitor;
class zdl_module;

class meta_base : public zce_object
{

public:
    virtual ~meta_base() {}

    const std::vector<std::pair<std::string, std::string> >& vec_metas() const
    {
        return vec_metas_;
    }

    const char* fetch_meta(const std::string& key, const char* def = NULL) const
    {
        std::vector<std::pair<std::string, std::string> >::const_iterator iter = std::find_if(
            vec_metas_.begin(), vec_metas_.end(), [key](auto iter) {
                return iter.first == key;
            });
        if (iter == vec_metas_.end())
            return def;
        return iter->second.c_str();
    }

    void vec_metas(const std::vector<std::pair<std::string, std::string> >& val) 
    {
        vec_metas_ = val;
        vec_metas_added();
    }

    const std::vector<std::pair<std::string, std::string> >& vec_metas() { return vec_metas_; }

    virtual void vec_metas_added() {};

protected:
    std::vector<std::pair<std::string, std::string> > vec_metas_;
};

class zdl_type : public meta_base
{
protected:
    typedef zce_smartptr<zdl_visitor> zdl_visitor_ptr;
public:

    enum zdl_type_e {
        type_unknow_e,
        type_builtin_e,
        type_builtinvec_e,
        type_enum_e,
        type_struct_e,
    };
    static zdl_type_e get_type_e (int tpid);
    
public:
    //tpid : UIDL_CHAR ,UIDL_UCHAR... @TODO CHECK CONV
    zdl_type(int tpid, const zce_smartptr<zdl_module>& module_ptr, const std::string& name);

    ~zdl_type();

    const std::string& name() const{
        return type_name_;
    };

    const std::string& full_name() const {
        return full_name_;
    }

    const zce_smartptr<zdl_module>& module_ptr() const {
        return module_ptr_;
    }

    int id() const{
        return type_id_;
    };
    zdl_type_e type_e() const{
        return get_type_e(type_id_);
    };

    bool is_struct() const {
        return type_e() == type_struct_e;
    }

    virtual void visit(const zdl_visitor_ptr&) const = 0;
    virtual void visit(const zdl_visitor_ptr&, const std::string&, bool include = true) const {};
protected:
    int         type_id_;
    zce_smartptr<zdl_module> module_ptr_;
    std::string type_name_;
    std::string full_name_;
};
typedef zce_smartptr<zdl_type> zdl_type_ptr;
