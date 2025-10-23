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
#include <zce/zce_object.h>
#include <zce/zce_task.h>
#include <zce/zce_types.h>
#include <zce/zce_dblock.h>
#include <zce/zce_any.h>
#include <string>
#include <deque>
#include <map>

//////////////////////////////////////////////////////////////////////////

bool ZCE_API operator<(const zce_sockaddr_t& s1, const zce_sockaddr_t& s2);

bool ZCE_API operator<(const zce_addr_t& s1, const zce_addr_t& s2);

//////////////////////////////////////////////////////////////////////////

namespace zce {

class Reactor;
class RefBlock;
class Timer;

class ZCE_API IStream : virtual public zce::Object {
  protected:
    zce::SmartPtr<IStream> prev_;

    zce::SmartPtr<IStream> next_;

  public:
    enum ERV_ISTREAM_WRITEOPT {

        ERV_ISTREAM_DEFAULT,

        ERV_ISTREAM_PRIORITY_LOWP = 1,  // 最低可丢弃等级，比如P帧
        ERV_ISTREAM_PRIORITY_LOWI,      // 较高可丢弃等级，比如I帧
        ERV_ISTREAM_PRIORITY_LOWA,      // 最高可丢弃等级，音频
        ERV_ISTREAM_PRIORITY_STD,       // 正常等级，不可丢弃
        ERV_ISTREAM_PRIORITY_HIGH,      // 优先可插队发送

        ERV_ISTREAM_PRIORITY_MASK = 0x7,

        ERV_ISTREAM_WITHSOCKADDR = 0x10,  // 包前面带了目标地址

        ERV_ISTREAM_REQUESTCLOSE = 0x20,
    };

    const zce::SmartPtr<IStream>& prev() { return prev_; }

    void prev(const zce::SmartPtr<IStream>& v) { prev_ = v; }

    const zce::SmartPtr<IStream>& next() { return next_; }

    void next(const zce::SmartPtr<IStream>& v) { next_ = v; }

    void link(const zce::SmartPtr<IStream>& v) {
        next_ = v;
        if (v) {
            v->prev(zce::SmartPtr<IStream>(this));
        }
    }

    virtual void on_open(bool passive, const zce_sockaddr_t& remote);

    virtual void on_read(zce::RefBlock& dblock, const zce::Any&);

    virtual void on_close();

    virtual int write(zce::RefBlock& dblock, ERV_ISTREAM_WRITEOPT opt = ERV_ISTREAM_DEFAULT);

    virtual void close();

    const zce_sockaddr_t& get_remote_addr() const;
};

class ZCE_API Socket : public IStream {
    ZCE_OBJECT_DECLARE;

  protected:
    struct data_item {
        zce::RefBlock dblock_ptr_;
        ERV_ISTREAM_WRITEOPT option_;

        data_item();

        data_item(const zce::RefBlock& dblock, ERV_ISTREAM_WRITEOPT opt);
    };

    struct data_item_isless {
        ERV_ISTREAM_WRITEOPT opt_;
        data_item_isless(ERV_ISTREAM_WRITEOPT opt) : opt_(opt) {};

        bool operator()(const data_item& rhs) const { return rhs.option_ < opt_; }
    };

    zce::AtomicLong onclose_refcnt_;

    zce::AtomicLong close_refcnt_;

    zce::AtomicLong read_refcnt_;

    zce::SmartPtr<zce::Reactor> reactor_;

    zce::RefBlock read_dblock_;

    zce_sockaddr_t local_addr_;

    zce_sockaddr_t remote_addr_;

    unsigned suggest_size_;

    unsigned preserved_size_;

  protected:
    Socket(const zce::SmartPtr<zce::Reactor>& reactor_ptr, int preserved_size);

    void do_close();

    void do_close_delegate();

    virtual int do_write(zce::RefBlock& dblock_ptr, const zce_sockaddr_t* addr,
                         ERV_ISTREAM_WRITEOPT opt) = 0;

  public:
    virtual void on_open(bool passive, const zce_sockaddr_t& remote);

    virtual void close();

    virtual int write(zce::RefBlock& dblock_ptr, ERV_ISTREAM_WRITEOPT opt);

    virtual void* handle() const = 0;

    const zce::SmartPtr<zce::Reactor>& reactor() const { return reactor_; }

    void alloc_writebuf(unsigned suggest_size, zce_byte*& buf, zce_uint32& len);

    const zce_sockaddr_t& remote_addr() const { return remote_addr_; };

    virtual int get_local_addr(zce_sockaddr_t&) const = 0;

    void suggest_size(unsigned v) { suggest_size_ = v; };
};

class ZCE_API Udp : public Socket {
    struct pimpl;
    struct pimpl* pimpl_;

  protected:
    bool bopened_;

    bool bconnected_;

