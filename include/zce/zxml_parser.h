// ***************************************************************
//  zxml_parser   version:  1.0     date: 07/31/2002
//  -------------------------------------------------------------
//  Yongming Wang(ymwang@iipc.zju.edu.cn)
//  The Institute of System Engineering, Zhejiang University
//  -------------------------------------------------------------
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once
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
