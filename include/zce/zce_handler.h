#pragma once
// ***************************************************************
//  zce_handler   version:  1.0     date: 2002/06/22
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_handler_h__
#define __zce_handler_h__

#include <zce/zce_object.h>
#include <zce/zce_task.h>
#include <zce/zce_types.h>
#include <zce/zce_dblock.h>
#include <zce/zce_any.h>
#include <string>
#include <deque>
#include <map>

class zce_reactor;
class zce_dblock;
class zce_timer;

//////////////////////////////////////////////////////////////////////////

bool ZCE_API operator<(const zce_sockaddr_t & s1, const zce_sockaddr_t & s2);

bool ZCE_API operator<(const zce_addr_t & s1, const zce_addr_t & s2);

//////////////////////////////////////////////////////////////////////////

class ZCE_API zce_istream : virtual public zce_object
{
protected:

    zce_smartptr<zce_istream> prev_;

    zce_smartptr<zce_istream> next_;

public:
    enum ERV_ISTREAM_WRITEOPT {

        ERV_ISTREAM_DEFAULT,

        ERV_ISTREAM_PRIORITY_LOWP = 1, //最低可丢弃等级，比如P帧
        ERV_ISTREAM_PRIORITY_LOWI,     //较高可丢弃等级，比如I帧
        ERV_ISTREAM_PRIORITY_LOWA,     //最高可丢弃等级，音频 
        ERV_ISTREAM_PRIORITY_STD,      //正常等级，不可丢弃
        ERV_ISTREAM_PRIORITY_HIGH,     //优先可插队发送

        ERV_ISTREAM_PRIORITY_MASK = 0x7,

        ERV_ISTREAM_WITHSOCKADDR = 0x10,      //包前面带了目标地址

        ERV_ISTREAM_REQUESTCLOSE = 0x20,
    };

    const zce_smartptr<zce_istream>& prev() {
        return prev_;
    }

    void prev(const zce_smartptr<zce_istream>& v) {
        prev_ = v;
    }

    const zce_smartptr<zce_istream>& next() {
        return next_;
    }

    void next(const zce_smartptr<zce_istream>& v) {
        next_ = v;
    }

    void link(const zce_smartptr<zce_istream>& v) {
        next_ = v;
        if (v) {
            v->prev(zce_smartptr<zce_istream>(this));
        }
    }

    virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

    virtual void on_read(const zce_dblock& dblock, const zce_any&);

    virtual void on_close();

    virtual int  write(const zce_dblock& dblock, ERV_ISTREAM_WRITEOPT opt = ERV_ISTREAM_DEFAULT);

    virtual void close();

    const zce_sockaddr_t& get_remote_addr() const;
};

class ZCE_API zce_socket : public zce_istream
{
    ZCE_OBJECT_DECLARE;

protected:

    zce_atomic_long onclose_refcnt_;

    zce_atomic_long close_refcnt_;

    zce_atomic_long read_refcnt_;

    zce_smartptr<zce_reactor> reactor_;

    zce_dblock read_dblock_;

    zce_sockaddr_t local_addr_;

    zce_sockaddr_t remote_addr_;

    unsigned suggest_size_;

    unsigned preserved_size_;

protected:

    zce_socket(const zce_smartptr<zce_reactor>& reactor_ptr, int preserved_size);

    void do_close();

    void do_close_delegate();

    virtual int do_write(const zce_dblock& dblock_ptr, 
        const zce_sockaddr_t* addr, ERV_ISTREAM_WRITEOPT opt) = 0;

public:

    virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

    virtual void close();

    virtual int write(const zce_dblock& dblock_ptr, ERV_ISTREAM_WRITEOPT opt);

    virtual void* handle() const = 0;

    const zce_smartptr<zce_reactor>& reactor() const { return reactor_; }

    void alloc_writebuf(unsigned suggest_size, zce_byte*& buf, zce_uint32& len);