    int do_listen();

  public:
    static const int SOCKADDR_MAGIC = 0x19781217;

    Udp(const zce::SmartPtr<zce::Reactor>& reactor,
        int preserved_size = 6 +
                             sizeof(zce_sockaddr_t));  // default rtp, tcp head 4 + serveroverhead 2
                                                       // + sizeof(zce_sockaddr_t)

    ~Udp();

    virtual void* handle() const;

    virtual int do_write(zce::RefBlock& dblock_ptr, const zce_sockaddr_t* addr,
                         ERV_ISTREAM_WRITEOPT opt);

    virtual void on_read_data(zce_byte*, zce_uint32, const zce_sockaddr_t*);

    virtual void on_written(void* req, int status);

    virtual void on_close();

    virtual int write(zce::RefBlock& dblock_ptr, ERV_ISTREAM_WRITEOPT opt);

    int write(zce::RefBlock& dblock_ptr, const zce_sockaddr_t* addr, ERV_ISTREAM_WRITEOPT opt);

    int listen(const char* addr, unsigned short port);

    int connect(const char* addr, unsigned short port);

    int connect(const zce_sockaddr_t* addr);

    virtual int get_local_addr(zce_sockaddr_t& addr) const;
};

//////////////////////////////////////////////////////////////////////////

class ZCE_API Tcp : public Socket {
    struct pimpl;
    struct pimpl* pimpl_;

  protected:
    zce::Mutex dblock_lock_;

    std::deque<data_item> dblock_deque_;

    zce::SmartPtr<Timer> timer_ptr_;

    unsigned last_recv_tick_;

    unsigned working_priority_count_;

    ERV_ISTREAM_WRITEOPT working_priority_;

    volatile bool writing_;

  protected:
    int start_read();

    int start_write();  //<0 error; ==0, empty; > 0, write ret

    void do_priority();  // run inside dblock_lock_

    virtual int do_write(zce::RefBlock& dblock_ptr, const zce_sockaddr_t* addr,
                         ERV_ISTREAM_WRITEOPT opt);

  public:
    Tcp(const zce::SmartPtr<zce::Reactor>& reactor, int preserved_size = 16);

    ~Tcp();

    void handle_timeout(const zce::Any&);

    virtual void* handle() const;

    //////////////////////////////////////////////////////////////////////////
    virtual void close();

    virtual void on_open(bool passive, const zce_sockaddr_t& remote);

    virtual void on_close();

    //////////////////////////////////////////////////////////////////////////

    int connect(const char* addr, unsigned short port);

    virtual void on_written(void* req, int status);

    virtual void on_read_data(zce_byte*, zce_uint32);

    virtual int get_local_addr(zce_sockaddr_t& addr) const;
};

class ZCE_API DnsResolve : virtual public zce::Object {
    ZCE_OBJECT_DECLARE;
    zce::SmartPtr<zce::Reactor> reactor_ptr_;
    std::string domain_;

  public:
    DnsResolve(const zce::SmartPtr<zce::Reactor>& reactor, const char* domain);

    const zce::SmartPtr<zce::Reactor>& reactor() { return reactor_ptr_; }

    int start_resolve();

    virtual void on_resolved(int errcode, const zce_sockaddr_t& addr) = 0;
};

class ZCE_API Connector : public zce::Object {
    ZCE_OBJECT_DECLARE;

    struct pimpl;
    struct pimpl* pimpl_;

    zce::SmartPtr<zce::Tcp> tcp_ptr_;

    std::string remote_ip_;

    unsigned short remote_port_;

    unsigned short timeout_sec_;

    zce::SmartPtr<zce::Timer> timer_ptr_;

    int _prepare_connect();

    int _do_connect(zce_sockaddr_t& addr_in);

  public:
    Connector(const zce::SmartPtr<zce::Tcp>& tcp_ptr, const std::string& ip, unsigned short port,
              unsigned short timeoutsec = 0);

    ~Connector();

    void* handle() const;

    int start_connect();

    void handle_timeout(const zce::Any&);

    void on_resolved(int errcode, const zce_sockaddr_t& addr);

    virtual void on_connect(int status);

    const zce::SmartPtr<zce::Reactor>& reactor() { return tcp_ptr_->reactor(); }
};

class ZCE_API Acceptor : public zce::Object {
    struct pimpl;
    struct pimpl* pimpl_;

  protected:
    zce::SmartPtr<zce::Reactor> reactor_ptr_;

    std::function<zce::Tcp*()> make_handler_;

    std::string local_ip_;

    unsigned short local_port_;

    bool isclose_;

    zce::AtomicLong block_count_;
    zce::MutexReadWrite block_lock_;
    struct block_t {
        unsigned start_timet;
        unsigned end_timet;
        std::string reason;
    };
    std::map<zce_sockaddr_t, block_t> block_dict_;

