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

    virtual ~HttpStream();

    int header_length() const { return request_->header_length(); }

    int body_length() { return request_->body_length(); }

    const std::string& get_x_forward_for() const;

    void proc_dblock(RefBlock& dblock, const Any&);

  public:
    void on_open(bool passive, const zce_sockaddr_t& remote) override;

    void on_read(RefBlock& dblock, const Any&) override;

    virtual void on_http_request(const SmartPtr<ZCE_HTTP_REQUEST>&,
                                 const RefBlock& dblock);

    virtual void on_http_continue(RefBlock& dblock) {};

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

    virtual void on_prepare_nextres() {};  //@todo

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

  public:
    enum { OPCODE_TEXT = 1, OPCODE_BIN = 2 };

    WebSocketStream(int opcode = OPCODE_BIN);

    void on_open(bool passive, const zce_sockaddr_t& remote) override;

    void on_http_request(const SmartPtr<ZCE_HTTP_REQUEST>& request,
                         const RefBlock& dblock) override;

    void on_http_continue(RefBlock& dblock) override;

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
