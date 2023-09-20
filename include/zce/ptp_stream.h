#pragma once

#include <zce/zce_handler.h>
#include <zce/zce_object.h>
class ptp_recvjitter;
class ptp_sendjitter;
class ptp_node;
class ptp_rawtcp;

class ZCE_API ptp_stream : public zce_istream
{
    ZCE_OBJECT_DECLARE;

    zce_smartptr<ptp_node> node_ptr_;

    zce_smartptr<ptp_recvjitter> recv_ptr_;

    zce_smartptr<ptp_sendjitter> send_ptr_;

    zce_smartptr<ptp_rawtcp> rawtcp_ptr_;

    std::string remote_ip_;

    unsigned short remote_port_;

    unsigned peer_chnlid_; //channleid

    unsigned peer_ssrc_; //streamid

    unsigned my_ssrc_;

    unsigned ctrl_seq_;

    unsigned last_recv_tick_;

    unsigned close_refcnt_;

    enum STREAM_E{
        STREAM_NONE,
        STREAM_CONNECTING,
        STREAM_CONNECTED,
        STREAM_CLOSING, //WAITING PEER TO VERIFY DATA ,THEN CLOSE
        STREAM_CLOSED,
    } stream_stat_;

    int handle_data(const zce_dblock& dblock, void*);

    int handle_ctrl(const zce_dblock& dblock, void*);

public:

    ptp_stream(const zce_smartptr<ptp_node>&,
        unsigned peer_channelid, unsigned peer_ssrc);

    ~ptp_stream();

    const zce_smartptr<ptp_node>& node_ptr() const {
        return node_ptr_;
    }

    const std::string& remote_ip() const {
        return remote_ip_;
    }

    unsigned short remote_port() const {
        return remote_port_;
    }

    unsigned my_ssrc() const {
        return my_ssrc_;
    }

    unsigned peer_channelid() const {
        return peer_chnlid_;
    }

    unsigned peer_ssrc() const {
        return peer_ssrc_;
    }

    zce_uint64 peer_fullid() const {
        return ((zce_uint64)peer_chnlid_) | peer_ssrc_;
    }

    bool isconnected() const {
        return stream_stat_ == STREAM_CONNECTED;
    }

    bool isclosed() const {
        return stream_stat_ == STREAM_CLOSED;
    }

    void rawtcp_ptr(ptp_rawtcp* v); 

    void on_timer();

    int connect_direct(const char* ip, unsigned short port);

    //connect via stub
    int connect(const char* ip, unsigned short port);

    int start_connect();

    int send_connack();

    int send_ctrl(unsigned short cmd, const zce_byte* payload, unsigned len);

    int on_read_rtpdata(zce_dblock& dblock, void* ctx);

    int on_read_rawdata(zce_dblock& dblock, void* ctx);

    //virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

    //virtual void on_read(zce_dblock& dblock, void*);

    virtual void on_close();

    virtual int  write(const zce_dblock& dblock, ERV_ISTREAM_WRITEOPT opt);

    virtual void close();
};
