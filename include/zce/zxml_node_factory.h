#pragma once
// ***************************************************************
//  zxml_node_factory   version:  1.0   -  date: 2002/07/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zxml_node_factory_h__
#define __zxml_node_factory_h__

#include <zce/zce_config.h>
#include <map>
#include <zce/zce_object.h>

class zxml_node_base_unpack;

class ZCE_API zxml_node_factory
{
    struct Pimpl;
    Pimpl* pimpl_ptr_;
public:
    zxml_node_factory();
    virtual ~zxml_node_factory();
    
    virtual zce_smartptr<zxml_node_base_unpack> create_node(const std::string& name, zxml_node_base_unpack* parent, bool* validate_self, zce_object_ptr& node_ptr, unsigned& msgtype);
    virtual zce_smartptr<zxml_node_base_unpack> create_root_bysubitem(const std::string& elem_name, const std::string& elem_val, zce_object_ptr& node_ptr, unsigned& msgtype);

    typedef zxml_node_base_unpack* (*pfn_node_creator)(zxml_node_base_unpack* parent, bool* validate_self, zce_object_ptr& node_ptr, unsigned& msgtype);
    int registe_type(const std::string& node_name, pfn_node_creator lpfn_creator);
    pfn_node_creator find_type(const std::string& node_name);
    
    virtual const std::string& match_root_name(const std::string& node_name, const std::map<std::string, std::string>& attrs_map);
};

#endif /*__zxml_node_factory_h__*/
