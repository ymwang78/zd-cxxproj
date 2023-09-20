#pragma once
#include <zce/zce_handler.h>

struct ZCE_API rtmp_data : public zce_smartptr_mtbase
{
    zce_dblock datablock;
    unsigned int timestamp;
    int flag;
    char privatedata[256];
    int  privatedata_len;
};

class ZCE_API rtmp_stream : public zce_istream
{
    struct pimpl;

    zce_smartptr<pimpl> pimpl_;

public:

    rtmp_stream(const std::string& rtmpurl);

    ~rtmp_stream();

    void write_data(const zce_smartptr<rtmp_data>& dataptr);

    virtual void on_read(zce_dblock& dblock_ptr, void*);

    virtual void close();
};