    const zce_sockaddr_t& remote_addr() const {
        return remote_addr_;
    };

    virtual int get_local_addr(zce_sockaddr_t&) const = 0;

	void suggest_size(unsigned v) { suggest_size_ = v; };
};

class ZCE_API zce_udp : public zce_socket
{
    struct pimpl;
    struct pimpl *pimpl_;
    
protected:

    bool bopened_;

    bool bconnected_;

    int do_listen();

public:
    static const int SOCKADDR_MAGIC = 0x19781217;

    zce_udp(const zce_smartptr<zce_reactor>& reactor, int preserved_size = 6 + sizeof(zce_sockaddr_t)); // default rtp, tcp head 4 + serveroverhead 2 + sizeof(zce_sockaddr_t)

    ~zce_udp();

    virtual void* handle() const;

    virtual int do_write(const zce_dblock& dblock_ptr,
        const zce_sockaddr_t* addr, ERV_ISTREAM_WRITEOPT opt);

    virtual void on_read_data(zce_byte*, zce_uint32, const zce_sockaddr_t*);

    virtual void on_written(void* req, int status);

    virtual void on_close();

    virtual int write(const zce_dblock& dblock_ptr, ERV_ISTREAM_WRITEOPT opt);

    int write(const zce_dblock& dblock_ptr, const zce_sockaddr_t* addr, ERV_ISTREAM_WRITEOPT opt);

    int listen(const char* addr, unsigned short port);

    int connect(const char* addr, unsigned short port);

    int connect(const zce_sockaddr_t* addr);

    virtual int get_local_addr(zce_sockaddr_t& addr) const;
};

//////////////////////////////////////////////////////////////////////////

class ZCE_API zce_tcp : public zce_socket
{
    struct pimpl;
    struct pimpl *pimpl_;

protected:
    struct data_item {
        zce_dblock dblock_ptr_;
        ERV_ISTREAM_WRITEOPT option_;

        data_item();

        data_item(const zce_dblock& dblock, ERV_ISTREAM_WRITEOPT opt);
    };

    struct data_item_isless {
        ERV_ISTREAM_WRITEOPT opt_;
        data_item_isless(ERV_ISTREAM_WRITEOPT opt) :opt_(opt) {
        };

        bool operator()(const data_item& rhs) const {
            return rhs.option_ < opt_;
        }
    };

    zce_mutex dblock_lock_;

    std::deque<data_item> dblock_deque_;

    zce_smartptr<zce_timer> timer_ptr_;

    unsigned last_recv_tick_;

    unsigned working_priority_count_;

    ERV_ISTREAM_WRITEOPT working_priority_;

    volatile bool writing_;

protected:

    int start_read();

    int start_write(); //<0 error; ==0, empty; > 0, write ret

    void do_priority(); //run inside dblock_lock_

    virtual int do_write(const zce_dblock& dblock_ptr,
        const zce_sockaddr_t* addr, ERV_ISTREAM_WRITEOPT opt);

public:

    zce_tcp(const zce_smartptr<zce_reactor>& reactor, int preserved_size = 16);

    ~zce_tcp();

    void handle_timeout(const zce_any&);

    virtual void* handle() const;

    //////////////////////////////////////////////////////////////////////////
    virtual void close();

    virtual void on_open(bool passive, const  char* peerip, zce_uint16 peerport);

    virtual void on_close();

    //////////////////////////////////////////////////////////////////////////

    int connect(const char* addr, unsigned short port);

    virtual void on_written(void* req, int status);

    virtual void on_read_data(zce_byte*, zce_uint32);

    virtual int get_local_addr(zce_sockaddr_t& addr) const;
};

class ZCE_API zce_dnsresolve : virtual public zce_object
{
    ZCE_OBJECT_DECLARE;
	zce_smartptr<zce_reactor> reactor_ptr_;
	std::string domain_;

public:
	zce_dnsresolve(const zce_smartptr<zce_reactor>& reactor, const char* domain);

