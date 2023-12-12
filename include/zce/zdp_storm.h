// ***************************************************************
//  zdp_storm   version:  1.0   -  date: 11/16/2015
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once

#include <zce/zce_object.h>
#include <zce/zdp_schema.h>
#include <zce/zce_api.h>
#include <zce/zce_handler.h>

class zce_dblock;
class zce_reactor;
class zce_schedule;

namespace zdp
{ 
    struct ZCE_API zdp_storm_peer
    {
        zce_int64 to;
        zce_int64 from;
        zdp_storm_peer() : to(0), from(0) {

        }
    };

    extern "C" {
        typedef int (*publish_callback)(void * ctx, int cnt,
            zce_int64* topics, zce_int64 from, zce_byte* data, zce_uint32 len);

        typedef int (*set_callback)(void * ctx, zce_int64 topic, const zce_string& name, 
            zce_int64 seq, zce_int64 tick, zce_int64 uid, zce_int64 flag, const std::vector<zce_byte>& data);

        char* ZCE_API zdp_printable_topic(char buffer[32], zce_int64 topic);

        zce_int64 ZCE_API zdp_create_topic(const char svrname[4], const char siteid[4]);

        zce_int64 ZCE_API zdp_server_ident(const char svrtype[2], zce_uint16 seq, const char siteid[4]);

        zce_int64 ZCE_API zdp_service_ident(zce_uint32 seq, const char siteid[4]);

        zce_int64 ZCE_API zdp_user_ident(unsigned uid, const char siteid[4]);
    };

    class ZCE_API zdp_storm : public zce_object
    {
    public:
        struct Pimpl;
    private:
        zce_smartptr<Pimpl> pimpl_ptr_;

    public:

        zdp_storm(const zce_smartptr<zce_reactor>&, 
            const zce_smartptr<zce_schedule>&,
            zce_int64 uniqueid, const zce_string& token, void* ctx,
            publish_callback child_cb, set_callback set_cb);

        ~zdp_storm();

        int listen(const char* localip, zce_uint16 localport);

        int stop();
    };

    class ZCE_API zdp_storm_client : public zce_object
    {
    public:
        struct Pimpl;
    private:
        zce_smartptr<Pimpl> pimpl_ptr_;

    public:

        zdp_storm_client(const zce_smartptr<zce_reactor>& ,
            zce_int64 uniqueid, const zce_string& token,
            void* ctx, publish_callback father_cb, set_callback set_cb);

        ~zdp_storm_client();

        int stop();

        ERV_ZCE_COMPRESS default_cps() const;

        void default_cps(ERV_ZCE_COMPRESS v);

        int connect(const char* fatherip, zce_uint16 fatherport);

        //subscribe topic
        int subscribe(zce_int64 topic);

        int unsubscribe(zce_int64 topic);

        int publish(zce_int64 topic, const zce_byte* data, size_t len, zce_int64 trace);

        int publish(const std::vector<zce_int64>& topics, const zce_byte* data, size_t len);

        int set(zce_int64 topic, const zce_string& name, zce_int64 oldseq, 
            zce_int64 uid, zce_int64 flag, const zce_byte* data, zce_uint32 len);

        template<typename T, typename TTOPIC>
        int publish(TTOPIC topic, const T& msg, const zdp_storm_peer& peer = zdp_storm_peer(), int seq = 0, zce_int64 trace = 0);

        template<typename T>
        int set(zce_int64 topic, const zce_string& name, zce_int64 oldseq, zce_int64 uid, zce_int64 flag, const T& msg);
    };

    template<typename T, typename TTOPIC>
    int zdp_storm_client::publish(TTOPIC topic, const T& msg, const zdp_storm_peer& peer, int seq, zce_int64 trace) {
        zce_dblock dblock_ptr;
        bool preserv = !(peer.to == 0 && peer.from == 0);
        int ret = zdp_serialize(dblock_ptr, seq, msg, 0, default_cps(), preserv ? 16 : 0);
        ZCE_ASSERT(ret >= 0);
        if (ret < 0)
            return ret;
        if (preserv) {
            dblock_ptr.rd_ptr(-16);
            zdp::pack_builtin(dblock_ptr.rd_ptr(), 8, peer.to);
            zdp::pack_builtin(dblock_ptr.rd_ptr() + 8, 8, peer.from);
        }
        return publish(topic, dblock_ptr.rd_ptr(), dblock_ptr.length(), trace);
    }

    template<typename T>
    int zdp_storm_client::set(zce_int64 topic, const zce_string& name, zce_int64 oldseq, zce_int64 uid, zce_int64 flag, const T& msg) {
        zce_dblock data_ptr, cond_ptr;
        zdp_serialize(data_ptr, 0, msg, 0, default_cps());
        return set(topic, name, oldseq, uid, flag, data_ptr.rd_ptr(), data_ptr.length());
    }

    //////////////////////////////////////////////////////////////////////////

    class ZCE_API zdp_storm_stream_adapter : public zce_istream
    {
        zce_int64 topic_;

        zdp::zdp_storm_peer peer_;

        zce_smartptr<zdp::zdp_storm_client> stormclient_ptr_;

    public:

        zdp_storm_stream_adapter(zce_int64 topic, const zdp_storm_peer& peer,
            const zce_smartptr<zdp::zdp_storm_client>& stormclient);

        void set_peer(zce_int64 topic, const zdp::zdp_storm_peer& p) {
            topic_ = topic;
            peer_ = p;
        }

        virtual int  write(const zce_dblock& dblock, ERV_ISTREAM_WRITEOPT);
    };

}
