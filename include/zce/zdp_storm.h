// ***************************************************************
//  Storm   version:  1.0   -  date: 2015/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#pragma once

#include <zce/zce_object.h>
#include <zce/zdp_schema.h>
#include <zce/zds_schema.h>
#include <zce/zce_api.h>
#include <zce/zce_handler.h>
#include <zce/zvm.h>

namespace zce {

class Scheduler;
class RefBlock;
class Any;
class Reactor;

namespace zdp {

extern "C" {
typedef int (*publish_callback)(const ::zce::Any& ctx, int cnt, const zce_int64* topics,
                                zce_int64 from, const zce_byte* data, zce_uint32 len);

typedef int (*set_callback)(const ::zce::Any& ctx, zce_int64 topic, const zce_string& name,
                            zce_int64 seq, zce_int64 tick, zce_int64 uid, zce_int64 flag,
                            const std::vector<zce_byte>& data);

char* ZCE_API zdp_printable_topic(char buffer[32], zce_int64 topic);

zce_int64 ZCE_API zdp_create_topic(const char svrname[4], const char siteid[4]);

zce_int64 ZCE_API zdp_server_ident(const char svrtype[2], zce_uint16 seq, const char siteid[4]);

zce_int64 ZCE_API zdp_service_ident(zce_uint32 seq, const char siteid[4]);

zce_int64 ZCE_API zdp_user_ident(unsigned uid, const char siteid[4]);
};

struct ZCE_API zdp_storm_peer {
    zce_int64 to;
    zce_int64 from;
    zdp_storm_peer() : to(0), from(0) {}
};

class ZCE_API Storm : public ::zce::Object {
  public:
    struct Pimpl;

  private:
    ::zce::SmartPtr<Pimpl> pimpl_ptr_;

  public:
    Storm(const ::zce::SmartPtr<::zce::Reactor>&, const ::zce::SmartPtr<::zce::Scheduler>&,
          zce_uint16 shard_id, const ::zce::Any& ctx, publish_callback child_cb,
          set_callback set_cb);

    ~Storm();

    int listen(const char* localip, zce_uint16 localport);

    int stop();
};

class ZCE_API StormClient : public ::zce::Object {
  public:
    struct Pimpl;

  private:
    ::zce::SmartPtr<Pimpl> pimpl_ptr_;

  public:
    StormClient(const ::zce::SmartPtr<::zce::Reactor>&, const std::string& client_ident,
                const std::string& token, const ::zce::Any& ctx, publish_callback father_cb,
                set_callback set_cb,
                std::function<void(bool passive, const zce_sockaddr_t& remote)> connected_cb,
                std::function<void()> disconnect_cb);

    ~StormClient();

    const std::string getClientIdent() const noexcept;

    zce_int64 getClientId() const noexcept;

    int stop();

    ERV_ZCE_COMPRESS default_cps() const;

    void default_cps(ERV_ZCE_COMPRESS v);

    int connect(const char* fatherip, zce_uint16 fatherport);

    // subscribe by topic name (e.g. "server.topic"); server returns unique id for publish/set
    int subscribe(const std::string& topic);

    zce_int64 getTopicId(const std::string& topic) const;

    int unsubscribe(zce_int64 topic_id);

    int publish(zce_int64 topic, const zce_byte* data, size_t len, zce_int32 trace);

    int publish(const std::vector<zce_int64>& topics, const zce_byte* data, size_t len);

    int set(zce_int64 topic, const std::string& name, zce_int64 oldseq, zce_int64 uid,
            zce_int64 flag, const zce_byte* data, zce_uint32 len);

    template <typename T, typename TTOPIC>
    int publish(TTOPIC topic, const T& msg, const zdp_storm_peer& peer = zdp_storm_peer(),
                int seq = 0, zce_int64 trace = 0);

    template <typename T>
    int set(zce_int64 topic, const zce_string& name, zce_int64 oldseq, zce_int64 uid,
            zce_int64 flag, const T& msg);

