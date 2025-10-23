#pragma once
// ***************************************************************
//  zdp_stream   version:  1.0   -  date: 2015/11/14
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <zce/zce_handler.h>
#include <zce/zdp_schema.h>
#include <zce/zds_schema.h>
#include <zce/zce_timer.h>
#include <zce/zce_any.h>

namespace zce {

class RefBlock;
class Reactor;

namespace zdp {

class zdp_stream;
struct zds_context_t;

class ZCE_API zdp_resctx : public zce::Object {
    unsigned short seq_;

    unsigned short msgid_;

    zce::Any ctx_;

    zce::SmartPtr<zdp_stream> stream_ptr_;

    zce::SmartPtr<zce::Timer> timeout_ptr_;

  public:
    zdp_resctx(const zce::SmartPtr<zdp_stream>& stream_ptr, unsigned short seq,
               unsigned short msgid, const zce::Any& ctx);

    const zce::Any& context() const { return ctx_; }

    unsigned short seq() const { return seq_; }

    unsigned short msgid() const { return msgid_; }

    void timeout(int ms);

    void cancel_timer();

    void handle_timeout(const zce::Any& arg);
};

class ZCE_API zdp_stream : public zce::IStream {
    zce::RefBlock dblock_;

    zce::SmartPtr<zce::Reactor> reactor_ptr_;

    unsigned last_recv_tick_;

    zce::AtomicLong seq_;

    std::map<unsigned short, zce::SmartPtr<zdp_resctx> > res_dict_;

    int proc_next(const zdp_head& head, zce::RefBlock& dblock_ptr);

    int split_pkg(zce_byte* buf, unsigned size);

    int _do_request(const zdp_head& head, zce::RefBlock& dblock_ptr, int mstimeout,
                    const zce::Any& ctx);

  public:
    zdp_stream(const zce::SmartPtr<zce::Reactor>& reactor_ptr, unsigned preserv = 0);

    ~zdp_stream();

    unsigned last_recv_tick() const { return last_recv_tick_; }

    zce_uint32 next_seq();

    const zce::SmartPtr<zce::Reactor>& reactor_ptr() const { return reactor_ptr_; }

    //////////////////////////////////////////////////////////////////////////
    int peek_response_context(zce::Any& ctx, zce_uint16 msgmid, unsigned msgseq);

    //////////////////////////////////////////////////////////////////////////

    void on_read(zce::RefBlock& dblock_ptr, const zce::Any&) override;

    int write(zce::RefBlock& dblock_ptr,
              zce::IStream::ERV_ISTREAM_WRITEOPT opt = zce::IStream::ERV_ISTREAM_DEFAULT) override;

    // void close() override;

    void on_close() override;

    //////////////////////////////////////////////////////////////////////////

    virtual void on_packet(const zdp_head&, const zce::RefBlock& plain_body,
                           const zce::RefBlock& org_full, const zce::Any& ctx);

    virtual void on_timeout(zdp_resctx* resctx);

    zce::RefBlock create_failed_response_dblock(zce_uint16 msgmid, unsigned msgseq);

    int do_failed_process(zce_uint16 msgmid, unsigned msgseq, const zce::Any& ctx);

    // timeout = 0, won't wait res
    int do_request(zce::RefBlock& plain_body, int mstimeout = 0,
                   const zce::Any& ctx = zce::Any((zce_int64)0));

    template <typename MSG_T>
    int request(const MSG_T& msg, int mstimeout = 0, const zce::Any& ctx = zce::Any((zce_int64)0),
                ERV_ZCE_COMPRESS cps = ZCE_COMPRESS_NONE);

    template <typename MSG_T>
    int response(const MSG_T& msg, unsigned seq, zce_byte rev = 0,
                 ERV_ZCE_COMPRESS cps = ZCE_COMPRESS_NONE);

    template <typename T, typename... Args>
    int requestArgs(int msgmid, int mstimeout, const zce::Any& ctx, ERV_ZCE_COMPRESS cps,
                    int preserv, const T& val, Args&&... args);

