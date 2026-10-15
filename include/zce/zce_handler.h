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
#include <zce/zce_timer.h>
#include <string>
#include <chrono>
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

/// Why a socket was closed, as reported to the layer above in on_close().
///
/// A bare libuv error code cannot carry all of these: CONNECT_TIMEOUT and
/// IDLE_TIMEOUT come from zce timers and have no code of their own, and
/// UV_ETIMEDOUT also arrives on the read path from a failed keepalive probe, so
/// it cannot stand in for a receive idle timeout.
enum class CloseCause : int {
    NONE = 0,         ///< not closed
    LOCAL,            ///< close() was called on this side
    PEER_EOF,         ///< the peer closed the connection
    IO_ERROR,         ///< a read or write failed; see close_error()
    CONNECT_FAILED,   ///< connect failed (refused, unreachable, DNS, bad address); see close_error()
    CONNECT_TIMEOUT,  ///< the Connector gave up waiting
    IDLE_TIMEOUT,     ///< the receive idle timer expired
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

    /// Set before on_close() is dispatched and never changed afterwards; the
    /// first cause recorded is the one reported.
    CloseCause close_cause_ = CloseCause::NONE;

    /// The libuv error behind close_cause_, 0 when there is none.
    int close_error_ = 0;

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

    /// Record why this socket is being closed.
    ///
    /// The first cause recorded wins: a path that closes a socket after an
    /// earlier one has already settled the reason (a late
    /// on_connect(UV_ECANCELED) after a connect timeout, for instance) must not
    /// overwrite it. Call it before close(), on the reactor thread; the layer
    /// above reads it in on_close().
    void set_close_cause(CloseCause cause, int error = 0);

    /// Why this socket was closed; NONE until it is.
    CloseCause close_cause() const { return close_cause_; }

    /// The libuv error code behind close_cause(), 0 when there is none.
    int close_error() const { return close_error_; }

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

    /// Turn TCP_NODELAY (Nagle's algorithm) on or off.
    ///
    /// Call it on the reactor thread. It may be called before connect(): the
    /// setting is kept and applied to the socket before on_open() is
    /// dispatched, so a connection observed in on_open() already carries it.
    /// Returns 0 on success, or the libuv error code.
    int set_nodelay(bool enable);

    /// Turn TCP keepalive on or off, and set how it probes.
    ///
    /// idle_sec is how long the connection must be idle before the first probe;
    /// interval_sec and count are the probe interval and the number of unacked
    /// probes that end the connection. 0 means "leave the system default".
    ///
    /// Call it on the reactor thread; it may be called before connect(), with
    /// the same deferral and guarantee as set_nodelay(). interval_sec and count
    /// need uv_tcp_keepalive_ex (libuv >= 1.52.0): with an older libuv this
    /// degrades to uv_tcp_keepalive(idle_sec) and returns UV_ENOTSUP when
    /// either is non-zero, so the caller learns it did not take effect.
    /// Returns 0 on success, or the libuv error code.
    int set_keepalive(bool enable, unsigned idle_sec, unsigned interval_sec = 0, unsigned count = 0);

    //////////////////////////////////////////////////////////////////////////

    /// How long connect(addr, port) waits for the connection to be established.
    static const unsigned short DEFAULT_CONNECT_TIMEOUT_SEC = 10;

    /// Connect to addr:port, giving up after DEFAULT_CONNECT_TIMEOUT_SEC.
    int connect(const char* addr, unsigned short port);

    /// Connect to addr:port, giving up after timeout_sec seconds.
    ///
    /// The time counts from the start of the attempt and covers resolving
    /// addr when it is a host name. A connection not established by then is
    /// abandoned the way a refused one is: the stream is closed and the layers
    /// above get on_close() without on_open(). It only bounds this socket's own
    /// connection; a handshake run over it afterwards is not covered. A TLS
    /// client (zce_ssl) and a SOCKS5 client (SocksStream) each give their
    /// handshake DEFAULT_CONNECT_TIMEOUT_SEC of its own.
    ///
    /// 0 leaves the wait to the operating system, which for a peer that never
    /// answers is about two minutes on Linux.
    int connect(const char* addr, unsigned short port, unsigned short timeout_sec);

