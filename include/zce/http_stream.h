// ***************************************************************
//  http_stream   version:  1.0   -  date: 2003/03/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#pragma once

#include <zce/zce_config.h>
#include <zce/zce_handler.h>
#include <zce/zce_object.h>
#include <zce/text_stream.h>

#define HTTP_VERSION_BIND(x, y) (((x) << 16) | (y))
#define HTTP_VERSION_MAJOR(v) (((v) >> 16) & 0x0000ffff)
#define HTTP_VERSION_MINOR(v) ((v) & 0x0000ffff)

namespace zce {

enum HTTP_CGI_E {
    HTTP_CGI_STANDARD,
    HTTP_CGI_UWSGI,
};

std::string ZCE_API http_urlencode(const std::string& str);

std::string ZCE_API http_urldecode(const std::string& str);

std::string ZCE_API http_urldecode(const char* data, size_t len);

int ZCE_API http_getpath_pos(const std::string& uri);

int ZCE_API http_gzip(zce_byte* dest, size_t* destLen, const zce_byte* source, size_t sourceLen,
                      int level);

///////////////////////////////////////////////////////////////////////////////

struct ZCE_API ZCE_HTTP_HEADER : public TEXT_HEADER {
  protected:
    unsigned unpack_offset_;
    bool unpack_finished_;

    virtual int parse_first_line(const char* buf, int size) = 0;

    int parse_param(const char* buf, int size);

    // int decode_uwsgi(const char* buf, int size);

  public:
    ZCE_HTTP_HEADER(TEXT_HEADER::TEXT_HEADER_E header_type);

    bool is_unpack_finished() const { return unpack_finished_; }

    int unpack(HTTP_CGI_E cgi, const zce_byte* buf, int size);

    virtual int pack(char* buf, int size) const = 0;

    const std::string& get_x_forward_for() const;
};

///////////////////////////////////////////////////////////////////////////////

struct ZCE_API ZCE_HTTP_REQUEST : public ZCE_HTTP_HEADER {
    enum METHOD_E {
        METHOD_UNKNOW,
        METHOD_GET,
        METHOD_POST,
        METHOD_PUT,
        METHOD_DELETE,
        METHOD_OPTIONS,
        METHOD_HEAD,
        METHOD_TRACE,
        METHOD_LIMIT
    };

    ZCE_HTTP_REQUEST()
        : ZCE_HTTP_HEADER(TEXT_HEADER_REQUEST), method_(METHOD_UNKNOW), version_(0) {};
    ZCE_HTTP_REQUEST(const std::string& url, METHOD_E method,
                     unsigned version = HTTP_VERSION_BIND(1, 1));
    static const char* method_to_name(METHOD_E e);
    static METHOD_E name_to_method(const char*);
    std::string url_;
    METHOD_E method_;
    unsigned version_;

    virtual int pack(char* buf, int size) const;
    const std::string& uri() const { return url_; }
    METHOD_E method() { return method_; }

  private:
    virtual int parse_first_line(const char* buf, int size);

    int parse_request_uri(const char* buf, int size);
};

///////////////////////////////////////////////////////////////////////////////

struct ZCE_API ZCE_HTTP_RESPONSE : public ZCE_HTTP_HEADER {
    ZCE_HTTP_RESPONSE() : ZCE_HTTP_HEADER(TEXT_HEADER_RESPONSE) {};
    ZCE_HTTP_RESPONSE(const std::string& str, unsigned code = 200,
                      unsigned version = HTTP_VERSION_BIND(1, 1));
    unsigned result_code_;
    std::string result_string_;
    unsigned version_;

    virtual int pack(char* buf, int size) const;

    void set_status(unsigned code, const std::string& status_str) {
        result_code_ = code;
        result_string_ = status_str;
    }

    void set_header(const std::string& key, const std::string& value) {
        return exparam(key, value);
    }

    int encode(RefBlock& dblock) const;

  private:
    virtual int parse_first_line(const char* buf, int size);

    int parse_status_code(const char* buf, int size);
};

///////////////////////////////////////////////////////////////////////////////

class ZCE_API HttpStream : public IStream {
  public:
    /**
     * @brief Largest request head (request line + headers) accepted.
     *
     * Until the head has been parsed there is no Content-Length, so this is
     * the only point at which reading is genuinely unbounded and the only
     * place a byte cap belongs. 8 KB is the same figure nginx and Apache use
     * for a header block.
     */
    enum { MAX_HEAD_LENGTH = 8192 };

