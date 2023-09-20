// ***************************************************************
//  zdp_tcp   version:  1.0   -  date: 11/14/2015
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once
#ifndef __zdp_tcp_h__
#define __zdp_tcp_h__

#include <zce/zce_handler.h>
#include <zce/zdp_schema.h>
#include <zce/zce_timer.h>

class zce_dblock;
class zce_reactor;

namespace zdp
{
    class zdp_stream;

    class ZCE_API zdp_resctx : public zce_timer_doozer
    {
        unsigned short seq_;

        unsigned short msgid_;

        void* ctx_;

        zce_smartptr<zdp_stream> stream_ptr_;

        zce_smartptr<zce_timer> timeout_ptr_;

    public:

        zdp_resctx(const zce_smartptr<zdp_stream>& stream_ptr, unsigned short seq, unsigned short msgid, void* ctx);

        void* context() const {
            return ctx_;
        }

        unsigned short seq() const {
            return seq_;
        }

        unsigned short msgid() const {
            return msgid_;
        }

        void timeout(int ms);

        void cancel_timer();

        virtual void handle_timeout(const void* arg);
    };

    class ZCE_API zdp_stream : public zce_istream
    {
        enum { READ_HEAD, READ_BODY } status_;

        zce_smartptr<zce_reactor> reactor_ptr_;

        unsigned preserve_;

        zce_atomic_long seq_;

        std::map<unsigned short, zce_smartptr<zdp_resctx> > res_dict_;

        int proc_next(const zdp_head& head, zce_dblock& dblock_ptr);

        int split_pkg(zce_byte* buf, unsigned size);

    public:

        zdp_stream(const zce_smartptr<zce_reactor>& reactor_ptr, unsigned preserv = 0);

        ~zdp_stream();

        zce_uint32 next_seq();

        const zce_smartptr<zce_reactor>& reactor_ptr() const {
            return reactor_ptr_;
        }

        //////////////////////////////////////////////////////////////////////////
        int peek_response_context(void*& ctx, zce_uint16 msgmid, unsigned msgseq);

        //////////////////////////////////////////////////////////////////////////

        virtual void on_read(zce_dblock& dblock_ptr, void*);

        virtual int write(const zce_dblock& dblock_ptr, zce_istream::ERV_ISTREAM_WRITEOPT opt);

        virtual void close();

        //////////////////////////////////////////////////////////////////////////

        virtual void on_packet(const zdp_head&, const zce_dblock& plain_body, const zce_dblock& org_full);

        virtual void on_timeout(zdp_resctx* resctx);

        zce_dblock create_failed_response_dblock(zce_uint16 msgmid, unsigned msgseq);

        int do_failed_process(zce_uint16 msgmid, unsigned msgseq, void* ctx);

        //timeout = 0, won't wait res
        int do_request(const zce_dblock& plain_body, int mstimeout = 0, void* ctx = 0);

        template<typename MSG_T>
        int request(const MSG_T& msg, int mstimeout = 0, void* ctx = 0);

        template<typename MSG_T>
        int response(const MSG_T& msg, unsigned seq, zce_byte rev = 0);
    };

    int ZCE_API zdp_serialize_dblock(zce_dblock& dblock_ptr, zce_uint16 msgmid,
        zce_uint32 seq, ERV_ZCE_COMPRESS cps, zce_uint32 bodylen, int preserv, int rev = 0
#if 1 //def ZDP_GEP
        , zdp::zdp_head::gepex_t* gepexptr = 0
#endif
    );

    template<typename T>
    int zdp_serialize_struct(zce_dblock& dblock_ptr, const T& msg, zce_byte rev, int preserv = 0) {
        int ret = 0;
        int bodylen = zdp_body_length(msg, rev);
        ZCE_MBACQUIRE(dblock_ptr, preserv + bodylen);
        if (dblock_ptr.space() == 0)
            return ZCE_ERROR_MALLOC;

        if (preserv)
            dblock_ptr.preserv(preserv);
        ret += preserv;

        if (bodylen) {
            int len = zdp_pack(msg, dblock_ptr.wr_ptr(), (int)dblock_ptr.space(), rev);
            ZCE_ASSERT(bodylen == len);
            if (len < 0)
                return len;
            if (bodylen != len)
                return ZCE_ERROR_SYNTAX;
            ret += len;
        }
        dblock_ptr.wr_ptr(ret);
        return ret;
    }

