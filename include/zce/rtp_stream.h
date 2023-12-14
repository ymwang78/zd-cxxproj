#pragma once
#include <zce/zce_handler.h>

static const int RTP_STREAM_HEADLEN = 4;

class ZCE_API rtp_stream : public zce_istream
{
    zce_dblock dblock_;

public:

    rtp_stream();

    ~rtp_stream();

    virtual void on_read(const zce_dblock& dblock_ptr, const zce_any&);

    virtual int write(const zce_dblock& dblock_ptr, zce_istream::ERV_ISTREAM_WRITEOPT opt);
};