    /// Connect to addr:port, giving up after timeout milliseconds.
    ///
    /// Identical to the timeout_sec overload in every other respect; it exists
    /// because a site that wants a 1.5 s verdict cannot ask for it in whole
    /// seconds. std::chrono::milliseconds is a distinct type on purpose, so
    /// connect(addr, port, 3) still means three seconds and is not read as
    /// three milliseconds. 0 waits for the operating system to give up, as the
    /// timeout_sec overload's 0 does; a negative value is read as 0, and one
    /// beyond UINT_MAX milliseconds saturates there rather than wrapping into a
    /// short deadline.
    int connect(const char* addr, unsigned short port, std::chrono::milliseconds timeout);

    virtual void on_written(void* req, int status);

    virtual void on_read_data(zce_byte*, zce_uint32);

    /// Stamp the receive clock that handle_timeout() measures the idle window
    /// against.
    ///
    /// Called from the libuv read callback BEFORE it dispatches to
    /// on_read_data(), because on_read_data() is virtual and every override in
    /// this tree replaces it outright rather than chaining to the base. The
    /// clock belongs to the socket, not to whatever a subclass does with the
    /// bytes, so an override must not be able to lose it: while it could, a
    /// connection carrying traffic every few milliseconds still looked idle and
    /// was closed on the 300s timer — every five minutes, for as long as it
    /// stayed up.
    void touch_recv_clock();

    /// Stop delivering on_read_data() without closing the connection, so the
    /// peer's TCP receive window closes and it stops sending.
    ///
    /// This is the only way to apply real backpressure to a source that
    /// produces faster than the reader can forward. Without it a relay can
    /// only buffer what it cannot yet pass on, and an unbounded buffer is not
    /// a policy but a deferred out-of-memory; a bounded one just turns the
    /// same overrun into a dropped connection.
    ///
    /// Both are idempotent and must be called on the reactor thread. Neither
    /// touches the read refcount that start_read()/on_close() maintain, so a
    /// paused handle keeps exactly the same lifetime as a reading one.
    int  pause_read();
    int  resume_read();

    /// How long a connection may go without receiving anything before it is
    /// closed as idle, in milliseconds; 0 turns the check off. Defaults to
    /// 300000, the 300 s the receive timer has always used.
    ///
    /// Call it on the reactor thread, before or after connect(). Setting it on
    /// a connected stream restarts the deadline from the last packet received.
    /// The close it causes reports CloseCause::IDLE_TIMEOUT, which tells it
    /// apart from the peer hanging up.
    ///
    /// The stream is closed within one timer tick after "last packet +
    /// timeout", not at the next multiple of the timeout: a packet received
    /// just after a check makes the next one due timeout milliseconds later,
    /// not up to one whole period later. While reading is paused the clock does
    /// not run, and resume_read() starts it again.
    void set_recv_idle_timeout(unsigned msec);

    /// The receive idle timeout in milliseconds; 0 when the check is off.
    unsigned recv_idle_timeout() const;
    bool read_paused() const { return read_paused_; }

    virtual int get_local_addr(zce_sockaddr_t& addr) const;

  protected:
    bool read_paused_ = false;
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
    /// timeoutsec bounds the connection attempt as Tcp::connect() describes;
    /// 0 sets no bound of its own.
    Connector(const zce::SmartPtr<zce::Tcp>& tcp_ptr, const std::string& ip, unsigned short port,
              unsigned short timeoutsec = 0);

    /// Bound this connection attempt by timeout milliseconds, replacing any
    /// bound the constructor or an earlier call set. 0 removes the bound; a
    /// negative value is read as 0, and one beyond UINT_MAX milliseconds (about
    /// 49.7 days) saturates there rather than wrapping into a short deadline.
    ///
    /// Call it before start_connect(); the deadline it sets runs from there and
    /// covers resolving the address as well as the handshake, exactly as the
    /// constructor's timeoutsec does.
    void set_timeout(std::chrono::milliseconds timeout);

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