    template <typename... Args>
    int publishMessage(zce_int64 topic, Args&&... args) {
        ZTRACE("ZmpcStormClient::publish", hex_t<zce_uint64>(topic), sizeof...(args));
        if (topic == 0) {
            return -1;
        }
        int ret = 0;
        zce::RefBlock dblock;
        {
            ret = zce::zdp::zds_pack_multi(0, 0, nullptr, true, std::forward<Args>(args)...);
            if (ret < 0) return ret;
            ZCE_MBACQUIRE(dblock, ret);
            if ((int)dblock.space() < ret) return ZCE_ERROR_MALLOC;
            ret = zce::zdp::zds_pack_multi(dblock.wr_ptr_cow(), (int)dblock.space(), nullptr, true,
                                           std::forward<Args>(args)...);
            if (ret < 0) return ret;
            dblock.wr_ptr(ret);
        }

        return publish(topic, dblock.rd_ptr(), dblock.length(), 0);
    }
};

template <typename T, typename TTOPIC>
int StormClient::publish(TTOPIC topic, const T& msg, const zdp_storm_peer& peer, int seq,
                         zce_int64 trace) {
    ::zce::RefBlock dblock_ptr;
    bool preserv = !(peer.to == 0 && peer.from == 0);
    int ret = zdp_serialize(dblock_ptr, seq, msg, 0, default_cps(), preserv ? 16 : 0);
    ZCE_ASSERT(ret >= 0);
    if (ret < 0) return ret;
    if (preserv) {
        dblock_ptr.rd_ptr(-16);
        zdp::pack_builtin(dblock_ptr.rd_ptr_cow(), 8, peer.to);
        zdp::pack_builtin(dblock_ptr.rd_ptr_cow() + 8, 8, peer.from);
    }
    return publish(topic, dblock_ptr.rd_ptr(), dblock_ptr.length(), trace);
}

template <typename T>
int StormClient::set(zce_int64 topic, const zce_string& name, zce_int64 oldseq, zce_int64 uid,
                     zce_int64 flag, const T& msg) {
    ::zce::RefBlock data_ptr, cond_ptr;
    zdp_serialize(data_ptr, 0, msg, 0, default_cps());
    return set(topic, name, oldseq, uid, flag, data_ptr.rd_ptr(), data_ptr.length());
}

//////////////////////////////////////////////////////////////////////////

class ZCE_API StormStreamAdapter : public ::zce::IStream {
    zce_int64 topic_;

    zdp::zdp_storm_peer peer_;

    ::zce::SmartPtr<zdp::StormClient> stormclient_ptr_;

  public:
    StormStreamAdapter(zce_int64 topic, const zdp_storm_peer& peer,
                       const ::zce::SmartPtr<zdp::StormClient>& stormclient);

    void set_peer(zce_int64 topic, const zdp::zdp_storm_peer& p) {
        topic_ = topic;
        peer_ = p;
    }

    int write(::zce::RefBlock& dblock, ERV_ISTREAM_WRITEOPT) override;
};

//////////////////////////////////////////////////////////////////////////


class ZCE_API StormVM : public ::zce::zvm::Machine {
    struct Impl;
    std::unique_ptr<Impl> pimpl_;

  public:

    StormVM(const std::string& vm_name, const zce::SmartPtr<zce::zvm::VirtualMachineStub>& stub_ptr,
            const zce::SmartPtr<zce::Reactor>& reactor_ptr,
            const zce::SmartPtr<zce::Scheduler>& scheduler_ptr, zce_uint16 shard_id,
            const std::string& listen_ip, unsigned short listen_port);

    virtual ~StormVM() noexcept;

    virtual int start() override;

    virtual void stop() override;

    virtual int call_dblock(zce_int64 objid, const std::string& method, ::zce::RefBlock& dblock,
                            int mstimeout,
                            const zce::zvm::VirtualMachineStub::response_cb& response) override;
};


}  // namespace zdp

}  // namespace zce
