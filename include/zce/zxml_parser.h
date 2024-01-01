#pragma once
// ***************************************************************
//  zxml_parser   version:  1.0   -  date: 2002/07/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zxml_parser_h__
#define __zxml_parser_h__

#include <zce/zce_config.h>
#include <zce/zce_object.h>

class zxml_node_factory;
class zxml_node_base_unpack;

class ZCE_API  zxml_parser
{
    typedef zce_smartptr<zxml_node_base_unpack> zxml_node_base_unpack_ptr;

    struct Pimpl;
    Pimpl* pimpl_ptr_;

public:
    zxml_parser();
    ~zxml_parser();

    //utf-8 buffer only
    int parser(zxml_node_base_unpack_ptr& root, zxml_node_factory& factory, 
        const char* buffer, int len, bool interrupt_if_missing = true, bool interrupt_if_dummy = false);

    zce_object_ptr root_node_ptr();
    unsigned msgtype();
};

#endif /*__zxml_parser_h__*/