    /// Default ceiling for a request BODY.
    enum { DEFAULT_MAX_BODY_LENGTH = 8 * 1024 * 1024 };

    /**
     * @brief Bounds of the lingering close applied to a refused request.
     *
     * A request is refused (400, 413) as soon as the head says so, which is
     * usually while the peer is still uploading. Answering and closing at once
     * makes the kernel reset the connection, and the RST discards whatever the
     * peer has not read yet — including the response just written. The caller
     * then sees a bare disconnect instead of the status explaining it, which
     * is exactly the case where the reason matters most.
     *
     * So the close is deferred: the response is queued, then inbound data is
     * read and discarded until the peer stops, giving the response time to be
     * seen. Both bounds below exist so that refusing a large upload does not
     * cost the very read the limit was meant to avoid — at most
     * DEFAULT_LINGER_MAX_BYTES are discarded, for at most
     * DEFAULT_LINGER_TIMEOUT_MS. Whichever is reached first ends the drain.
     *
     * The figures follow nginx's lingering_close, scaled down: it drains for
     * up to 30 s with a 5 s idle timeout and no byte bound, which is generous
     * for a server that has already decided to say no.
     */
    enum { DEFAULT_LINGER_TIMEOUT_MS = 5000 };

    enum { DEFAULT_LINGER_MAX_BYTES = 1024 * 1024 };

  protected:
    HTTP_CGI_E cgi_;
    SmartPtr<ZCE_HTTP_REQUEST> org_request_;
    SmartPtr<ZCE_HTTP_REQUEST> request_;
    std::string remote_ip_;
    unsigned short remote_port_;
    RefBlock dblock_;
    bool chunked_ack_;
    bool gzip_ack_;
    zce_int64 body_length_ack_;

  public:
    HttpStream(HTTP_CGI_E cgi = HTTP_CGI_STANDARD);

    /**
     * @brief Raise or lower the accepted request body size, process-wide.
     *
     * Deliberately static rather than a per-stream member: adding a data
     * member to HttpStream changes its size, and this class is exported from a
     * shared library that several applications link against and do not all
     * rebuild together. A caller compiled against the old layout would then
     * allocate an object too small for the constructor to fill — which crashes
     * where it is very hard to attribute. A static costs no layout change.
     *
     * The body is buffered whole before the request is dispatched, so this is
     * also the per-connection memory a slow sender can pin. Raise it only for
     * a process whose clients are trusted.
     *
     * `unsigned` rather than size_t on purpose: this bounds
     * ZCE_HTTP_REQUEST::body_length(), which is itself unsigned, so a wider
     * parameter would advertise a range the parser cannot represent. There is
     * no 64-bit body length to compare against.
     */
    static void set_max_body_length(unsigned bytes);

    static unsigned get_max_body_length();

    /**
     * @brief Tune the lingering close, process-wide.
     *
     * Static for the same reason as set_max_body_length(): HttpStream must not
     * grow a data member. `max_bytes == 0` disables lingering and restores the
     * immediate close, which is the right setting only for a process that
     * never refuses a request mid-upload.
     *
     * @param timeout_ms  0 leaves the drain bounded by bytes alone (and by the
     *                    transport's own idle timeout).
     */
    static void set_linger_close(unsigned timeout_ms, unsigned max_bytes);

    static unsigned get_linger_timeout_ms();

    static unsigned get_linger_max_bytes();

    virtual ~HttpStream();

    int header_length() const { return request_->header_length(); }

    int body_length() { return request_->body_length(); }

    const std::string& get_x_forward_for() const;

    void proc_dblock(RefBlock& dblock, const Any&);

  protected:
    /**
     * @brief Cache the peer address for get_x_forward_for() and logging.
     *
     * Split out of on_open() so a derived stream that must delay the
     * on_open() propagation (e.g. WebSocketStream, which only opens the
     * application layer once the handshake response has been written) can
     * still record the peer without notifying the next stream.
     */
    void record_remote(const zce_sockaddr_t& remote);

    /**
     * @brief Answer a request this stream will not process, and close.
     *
     * Writes a bodyless `code` response carrying Connection: close, then hands
     * the connection to a bounded lingering close so the response survives the
     * shutdown even though the peer is still uploading — see the
     * DEFAULT_LINGER_* bounds. Non-virtual on purpose: HttpStream is exported
     * and must not grow a vtable slot any more than a data member.
     *
     * @param expect_more bytes of this request still outstanding, so a refusal
     *        never drains longer than the request itself; pass -1 when the head
     *        did not parse and there is no Content-Length to go by.
     */
    void refuseRequest(unsigned code, const char* reason, zce_int64 expect_more);

