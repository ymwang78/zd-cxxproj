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
#ifndef __zce_http_stream__
#define __zce_http_stream__

#include <zce/zce_config.h>
#include <zce/zce_handler.h>
#include <zce/zce_object.h>
#include <zce/text_stream.h>

#define HTTP_VERSION_BIND(x, y) (((x) << 16) | (y))
#define HTTP_VERSION_MAJOR(v)    (((v) >> 16) & 0x0000ffff)
#define HTTP_VERSION_MINOR(v)    ((v) & 0x0000ffff)

enum HTTP_CGI_E{
    HTTP_CGI_STANDARD,
    HTTP_CGI_UWSGI,
};

std::string ZCE_API http_urlencode(const std::string& str);

std::string ZCE_API http_urldecode(const std::string& str);

std::string ZCE_API http_urldecode(const char* data, size_t len);

int ZCE_API http_getpath_pos(const std::string& uri);

int ZCE_API http_gzip(zce_byte* dest, size_t* destLen, const zce_byte* source, size_t sourceLen, int level);

///////////////////////////////////////////////////////////////////////////////

struct ZCE_API ZCE_HTTP_HEADER : public TEXT_HEADER
{
protected:
    unsigned    unpack_offset_;
    bool        unpack_finished_;

    virtual int parse_first_line(const char* buf, int size) = 0;

    int parse_param(const char* buf, int size);

    //int decode_uwsgi(const char* buf, int size);

public:

    ZCE_HTTP_HEADER(TEXT_HEADER::TEXT_HEADER_E header_type);

    bool is_unpack_finished() const { return unpack_finished_; }

    int unpack(HTTP_CGI_E cgi, const zce_byte* buf, int size);

    virtual int pack (char* buf, int size) const = 0;

    const std::string& get_x_forward_for() const;
};

///////////////////////////////////////////////////////////////////////////////

struct ZCE_API ZCE_HTTP_REQUEST : public ZCE_HTTP_HEADER
{
	enum HTTP_REQUEST_METHOD_E
	{
		HTTP_REQUEST_METHOD_UNKNOW,
		HTTP_REQUEST_METHOD_GET,
		HTTP_REQUEST_METHOD_POST,
		HTTP_REQUEST_METHOD_PUT,
		HTTP_REQUEST_METHOD_DELETE,
		HTTP_REQUEST_METHOD_OPTIONS,
		HTTP_REQUEST_METHOD_HEAD,
		HTTP_REQUEST_METHOD_TRACE,
		HTTP_REQUEST_METHOD_LIMIT
	};

	ZCE_HTTP_REQUEST() : ZCE_HTTP_HEADER(TEXT_HEADER_REQUEST), method_(HTTP_REQUEST_METHOD_UNKNOW), version_(0) {};
	ZCE_HTTP_REQUEST(const std::string& url, HTTP_REQUEST_METHOD_E method, unsigned version = HTTP_VERSION_BIND(1, 1));
	static const char* method_to_name(HTTP_REQUEST_METHOD_E e);
	static HTTP_REQUEST_METHOD_E name_to_method(const char*);
	std::string             url_;
	HTTP_REQUEST_METHOD_E   method_;
	unsigned                version_;

	virtual int  pack(char* buf, int size) const;

private:

    virtual int parse_first_line(const char* buf, int size);

	int parse_request_uri(const char* buf, int size);
};

///////////////////////////////////////////////////////////////////////////////

struct ZCE_API ZCE_HTTP_RESPONSE : public ZCE_HTTP_HEADER
{
	ZCE_HTTP_RESPONSE() : ZCE_HTTP_HEADER(TEXT_HEADER_RESPONSE) {};
	ZCE_HTTP_RESPONSE(const std::string& str, unsigned code = 200, unsigned version = HTTP_VERSION_BIND(1, 1));
	unsigned        result_code_;
	std::string        result_string_;
	unsigned        version_;

	virtual int  pack(char* buf, int size) const;

private:

	virtual int parse_first_line(const char* buf, int size);

	int parse_status_code(const char* buf, int size);
};

///////////////////////////////////////////////////////////////////////////////

