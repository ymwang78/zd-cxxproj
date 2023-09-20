#pragma once

#include <zce/zce_smartptr.h>
#include <zce/zce_handler.h>
#include <zce/zce_object.h>

struct ZCE_API iface_stat
{
    zce_int64 ot_byte;
    zce_int64 in_byte;
    iface_stat() : ot_byte(0), in_byte(0) {
    };
};

class ZCE_API ptp_iface : public zce_smartptr_mtbase
{
    iface_stat last_stat_;

    iface_stat stat_;

public:

    ptp_iface();

    ~ptp_iface();

    inline void add_outbytes(zce_int64 bytes) { stat_.ot_byte += bytes; };

    inline void add_inbytes(zce_int64 bytes) { stat_.in_byte += bytes; };

    void reset_stat_1s();
};

//////////////////////////////////////////////////////////////////////////

class ZCE_API ptp_transport : virtual public zce_smartptr_mtbase
{
protected:

    zce_smartptr<ptp_iface> iface_ptr_;

public:

    virtual int open(const std::string& ip, unsigned short port, const std::string& remote_ip = "", unsigned short remote_port = 0) = 0;

    virtual zce_smartptr<zce_istream> istream_ptr() = 0;

    virtual void close() = 0;

};

//////////////////////////////////////////////////////////////////////////

class ZCE_API ptp_udp : public ptp_transport, public zce_udp
{
    ZCE_OBJECT_DECLARE;

public:

    ptp_udp(zce_reactor*);

    virtual ~ptp_udp();

    virtual void on_read_data(zce_byte* buf, zce_uint32 len, const zce_sockaddr_t* addr);

    virtual int open(const std::string& ip, unsigned short port,
        const std::string& remote_ip, unsigned short remote_port);

    virtual zce_smartptr<zce_istream> istream_ptr() {
        return zce_smartptr<zce_istream>(this);
    }

    virtual int write(const zce_dblock& dblockptr, ERV_ISTREAM_WRITEOPT opt);

    virtual void on_close();

    virtual void close();
};

//////////////////////////////////////////////////////////////////////////


class ZCE_API ptp_tcp : public ptp_transport, public zce_tcp
{
    ZCE_OBJECT_DECLARE;

protected:

    zce_smartptr<zce_acceptor> acceptor_ptr_;

    bool  is_support_frame_;

protected:

    virtual void close();

public:
    ptp_tcp(zce_reactor*);

    virtual ~ptp_tcp();

    virtual zce_smartptr<zce_istream> istream_ptr() {
        return zce_smartptr<zce_istream>(this);
    }

    virtual int open(const ::std::string& ip, unsigned short port, const std::string& remote_ip = "", unsigned short remote_port = 0);

    virtual void on_open(bool passive, const  char* peerip, zce_uint16 peerport);

    virtual void on_read(zce_dblock& dblock, void*);

    virtual int write(const zce_dblock& dblock, ERV_ISTREAM_WRITEOPT opt);
};