    template <typename T, typename... Args>
    int responseArgs(int msgmid, unsigned seq, zce_byte rev, ERV_ZCE_COMPRESS cps, const T& val,
                     Args&&... args);
};

int ZCE_API zdp_serialize_dblock(zce::RefBlock& dblock_ptr, zce_uint16 msgmid, zce_uint32 seq,
                                 ERV_ZCE_COMPRESS cps, zce_uint32 bodylen, int preserv,
                                 int rev = 0);

template <typename T>
int zdp_serialize_struct(zce::RefBlock& dblock_ptr, const T& msg, zce_byte rev, int preserv = 0) {
    int ret = 0;
    int bodylen = zdp_body_length(msg, rev);
    ZCE_MBACQUIRE(dblock_ptr, preserv + bodylen);
    if (dblock_ptr.space() == 0) return ZCE_ERROR_MALLOC;

    if (preserv) dblock_ptr.preserv(preserv);
    ret += preserv;

    if (bodylen) {
        int len = zdp_pack(msg, dblock_ptr.wr_ptr(), (int)dblock_ptr.space(), rev);
        ZCE_ASSERT(bodylen == len);
        if (len < 0) return len;
        if (bodylen != len) return ZCE_ERROR_SYNTAX;
        ret += len;
    }
    dblock_ptr.wr_ptr(ret);
    return ret;
}

template <typename T>
int zdp_serialize(zce::RefBlock& dblock_ptr, zce_uint32 seq, const T& msg, zce_byte rev,
                  ERV_ZCE_COMPRESS cps = ZCE_COMPRESS_NONE, int preserv = 0) {
    int bodylen = zdp::zds_pack(0, 0, msg, 0, true);
    ZCE_MBACQUIRE(dblock_ptr, preserv + zdp_headlen(rev) + bodylen);
    if (dblock_ptr.space() <= 0) return ZCE_ERROR_MALLOC;

    if (preserv) dblock_ptr.preserv(preserv);

    if (bodylen) {
        int ret = zds_pack(dblock_ptr.wr_ptr_cow() + zdp_headlen(rev),
                           (int)(dblock_ptr.space() - zdp_headlen(rev)), msg, 0, true);
        ZCE_ASSERT(bodylen == ret);
        if (ret < 0) return ret;
        if (bodylen != ret) return ZCE_ERROR_SYNTAX;
        dblock_ptr.wr_ptr(bodylen + zdp_headlen(rev));
    } else {
        cps = ZCE_COMPRESS_NONE;
        dblock_ptr.wr_ptr(zdp_headlen(rev));
    }

    int ret = zdp_serialize_dblock(dblock_ptr, T::MSG_TYPE, seq, cps, bodylen, preserv, rev);
    ZCE_ASSERT(ret >= 0);
    if (ret < 0) {
        return ret;
    }

    return ret;
}

template <typename MSG_T>
int zdp_stream::request(const MSG_T& msg, int mstimeout, const zce::Any& ctx,
                        ERV_ZCE_COMPRESS cps) {
    zce::RefBlock dblock_ptr;
    int ret = zdp_serialize(dblock_ptr, 0, msg, 0, cps, 32);
    ZCE_ASSERT(ret >= 0);
    if (ret < 0) return ret;
    return do_request(dblock_ptr, mstimeout, ctx);
}

template <typename MSG_T>
int zdp_stream::response(const MSG_T& msg, unsigned seq, zce_byte rev, ERV_ZCE_COMPRESS cps) {
    zce::RefBlock dblock_ptr;
    // ZTRACE("response", seq);
    int ret = zdp_serialize(dblock_ptr, seq, msg, rev, cps, 32);
    ZCE_ASSERT(ret >= 0);
    if (ret < 0) return ret;
    return write(dblock_ptr, zce::IStream::ERV_ISTREAM_DEFAULT);
}

template <typename T, typename... Args>
int zdp_stream::requestArgs(int msgmid, int mstimeout, const zce::Any& ctx, ERV_ZCE_COMPRESS cps,
                            int preserv, const T& val, Args&&... args) {
    int bodylen = zds_pack_multi(0, 0, nullptr, true, val, std::forward<Args>(args)...);
    if (bodylen < 0) return bodylen;
    zce::RefBlock dblock_ptr;
    ZCE_MBACQUIRE(dblock_ptr, bodylen + zdp_headlen(0) + 32);
    if (dblock_ptr.space() <= 0) return ZCE_ERROR_MALLOC;
    dblock_ptr.preserv(preserv);

    if (bodylen > 0) {
        int ret = zds_pack_multi(dblock_ptr.wr_ptr_cow() + zdp_headlen(0),
                                 (int)(dblock_ptr.space() - zdp_headlen(0)), nullptr, true, val,
                                 std::forward<Args>(args)...);
        ZCE_ASSERT(bodylen == ret);
        if (ret < 0) return ret;
        if (bodylen != ret) return ZCE_ERROR_SYNTAX;
        dblock_ptr.wr_ptr(bodylen + zdp_headlen(0));
    } else {
        cps = ZCE_COMPRESS_NONE;
        dblock_ptr.wr_ptr(zdp_headlen(0));
    }

    int ret = zdp_serialize_dblock(dblock_ptr, msgmid, 0, cps, bodylen, preserv, 0);
    ZCE_ASSERT(ret >= 0);
    if (ret < 0) {
        return ret;
    }

    return do_request(dblock_ptr, mstimeout, ctx);
}

template <typename T, typename... Args>
int zdp_stream::responseArgs(int msgmid, unsigned seq, zce_byte rev, ERV_ZCE_COMPRESS cps,
                             const T& val, Args&&... args) {
    const int preserv = 32;
    int bodylen = zds_pack_multi(0, 0, nullptr, true, val, std::forward<Args>(args)...);
    if (bodylen < 0) return bodylen;
    zce::RefBlock dblock_ptr;
    ZCE_MBACQUIRE(dblock_ptr, bodylen + zdp_headlen(0) + 32);
    if (dblock_ptr.space() <= 0) return ZCE_ERROR_MALLOC;
    dblock_ptr.preserv(preserv);

    if (bodylen > 0) {
        int ret = zds_pack_multi(dblock_ptr.wr_ptr_cow() + zdp_headlen(0),
                                 (int)(dblock_ptr.space() - zdp_headlen(0)), nullptr, true, val,
                                 std::forward<Args>(args)...);
        ZCE_ASSERT(bodylen == ret);
        if (ret < 0) return ret;
        if (bodylen != ret) return ZCE_ERROR_SYNTAX;
        dblock_ptr.wr_ptr(bodylen + zdp_headlen(0));
    } else {
        cps = ZCE_COMPRESS_NONE;
        dblock_ptr.wr_ptr(zdp_headlen(0));
    }

    int ret = zdp_serialize_dblock(dblock_ptr, msgmid, seq, cps, bodylen, preserv, 0);
    ZCE_ASSERT(ret >= 0);
    if (ret < 0) {
        return ret;
    }
    return write(dblock_ptr, zce::IStream::ERV_ISTREAM_DEFAULT);
}

};  // namespace zdp

}  // namespace zce