class ZCE_API zce_http_stream : public zce_istream
{
protected:
    HTTP_CGI_E  cgi_;
    zce_smartptr<ZCE_HTTP_REQUEST> org_request_;
	zce_smartptr<ZCE_HTTP_REQUEST> request_;
    std::string remote_ip_;
    unsigned short remote_port_;
    zce_dblock dblock_;

public:

    zce_http_stream(HTTP_CGI_E cgi = HTTP_CGI_STANDARD);

    virtual ~zce_http_stream();

    int  header_length()  const { return request_->header_length(); }

    int  body_length() { return request_->body_length(); }
    
    const std::string& get_x_forward_for() const;

    void proc_dblock(zce_dblock& dblock, const zce_any&);

public:

    virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

    virtual void on_read(const zce_dblock& dblock, const zce_any&);

    virtual void on_http_request(const zce_smartptr<ZCE_HTTP_REQUEST>&, const zce_dblock& dblock);

    virtual void on_http_continue(zce_dblock& dblock) {};

    virtual void on_prepare_nextreq();

	int write_ack(unsigned code, const zce_byte* buf, size_t length, std::map<std::string, std::string>& paramdict);
};

///////////////////////////////////////////////////////////////////////////////

class ZCE_API zce_http_client : public zce_istream
{
	HTTP_CGI_E cgi_ = HTTP_CGI_STANDARD;
	//ZCE_HTTP_REQUEST request_;
	ZCE_HTTP_RESPONSE response_;
	zce_dblock dblock_;
	zce_dblock cont_dblock_;
public:

	virtual void on_read(const zce_dblock& dblock, const zce_any&);

	virtual void on_http_response(const ZCE_HTTP_RESPONSE& header, const zce_dblock& dblock) = 0;

	virtual void on_http_continue(zce_dblock& dblock);

	virtual void on_http_close() = 0;

	virtual void on_close();

	virtual void on_prepare_nextres() {}; //@todo

	int request(const std::string& url, 
		ZCE_HTTP_REQUEST::HTTP_REQUEST_METHOD_E m,
		zce_byte* buf,
		size_t length, 
		const std::map<std::string, std::string>& paramdict = std::map<std::string, std::string>{});

};

///////////////////////////////////////////////////////////////////////////////
struct zce_websocket_pimpl;

class zce_websocket_stream : public zce_http_stream
{
	zce_smartptr<zce_websocket_pimpl> pimpl_ptr_;

    int opcode_;

public:
    enum { OPCODE_TEXT = 1, OPCODE_BIN = 2};

	zce_websocket_stream(int opcode = OPCODE_BIN);

	virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

	virtual void on_http_request(const zce_smartptr<ZCE_HTTP_REQUEST>& request, const zce_dblock& dblock);

	virtual void on_http_continue(zce_dblock& dblock);

	virtual int  write(const zce_dblock& dblock, ERV_ISTREAM_WRITEOPT opt);

	virtual void on_prepare_nextreq() {}; //websocket just continue process
};

///////////////////////////////////////////////////////////////////////////////

class zce_websocket_client : public zce_istream
{
    ZCE_OBJECT_DECLARE;

    zce_smartptr<zce_websocket_pimpl> pimpl_ptr_;

    HTTP_CGI_E cgi_ = HTTP_CGI_STANDARD;

    ZCE_HTTP_RESPONSE response_;

    zce_dblock dblock_;

    std::string key_;

    std::string host_;

    std::string path_;

    std::string peerip_;

    zce_uint16 peerport_;

    int opcode_;

public:
    enum { OPCODE_TEXT = 1, OPCODE_BIN = 2 };

    zce_websocket_client(const std::string& host, const std::string& path = "/", int opcode = OPCODE_BIN);

    virtual void on_open(bool passive, const char* peerip, zce_uint16 peerport);

    virtual void on_read(const zce_dblock& dblock, const zce_any& ctx);

    virtual void on_http_response(const ZCE_HTTP_RESPONSE& header, const zce_dblock& dblock) ;

    virtual void on_http_continue(zce_dblock& dblock);

    virtual int  write(const zce_dblock& dblock, ERV_ISTREAM_WRITEOPT opt);
};

///////////////////////////////////////////////////////////////////////////////

#endif
