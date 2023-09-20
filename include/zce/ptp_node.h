// ***************************************************************
//  ptp_proxy   version:  1.0   -  date: 5/19/2016
//  -------------------------------------------------------------
//  Yongming Wang(wym@zhidu.biz)
//  Zhidu Network Co. Ltd.
//  -------------------------------------------------------------
//  Copyright (C) 2015 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once

#include <zce/zce_smartptr.h>
#include <zce/zce_handler.h>
#include <zce/zce_singleton.h>
#include <zce/zce_timer.h>
#include <map>

class zce_reactor;
class ptp_stream;
class ptp_channel;
class ptp_relay;
class ptp_udp;
class ptp_acceptor;

struct ptp_virtchnl;

namespace ptp {
    struct chnl_actreq_t;

    struct strm_connreq_t;
};


class ZCE_API ptp_node : public zce_istream, public zce_timer_doozer
{
    zce_smartptr<zce_reactor> reactor_ptr_;

    unsigned channel_id_;

    unsigned timer_cnt_;

    zce_smartptr<ptp_udp> ptproot_ptr_;

    zce_smartptr<ptp_relay> replay_ptr_;

    zce_smartptr<zce_timer> timer_;

    std::vector<unsigned> stubmask_;

    std::map<unsigned, zce_smartptr<ptp_acceptor> > id_acceptor_dict_;

    std::map<zce_addr_t, zce_smartptr<ptp_channel> > channel_dict_;

    std::map<zce_uint64, zce_smartptr<ptp_stream> > myfullid_stream_dict_; //channelid+streamid

    std::map<zce_uint64, zce_smartptr<ptp_stream> > peerfullid_stream_dict_; //only store stub side

    friend class zce_addgate_task;

    int do_addchannel(const char* localip, unsigned localport,
        const char* gateip, unsigned short gateport,
        unsigned traslayer);

    friend class zce_addproxy_task;

    int do_addproxy(unsigned id, unsigned short listenport,
        const char* remoteip, unsigned short remoteport,
        const char* jsoncfg);
public:

    ptp_node();

    ~ptp_node();

    const zce_smartptr<zce_reactor>& reactor_ptr() {
        return reactor_ptr_;
    }

    unsigned channel_id() const {
        return channel_id_;
    }

    bool is_relay() const {
        return replay_ptr_ != 0;
    }

    const std::vector<unsigned>& stubmask() const {
        return stubmask_;
    }

    int init(const zce_smartptr<zce_reactor>& reactor_ptr,
        unsigned relay_idx, const std::vector<unsigned>&);

    int add_channel(const char* localip, unsigned localport,
        const char* gateip, unsigned short gateport, unsigned traslayer);

    int add_proxy(unsigned id, unsigned short listenport,
        const char* remoteip, unsigned short remoteport,
        const char* jsoncfg);

    int on_connect_stub(ptp_stream* stream);

    int on_relay(ptp_channel* chnlptr, const zce_dblock& dblock, zce_sockaddr_t* addr);

    int on_idack(unsigned id);

    int on_stream_connreq(unsigned peer_channelid, unsigned peer_ssrc, ptp::strm_connreq_t* conn_t);

    virtual void handle_timeout(const void* arg);

    virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

    virtual void on_read(zce_dblock& dblock, void*);

    virtual void on_close();

    virtual int  write(const zce_dblock& dblock, zce_istream::ERV_ISTREAM_WRITEOPT opt);

    virtual void close();
};
