// ***************************************************************
//  ptp_proto   version:  1.0                         
//  ------------------------------------------------------------ 
//  This file was generated by vxgen.exe, don't manually modify.   
//  Hangzhou Zhidu Network Ltd.  (admin@zhidu.biz)               
//  Copyright (C) 2013-2016 - All Rights Reserved
// *************************************************************** 

#pragma once
#ifndef _ptp_proto_h_
#define _ptp_proto_h_

#include <zce/zce_config.h>
#include <zce/zce_smartptr.h>
#include <zce/zce_types.h>

namespace ptp
{
    enum CHNL_TYPE_E
    {
        CHNL_TYPE_PROXY = 0x00000000,
        CHNL_TYPE_STUB = 0x00000001,
        CHNL_TYPE_PTP = 0x00000002,
    };

    struct chnl_actreq_t
    {

        chnl_actreq_t()
        :rev(0)
        ,stubmask(0)
        ,payload(0)
        {
        }

        zce_byte    rev/*0-1: rev; 2-4:type;*/;
        zce_astring srckey;
        std::vector<zce_uint32> stubmask/*stub netmask*/;
        std::vector<zce_byte> payload;
    };
    struct chnl_actack_t
    {

        chnl_actack_t()
        :rev(0)
        ,actq(0)
        {
        }

        zce_byte    rev/*0-1: rev; 2-4:type;*/;
        zce_byte    actq/*bit=>ACTP*/;
    };
    struct strm_connreq_t
    {

        strm_connreq_t()
        :rev(0)
        ,translayer(0)
        ,servport(0)
        ,proxyssrc(0)
        {
        }

        zce_byte    rev/*0-1: rev;*/;
        zce_byte    translayer;
        zce_uint16    servport;
        zce_uint32    proxyssrc;
        zce_astring servaddr;
        zce_astring clieaddr;
    };
    struct strm_connack_t
    {

        strm_connack_t()
        :rev(0)
        ,breserv(0)
        ,reserv(0)
        ,stubssrc(0)
        {
        }

        zce_byte    rev/*0-1: rev;*/;
        zce_byte    breserv;
        zce_uint16    reserv;
        zce_uint32    stubssrc;
    };
    struct task_testconn_t
    {

        task_testconn_t()
        :rev(0)
        ,port(0)
        ,maxconn(0)
        ,periodsec(0)
        ,breserv(0)
        ,payload(0)
        {
        }

        zce_byte    rev/*0-1: rev;*/;
        zce_astring addr;
        zce_uint16    port;
        zce_uint16    maxconn;
        zce_uint16    periodsec;
        zce_byte    breserv;
        std::vector<zce_byte> payload;
    };
} //namespace ptp
#endif //_ptp_proto_h_
