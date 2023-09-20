// ***************************************************************
//  zxml_node_base   version:  1.0     date: 07/31/2002
//  -------------------------------------------------------------
//  Yongming Wang(ymwang@iipc.zju.edu.cn)
//  The Institute of System Engineering, Zhejiang University
//  -------------------------------------------------------------
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// return value(int) means characters
// size is characters, too. 
// ***************************************************************
#pragma once
#ifndef __zxml_node_base_h__
#define __zxml_node_base_h__

#include <zce/zce_config.h>
#include <string>
#include <map>
#include <vector>
#include <zce/zce_smartptr.h>

class ZCE_API zxml_proc
{
public:
    template<typename T, typename MSGT>
    void zxml_proc_packet_tpl(const zce_smartptr_mtbase_ptr& ctx, const zce_smartptr_mtbase_ptr& root)
    {
        zce_smartptr<MSGT> msgptr = zce_smartptr<MSGT>::__dynamic_cast(root);
        ((T*)this)->zxml_proc_packet(ctx, msgptr);
    }
};

class ZCE_API zxml_node_base_pack : public zce_smartptr_mtbase
{
public:
    zxml_node_base_pack() { };
    virtual ~zxml_node_base_pack(){};

public:
    //used for framework only
    virtual int generate_elem(char* buf, int size, const char* node_elem, bool subitem = false) const;
    virtual int generate(char* buf, int size, const char* node_elem = NULL) const = 0;

    int generate_elem_root(char* buf, int size, const char* node_elem) const;
    int generate_xml_header(char* buffer, int size) const;
    int generate_node_enclose(char* buffer, int size) const;
    int generate_node_header(char* buffer, int size, const char* name, bool sepa = false) const;
    int generate_node_tail(char* buffer, int size, const char* name,bool sepa = false) const;
};

class ZCE_API zxml_node_base_unpack : public zce_smartptr_mtbase
{
public:
    enum E_CHILD_STATUS
    {
        E_CHILD_NOTEXISTS = -1,
        E_CHILD_OK = 0,
        E_CHILD_IGNORE,
    };
    zxml_node_base_unpack(zxml_node_base_unpack* parent, const char* var_name, const std::string& var_id) 
        : parent_(parent), var_name_(var_name), var_id_(var_id) {};
    virtual ~zxml_node_base_unpack() {};
    virtual bool validate() = 0;
    virtual void validate_self() = 0;
    zxml_node_base_unpack* parent()  const  { return parent_;   }
    const std::string& var_name() const     { return var_name_; }
    const std::string& var_id() const       { return var_id_;   }

public:
    //used for framework only
    virtual int parser_attrs() = 0;
    virtual int query_child(zce_smartptr<zxml_node_base_unpack>& child_unpack_ptr, const char* name, const std::string& id = "") = 0;
    virtual int query_all_child(std::vector<zce_smartptr<zxml_node_base_unpack> >& vec_ptrs) = 0;
    virtual int get_attrs(std::map<std::string, std::string>& /*valmap*/) { return 0; }
    virtual bool match_name(const char*) { return true; }   
    void insert_attribute(const std::string& name, const std::string& value);
    void insert_attribute(const std::string& name, const char* value, size_t length);
    void attribute_map(const std::map<std::string, std::string>& attrs);
    int parser(char* buf, int size);
private:
    zxml_node_base_unpack* parent_;

protected:
    std::string var_name_;
    std::string var_id_;
    std::map<std::string, std::string> attrs_map_;
};

typedef zce_smartptr<zxml_node_base_unpack> zxml_node_base_unpack_ptr;

#define UXML_QUERY_CHILD_VECTOR_VALIDATE(TYPE_NAME, VAR_TYPE, VAR_NAME, ID_NAME) \
{\
    _val.VAR_NAME.push_back(TYPE_NAME());\
    child_unpack_ptr = (new _unpack_##TYPE_NAME(_val.VAR_NAME[_val.VAR_NAME.size()-1], this, &_validate._validate_##VAR_NAME, #VAR_NAME, "__")); \
    return zxml_node_base_unpack::E_CHILD_OK;\
}

#define UXML_QUERY_CHILD_VECTOR_NOVALIDATE(TYPE_NAME, VAR_TYPE, VAR_NAME, ID_NAME) \
{\
    _val.VAR_NAME.push_back(TYPE_NAME());\
    child_unpack_ptr = (new _unpack_##TYPE_NAME(_val.VAR_NAME[_val.VAR_NAME.size()-1], this, NULL, #VAR_NAME, "__")); \
    return zxml_node_base_unpack::E_CHILD_OK;\
}

#define UXML_SET_VALUE_LEXICAL_CAST(VAR_NAME, VAR_TYPE)\
        if (name == #VAR_NAME)\
        {\
            _val.VAR_NAME = zce_to_string<VAR_TYPE>(value);\
            return 0;\
        }

#define UXML_QUERY_ALL_CHILD_VECTOR(TYPE_NAME, VAR_TYPE, VAR_NAME, ID_NAME)\
    {\
        std::vector<TYPE_NAME>::iterator iter;\
        for (iter = _val.VAR_NAME.begin(); iter != _val.VAR_NAME.end(); ++iter)\
        {\
            children.push_back(zce_smartptr<zxml_node_base_unpack>(new _unpack_##TYPE_NAME(*iter, this, NULL, #VAR_NAME, std::string(""))));\
        }\
    }

#endif /*__zxml_node_base_h__*/