    int do_listen(std::function<void(unsigned short port)> listen_start_cb);

    void do_close();

  public:
    Acceptor(const zce::SmartPtr<zce::Reactor>& reactor, std::function<zce::Tcp*()> make_handler);

    ~Acceptor();

    const zce::SmartPtr<zce::Reactor>& reactor() { return reactor_ptr_; }

    int listen(const char* localip, unsigned short port, std::function<void(unsigned short port)> listen_start_cb = nullptr);

    void close();

    unsigned local_port() const { return local_port_; }

    void on_connect(int status);

    void on_close();

    void block_remote(const zce_sockaddr_t& remote, unsigned end_timet, const std::string& reason);

    void unblock_remote(const zce_sockaddr_t& remote);
};

class ZCE_API Pipe : public Socket {
    struct Pimpl;
    struct Pimpl* pimpl_;

  protected:
    int start_read();
    int start_write();  //<0 error; ==0, empty; > 0, write ret

    virtual int do_write(zce::RefBlock& dblock_ptr, const zce_sockaddr_t* addr,
                         ERV_ISTREAM_WRITEOPT opt);

  public:
    Pipe(const zce::SmartPtr<zce::Reactor>& reactor, int preserved_size = 16);
    ~Pipe();

    virtual void* handle() const;

    virtual void on_open(bool passive, const zce_sockaddr_t& remote);
    virtual void on_close();
    virtual void on_written(void* req, int status);
    virtual void on_read_data(zce_byte*, zce_uint32);

    // 对于Pipe，此函数获取的是管道路径名，而非sockaddr
    virtual int get_local_addr(char* buffer, size_t* size) const;
    // Socket的虚函数仍然需要实现，但对于Pipe意义不大
    virtual int get_local_addr(zce_sockaddr_t& addr) const;
};

class ZCE_API PipeConnector : public zce::Object {
    ZCE_OBJECT_DECLARE;

    struct Pimpl;
    struct Pimpl* pimpl_;

  public:
    PipeConnector(const zce::SmartPtr<zce::Pipe>& pipe_ptr, std::string name);

    ~PipeConnector();

    int start_connect();

    void on_connect(int status);
    // const zce::SmartPtr<zce::Reactor>& reactor() { return pipe_ptr_->reactor(); }
};

class ZCE_API PipeAcceptor : public zce::Object {
    struct Pimpl;
    struct Pimpl* pimpl_;

  public:
    using MakeHandlerFunc = std::function<Pipe*()>;

    PipeAcceptor(const zce::SmartPtr<zce::Reactor>& reactor, MakeHandlerFunc make_handler,
                 bool ipc = false);

    ~PipeAcceptor();

    const zce::SmartPtr<zce::Reactor>& reactor();

    int listen(const char* name);

    void close();

    void on_connect(int status);

    void on_close();
};

class TaskQueue;

class ZCE_API SyncStream : public IStream {
    ZCE_OBJECT_DECLARE;

  protected:
    zce::SmartPtr<zce::TaskQueue> taskdeque_ptr_;

    zce::SmartPtr<zce::Reactor> reactor_ptr_;

  public:
    SyncStream(const zce::SmartPtr<zce::TaskQueue>& taskdeque,
               const zce::SmartPtr<zce::Reactor>& reactor);

    ~SyncStream();

    const zce::SmartPtr<zce::TaskQueue>& queue_ptr() const { return taskdeque_ptr_; }

    virtual int do_match_queue(zce::SmartPtr<zce::TaskQueue>&, const zce::RefBlock& dblock,
                               const zce::Any& ctx) {
        return 0;
    };

    virtual void on_open(bool passive, const zce_sockaddr_t& remote);

    virtual void on_read(zce::RefBlock& dblock, const zce::Any&);

    virtual void on_close();

    virtual int write(zce::RefBlock& dblock, IStream::ERV_ISTREAM_WRITEOPT opt);

    virtual void close();
};

class SocksStream : public IStream {
    std::string user_;

    std::string pass_;

    std::string remote_ip_;

    zce::RefBlock dblock_;

    zce_uint16 remote_port_ = 0;

    enum { STATE_BEGIN, STATE_AUTH, STATE_CONNECT } state_ = STATE_BEGIN;

    void proc_begin_ack(zce::RefBlock& dblock, const zce::Any& ctx);

    void proc_auth_ack(zce::RefBlock& dblock, const zce::Any& ctx);

    void send_cmd_connect();

  public:
    SocksStream(const std::string& realip, zce_uint16 realport, const std::string& user,
                const std::string& pass);

    virtual void on_open(bool passive, const zce_sockaddr_t& remote);

    virtual void on_read(zce::RefBlock& dblock, const zce::Any&);
};

}  // namespace zce