    // Purge all block_dict_ entries whose end_timet is <= the current time.
    // Returns the number of entries removed. Safe to call at any time;
    // typically scheduled from an application level timer (e.g. 60s).
    unsigned purgeExpiredBlocks();

    // Current number of active (and possibly-expired-but-not-yet-purged) blocks.
    unsigned blockedCount() const { return (unsigned)block_count_.value(); }
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

    int open(int fd);

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

class ZCE_API Tty : public Socket {
    struct Pimpl;
    struct Pimpl* pimpl_;
  protected:
    int start_read();
    int do_write(zce::RefBlock&, const zce_sockaddr_t*,
                 zce::IStream::ERV_ISTREAM_WRITEOPT) override {
        return 0;
    };
  public:
    Tty(const zce::SmartPtr<zce::Reactor>& reactor, int preserved_size = 16);
    ~Tty();
    int open(int fd); 
    virtual void* handle() const;
    virtual void on_open(bool passive, const zce_sockaddr_t& remote);
    virtual void on_close();
    virtual void on_read_data(zce_byte*, zce_uint32);
    virtual int get_local_addr(zce_sockaddr_t& addr) const;
};

class ZCE_API Signal : public zce::Object {
    ZCE_OBJECT_DECLARE;
    struct Pimpl;
    struct Pimpl* pimpl_;
  public:
    Signal(const zce::SmartPtr<zce::Reactor>& reactor, int signum, std::function<void(int signum)> cb);
    ~Signal();
    int start();
    void close();
    virtual void onSignal(int signum);
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
        (void)dblock;
        (void)ctx;
        return 0;
    };

    virtual void on_open(bool passive, const zce_sockaddr_t& remote);

    virtual void on_read(zce::RefBlock& dblock, const zce::Any&);

    virtual void on_close();

    virtual int write(zce::RefBlock& dblock, IStream::ERV_ISTREAM_WRITEOPT opt);

    virtual void close();
};

/// The client side of a SOCKS5 handshake (RFC 1928, with RFC 1929 username/password), between a
/// connection to the proxy below and the stream above. The stream above gets on_open() once the
/// proxy has connected to realip:realport, and only then.
///
/// The proxy gets zce::Tcp::DEFAULT_CONNECT_TIMEOUT_SEC from on_open() to finish the whole
/// handshake, as long as Tcp::connect() gives the connection itself. A proxy that accepts the
/// connection but stops answering (a wrong service on the port, a stuck proxy, a CONNECT the
/// proxy never completes) is then hung up on as for a refused handshake: the stream above gets
/// on_close() and never on_open(). The deadline runs on the reactor of the socket below; with no
/// socket below there is none.
class SocksStream : public IStream {
    std::string user_;

    std::string pass_;

    std::string remote_ip_;

    zce::RefBlock dblock_;

    zce_uint16 remote_port_ = 0;

    enum { STATE_BEGIN, STATE_AUTH, STATE_CONNECT } state_ = STATE_BEGIN;

    // The deadline for the handshake, from on_open() until the stream above is opened or the
    // connection ends. Only the reactor thread starts or stops it.
    zce::SmartPtr<zce::Timer> handshake_timer_;

    void on_handshake_timeout();

    // Inline, as is on_close(): a libzce built before the deadline existed still links against
    // this header.
    void stop_handshake_timer() {
        if (handshake_timer_) {
            handshake_timer_->cancel();
            handshake_timer_ = 0;
        }
    }

    void proc_begin_ack(zce::RefBlock& dblock, const zce::Any& ctx);

    void proc_auth_ack(zce::RefBlock& dblock, const zce::Any& ctx);

    void send_cmd_connect();

  public:
    SocksStream(const std::string& realip, zce_uint16 realport, const std::string& user,
                const std::string& pass);

    virtual void on_open(bool passive, const zce_sockaddr_t& remote);

    virtual void on_read(zce::RefBlock& dblock, const zce::Any&);

    /// The connection below has ended; a handshake deadline still running goes with it.
    void on_close() override {
        stop_handshake_timer();
        IStream::on_close();
    }
};

}  // namespace zce
