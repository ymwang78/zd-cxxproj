#pragma once
// ***************************************************************
//  whp_stream   version:  1.0   -  date: 2003/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <zce/zce_handler.h>
#include <zce/zdp_schema.h>
#include <zce/zce_timer.h>

class zce_reactor;

class ZCE_API whp_stream : public zce::IStream {
    zce_smartptr<zce::Reactor> reactor_ptr_;

    zce::RefBlock dblock_;

    unsigned char m_cbSendRound;

    unsigned char m_cbRecvRound;

    unsigned m_dwSendXorKey;

    unsigned m_dwRecvXorKey;

    unsigned m_dwSendPacketCount;

    int encrypt(zce::RefBlock& dblock_ptr);

    int CrevasseBuffer(unsigned char checkcode, zce::RefBlock& dblock_ptr);

    unsigned short SeedRandMap(unsigned short wSeed);

    unsigned char MapSendByte(unsigned char cbData);

    unsigned char MapRecvByte(unsigned char cbData);

    int proc_next(zce::RefBlock& dblock_ptr, const zce::Any& ctx);

  public:
    whp_stream(const zce_smartptr<zce::Reactor>& reactor_ptr, unsigned preserv = 0);

    int write(zce::RefBlock& dblock_ptr, ERV_ISTREAM_WRITEOPT opt) override;

    void on_read(zce::RefBlock& dblock, const zce::Any&) override;
};

//////////////////////////////////////////////////////////////////////////

class ZCE_API idip_stream : public zce::IStream {
    zce_smartptr<zce::Reactor> reactor_ptr_;

    zce::RefBlock dblock_;

  public:
    idip_stream(const zce_smartptr<zce::Reactor>& reactor_ptr, unsigned preserv = 0);

    // virtual int write(zce::RefBlock& dblock_ptr, ERV_ISTREAM_WRITEOPT opt);

    void on_read(zce::RefBlock& dblock, const zce::Any&) override;
};