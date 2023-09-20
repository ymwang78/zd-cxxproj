#pragma once
#include <zce/zce_handler.h>

static const int RTP_STREAM_HEADLEN = 4;

class ZCE_API rtp_stream : public zce_istream
{
public:

    rtp_stream();

    ~rtp_stream();

    virtual void on_read(zce_dblock& dblock_ptr, void*);

    virtual int write(const zce_dblock& dblock_ptr, zce_istream::ERV_ISTREAM_WRITEOPT opt);
};