    template<typename T>
    int zdp_serialize_ie(zce_dblock& dblock_ptr, const T& msg, zce_byte rev, int preserv = 0) {
        int ret = 0;
        int bodylen = zdp_body_length(msg, rev);
        bodylen += zdp_ie_head_len(bodylen, rev);
        ZCE_MBACQUIRE(dblock_ptr, preserv + bodylen);
        if (dblock_ptr.space() == 0)
            return ZCE_ERROR_MALLOC;

        if (preserv)
            dblock_ptr.preserv(preserv);
        ret += preserv;

        if (bodylen) {
            int len = zdp_pack(msg, dblock_ptr.wr_ptr(), (int)dblock_ptr.space(), rev);
            ZCE_ASSERT(bodylen == len);
            if (len < 0)
                return len;
            if (bodylen != len)
                return ZCE_ERROR_SYNTAX;
            ret += len;
        }
        dblock_ptr.wr_ptr(ret);
        return ret;
    }

    template<typename T>
    int zdp_serialize(zce_dblock& dblock_ptr, zce_uint32 seq, const T& msg, zce_byte rev,
        ERV_ZCE_COMPRESS cps = ZCE_COMPRESS_NONE, int preserv = 0
#ifdef ZDP_GEP
        , zdp::zdp_head::gepex_t* gepexptr = 0
#endif
    ) {

        int bodylen = zdp_body_length(msg, rev);
        ZCE_MBACQUIRE(dblock_ptr, preserv + zdp_headlen(rev) + bodylen);
        if (dblock_ptr.space() <= 0)
            return ZCE_ERROR_MALLOC;

        if (preserv)
            dblock_ptr.preserv(preserv);

        if (bodylen) {
            int ret = zdp_pack(msg, dblock_ptr.wr_ptr() + zdp_headlen(rev),
                (int)(dblock_ptr.space() - zdp_headlen(rev)), rev);
            ZCE_ASSERT(bodylen == ret);
            if (ret < 0)
                return ret;
            if (bodylen != ret)
                return ZCE_ERROR_SYNTAX;
            dblock_ptr.wr_ptr(bodylen + zdp_headlen(rev));
        }
        else {
            cps = ZCE_COMPRESS_NONE;
            dblock_ptr.wr_ptr(zdp_headlen(rev));
        }

        int ret = zdp_serialize_dblock(dblock_ptr, T::MSG_TYPE, seq, cps, bodylen, preserv, rev
#ifdef ZDP_GEP
        , gepexptr
#endif
        );
        ZCE_ASSERT(ret >= 0);
        if (ret < 0) {
            return ret;
        }
        
        return ret;
    }

    template<typename MSG_T>
    int zdp_stream::request(const MSG_T& msg, int mstimeout, void* ctx) {
        zce_dblock dblock_ptr;
        int ret = zdp_serialize(dblock_ptr, 0, msg, 0, ZCE_COMPRESS_NONE, 32);
        ZCE_ASSERT(ret >= 0);
        if (ret < 0)
            return ret;
        return do_request(dblock_ptr, mstimeout, ctx);
    }

    template<typename MSG_T>
    int zdp_stream::response(const MSG_T& msg, unsigned seq, zce_byte rev) {
        zce_dblock dblock_ptr;
        int ret = zdp_serialize(dblock_ptr, seq, msg, rev, ZCE_COMPRESS_NONE, 32);
        ZCE_ASSERT(ret >= 0);
        if (ret < 0)
            return ret;
        return write(dblock_ptr, zce_istream::ERV_ISTREAM_DEFAULT);
    }
};

#endif // __zdp_tcp_h__
