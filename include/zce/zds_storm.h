#pragma once

#include <zce/zce_object.h>
#include <zce/zce_types.h>
#include <zce/zce_reactor.h>
#include <zce/zce_singleton.h>
#include <zce/zce_dblock.h>
#include <zce/zdp_schema.h>
#include <zce/zdp_storm.h>
#include <zce/zdp_stream.h>
#include <zce/zce_task_queue.h>
#include <map>

namespace zdp {
    class zdp_storm_client;
};

class zce_task_queue;

class zds_storm_requester : virtual public zce_object {
public:
    virtual const zce_smartptr<zce_task_queue>& taskdeque_ptr() = 0;

    virtual const zdp::zdp_storm_peer& peer() const = 0;

    virtual void on_response(const zce_dblock& dblockptr, void* ctx) = 0;
};

class zds_storm : public zce_reactor
{
    class zds_storm_task : public zce_task
    {
        enum { TO_STORM, FROM_STORM } step_;
        zce_smartptr<zds_storm> stormptr_;
        zce_smartptr<zds_storm_requester> requester_;
        zce_int64 topic_;
        zce_dblock dblockptr_;
        int timeoutms_;
        void* ctx_;

        zce_dblock response_dblockptr_;

    public:
        zds_storm_task(const zce_smartptr<zds_storm>& stormptr,
            const zce_smartptr<zds_storm_requester>& requester,
            zce_int64 topic,
            const zce_dblock& dblockptr,
            int timeoutms,
            void* ctx);

        ~zds_storm_task();

        virtual void call();

        void response(const zce_dblock& dblockptr);
    };

    zce_smartptr<zdp::zdp_stream> stream_ptr_;

    zce_smartptr<zdp::zdp_storm_stream_adapter> adapter_ptr_;

    zce_smartptr<zdp::zdp_storm_client> storm_ptr_;

public:

    zds_storm();

    ~zds_storm();

    const zce_smartptr<zdp::zdp_storm_client>& storm_client() const { return storm_ptr_; }

    void subscribe(const std::string& topic);

    void unsubscribe(const std::string& topic);

    int set(zce_int64 topic, const zce_string& name, zce_int64 oldseq, zce_int64 uid, zce_int64 flag, const zce_byte* data, zce_uint32 len);

    void substorm(const char* addr, unsigned short port, std::string_view platform, std::string_view siteid,
        std::string_view service_class, unsigned service_id);

    virtual zce_int64 streamtopic_from_msgmid(zce_uint16 msgmid) { return 0; };

    virtual int on_topic_readzdp(zce_int64 topic, zce_int64 from, const zdp::zdp_storm_peer& peer, zce_dblock& plaindblock) = 0;

    virtual int on_topic_read(zce_int64 topic, zce_int64 from, zce_byte* data, zce_uint32 len);

    virtual int on_multitopic_read(unsigned count, zce_int64* topics, zce_byte* data, zce_uint32 len) {
        return 0;
    };

    virtual int on_topic_set(zce_int64 topic, const std::string& name,
        zce_int64 seq, zce_int64 tick, zce_int64 uid, zce_int64 flag, const std::vector<zce_byte>& data) {
        return 0;
    };

    int publish(zce_int64 topic, const zce_byte* data, size_t len, zce_int64 trace) {
        if (storm_ptr_ == 0)
            return -1;
        return storm_ptr_->publish(topic, data, len, trace);
    }

    int publish(const std::vector<zce_int64>& topics, const zce_byte* data, size_t len) {
        if (storm_ptr_ == 0)
            return -1;
        return storm_ptr_->publish(topics, data, len);
    }

    //template <typename T>
    //int gep_publish(zce_int64 topic, const T& msg, const zdp::zdp_storm_peer& peer, int seq = 0);

    template <typename T>
    int zdp_publish(zce_int64 topic, const T& msg, const zdp::zdp_storm_peer& peer, int seq = 0, zce_int64 trace = 0);

    template <typename T>
    int zdp_publish(const std::vector<zce_int64>& topic, const T& msg, const zdp::zdp_storm_peer& peer, int seq = 0);

    //////////////////////////////////////////////////////////////////////////

    virtual const zce_smartptr<zdp::zdp_stream>& create_stream(zce_int64 topic, const zdp::zdp_storm_peer& peer);

    template <typename T>
    int request_async(const zce_smartptr<zds_storm_requester>& cbdeque, zce_int64 topic, const T& msg, int timeoutms, void* ctx);
};

template <typename T>
int zds_storm::zdp_publish(zce_int64 topic, const T& msg, const zdp::zdp_storm_peer& peer, int seq, zce_int64 trace)
{
    zce_smartptr<zdp::zdp_storm_client> storm_ptr = storm_ptr_;
    if (storm_ptr == 0)
        return -1;
    return storm_ptr_->publish(topic, msg, peer, seq, trace);
}

template <typename T>
int zds_storm::zdp_publish(const std::vector<zce_int64>& topics, const T& msg, const zdp::zdp_storm_peer& peer, int seq)
{
    zce_smartptr<zdp::zdp_storm_client> storm_ptr = storm_ptr_;
    if (storm_ptr == 0)
        return -1;
    return storm_ptr_->publish(topics, msg, peer, seq);
}

template <typename T>
int zds_storm::request_async(const zce_smartptr<zds_storm_requester>& requestptr, 
    zce_int64 topic, const T& msg, int timeoutms, void* ctx)
{
    zce_dblock dblock_ptr;
    int ret = zdp::zdp_serialize(dblock_ptr, 0, msg, 0, ZCE_COMPRESS_NONE, 32);
    ZCE_ASSERT(ret >= 0);
    if (ret < 0)
        return ret;
    
    zce_smartptr<zds_storm> this_ptr(this);
    zce_smartptr<zce_task> task_ptr(new zds_storm_task(this_ptr, requestptr,
        topic, dblock_ptr, timeoutms, ctx));
    ret = delegate_task(task_ptr);
    ZCE_ASSERT(ret >= 0);
    return ret;
}
//////////////////////////////////////////////////////////////////////////

template<typename SESSION, typename RES>
class zds_storm_requester_impl : public zds_storm_requester
{
    SESSION session_;
    zdp::zdp_storm_peer peer_;
public:
    zds_storm_requester_impl(const SESSION& s, const zdp::zdp_storm_peer& peer)
        :session_(s), peer_(peer)
    {
    }

    const zdp::zdp_storm_peer& peer() const {
        return peer_;
    }

    const zce_smartptr<zce_task_queue>& taskdeque_ptr(void) {
        return session_->taskdeque_ptr();
    }

    virtual void on_response(const zce_dblock& dblockptr, void* ctx)
    {
        int ret = -1;
        zdp::zdp_head head;
        zce_smartptr<RES> msgres;

        do
        {
            if (dblockptr.length() < 10)
                break;

            ret = zdp::zdp_unpack(head, dblockptr.rd_ptr(), dblockptr.length(), 0);
            if (ret < 0)
                break;

            ZCE_ASSERT(head.msgmid == RES::MSG_TYPE);
            if (head.msgmid != RES::MSG_TYPE) {
                ret = ZCE_ERROR_SYNTAX;
                break;
            }

            msgres = new RES;
            ret = zdp::zdp_unpack(*msgres, dblockptr.rd_ptr() + head.len(), dblockptr.length() - head.len(), head.rev());
            if (ret < 0) {
                msgres = 0;
                break;
            }
        } while (0);
        session_->zdp_execute(ret, msgres, ctx);
    }
};