  public:
    void on_open(bool passive, const zce_sockaddr_t& remote) override;

    void on_read(RefBlock& dblock, const Any&) override;

    virtual void on_http_request(const SmartPtr<ZCE_HTTP_REQUEST>&,
                                 const RefBlock& dblock);

    virtual void on_http_continue(RefBlock& dblock) {
        (void)dblock;
    };

    virtual void on_prepare_nextreq();

    int write_ack(unsigned code, const zce_byte* buf, size_t length,
                  std::map<std::string, std::string>& paramdict);

    int write_continue(const zce_byte* buf, size_t length);
};

///////////////////////////////////////////////////////////////////////////////

class ZCE_API zce_http_client : public IStream {
    HTTP_CGI_E cgi_ = HTTP_CGI_STANDARD;
    // ZCE_HTTP_REQUEST request_;
    ZCE_HTTP_RESPONSE response_;
    RefBlock dblock_;
    RefBlock cont_dblock_;

  public:
    virtual void on_read(RefBlock& dblock, const Any&);

    virtual void on_http_response(const ZCE_HTTP_RESPONSE& header, const RefBlock& dblock) = 0;

    virtual void on_http_continue(RefBlock& dblock);

    virtual void on_http_close() = 0;

    virtual void on_close();

    // Reset response/body state so the same IStream can be reused for
    // another HTTP/1.1 keep-alive response. Called right after the current
    // response has been fully delivered to on_http_response().
    virtual void on_prepare_nextres();

    int request(
        const std::string& url, ZCE_HTTP_REQUEST::METHOD_E m, zce_byte* buf,
        size_t length,
        const std::map<std::string, std::string>& paramdict = std::map<std::string, std::string>{});
};

///////////////////////////////////////////////////////////////////////////////
struct zce_websocket_pimpl;

class WebSocketStream : public HttpStream {
    SmartPtr<zce_websocket_pimpl> pimpl_ptr_;

    int opcode_;

    // RFC6455 4.2.2: the connection is only in the OPEN state once the 101
    // response has been sent. on_open() is therefore held back until
    // on_http_request() has written the handshake response, so the
    // application never sees a connected stream it could write frames into
    // ahead of the 101 status line. Mirrors zce_websocket_client, which
    // likewise defers on_open() until the 101 has been validated.
    zce_sockaddr_t open_remote_;

    bool open_passive_;

    bool opened_;

    /**
     * @brief Answer a request that is not a valid handshake and close.
     *
     * Writes a plain HTTP error response, unframed, followed by the stream
     * stack's flush-then-close request so the response reaches the peer
     * before the socket goes away.
     */
    void writeHandshakeError(unsigned code, const char* reason);

  public:
    enum { OPCODE_TEXT = 1, OPCODE_BIN = 2 };

    WebSocketStream(int opcode = OPCODE_BIN);

    void on_open(bool passive, const zce_sockaddr_t& remote) override;

    void on_http_request(const SmartPtr<ZCE_HTTP_REQUEST>& request,
                         const RefBlock& dblock) override;

    void on_http_continue(RefBlock& dblock) override;

    void on_close() override;

    int write(RefBlock& dblock, ERV_ISTREAM_WRITEOPT opt) override;

    void on_prepare_nextreq() override {};  // websocket just continue process
};

///////////////////////////////////////////////////////////////////////////////

class zce_websocket_client : public IStream {
    ZCE_OBJECT_DECLARE;

    SmartPtr<zce_websocket_pimpl> pimpl_ptr_;

    HTTP_CGI_E cgi_ = HTTP_CGI_STANDARD;

    ZCE_HTTP_RESPONSE response_;

    RefBlock dblock_;

    std::string key_;

    std::string host_;

    std::string path_;

    zce_sockaddr_t remote_;

    int opcode_;

  public:
    enum { OPCODE_TEXT = 1, OPCODE_BIN = 2 };

    zce_websocket_client(const std::string& host, const std::string& path = "/",
                         int opcode = OPCODE_BIN);

    void on_open(bool passive, const zce_sockaddr_t& remote) override;

    void on_read(RefBlock& dblock, const Any& ctx) override;

    void on_http_response(const ZCE_HTTP_RESPONSE& header, const RefBlock& dblock);

    void on_http_continue(RefBlock& dblock);

    int write(RefBlock& dblock, ERV_ISTREAM_WRITEOPT opt) override;
};

}  // namespace zce 
