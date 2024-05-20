#pragma once
// ***************************************************************
//  rtp_stream   version:  1.0   -  date: 2004/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_handler.h>

static const int RTP_STREAM_HEADLEN = 4;

class ZCE_API rtp_stream : public zce_istream
{
    zce_dblock dblock_;

public:

    rtp_stream();

    ~rtp_stream();

    void on_read(zce_dblock& dblock_ptr, const zce_any&) override;

    int write(const zce_dblock& dblock_ptr, zce_istream::ERV_ISTREAM_WRITEOPT opt) override;
};