	const zce_smartptr<zce_reactor>& reactor() { return reactor_ptr_; }

	int start_resolve();

	virtual void on_resolved(const zce_sockaddr_t& addr) = 0;
};

class ZCE_API zce_connector : public zce_object
{
    ZCE_OBJECT_DECLARE;

    struct pimpl;
    struct pimpl *pimpl_;

    zce_smartptr<zce_tcp> tcp_ptr_;

    std::string remote_ip_;

    unsigned short remote_port_;

    unsigned short timeout_sec_;

    zce_smartptr<zce_timer> timer_ptr_;

    int _prepare_connect();

	int _do_connect(zce_sockaddr_t& addr_in);

public:

    zce_connector(const zce_smartptr<zce_tcp>& tcp_ptr, const std::string& ip, unsigned short port, unsigned short timeoutsec = 0);

    ~zce_connector();

    void* handle() const;

    int start_connect();

    void handle_timeout(const zce_any&);

	void on_resolved(const zce_sockaddr_t& addr);

    virtual void on_connect(int status);

	const zce_smartptr<zce_reactor>& reactor() { return tcp_ptr_->reactor(); }
};

class ZCE_API zce_acceptor : public zce_object
{
    struct pimpl;
    struct pimpl *pimpl_;

protected:

    zce_smartptr<zce_reactor> reactor_ptr_;

    std::string local_ip_;

    unsigned short local_port_;

    bool isclose_;

    zce_atomic_long block_count_;
    zce_mutex_rw block_lock_;
    struct block_t { unsigned start_timet; unsigned end_timet; std::string reason; };
    std::map<zce_sockaddr_t, block_t> block_dict_;

    int do_listen();

    void do_close();

public:
    
    zce_acceptor(const zce_smartptr<zce_reactor>& reactor);

    ~zce_acceptor();

    const zce_smartptr<zce_reactor>& reactor() { return reactor_ptr_; }

    int listen(const char* localip, unsigned short port);

    void close();

    unsigned local_port() const { return local_port_; }

    void on_connect(int status);

    void on_close();

    void block_remote(const zce_sockaddr_t& remote, unsigned end_timet, const std::string& reason);

    void unblock_remote(const zce_sockaddr_t& remote);

    virtual zce_tcp* make_handler() = 0;
};

class zce_task_queue;
class zce_reactor;

class ZCE_API zce_sync_istream : public zce_istream
{
    ZCE_OBJECT_DECLARE;

protected:

    zce_smartptr<zce_task_queue> taskdeque_ptr_;

    zce_smartptr<zce_reactor> reactor_ptr_;

public:
    zce_sync_istream(const zce_smartptr<zce_task_queue>& taskdeque, 
        const zce_smartptr<zce_reactor>& reactor);

    ~zce_sync_istream();

    virtual int do_match_queue(zce_smartptr<zce_task_queue>&, const zce_dblock& dblock, const zce_any& ctx) { return 0; };

    virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

    virtual void on_read(const zce_dblock& dblock, const zce_any&);

    virtual void on_close();

    virtual int  write(const zce_dblock& dblock, zce_istream::ERV_ISTREAM_WRITEOPT opt);

    virtual void close();
};

class zce_proxy_socks : public zce_istream
{
    std::string user_;

    std::string pass_;

    std::string remote_ip_;

    zce_dblock dblock_;

    zce_uint16 remote_port_ = 0;

    enum { STATE_BEGIN, STATE_AUTH, STATE_CONNECT} state_ = STATE_BEGIN;

    void proc_begin_ack(zce_dblock& dblock, const zce_any& ctx);

    void proc_auth_ack(zce_dblock& dblock, const zce_any& ctx);

    void send_cmd_connect();

public:

    zce_proxy_socks(const std::string& realip, zce_uint16 realport, const std::string& user, const std::string& pass);

    virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

    virtual void on_read(const zce_dblock& dblock, const zce_any&);
};

#endif //__zce_handler_h__
