#pragma once
// ***************************************************************
//  zce_process   version:  1.0   -  date:  2025/10/15
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project ZCE.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <zce/zce_object.h>
#include <zce/zce_handler.h>
#include <zce/zdp_stream.h>
#include <map>
#include <functional>

namespace zce {
class Reactor;

namespace zdb {
class Database;
class Statement;
}  // namespace zdb

namespace zdp {
struct zdp_head;
}  // namespace zdp

}  // namespace zce

namespace zce {

enum PRECESS_MSGID {
    PRECESS_MSGID_S2MSTART_REQ = 0,
    PRECESS_MSGID_S2MSTART_RES = 2,
    PRECESS_MSGID_M2SHBEAT_REQ = 4,
    PRECESS_MSGID_M2SHBEAT_RES = 6,
    PRECESS_MSGID_S2MQUIT_REQ = 8,
    PRECESS_MSGID_S2MQUIT_RES = 10,

    PRECESS_MSGID_M2SQUIT_REQ = 0x100,
    PRECESS_MSGID_M2SQUIT_RES,
};

class SubProcessHost;

class ZCE_API Process : public zce::zdp::zdp_stream {
    struct Impl;
    Impl* pimpl_;

  public:
    struct ZCE_API ProcessInfo {
        std::string name;       // name在本进程内唯一
        std::string pipeid;     // pipe id, guid, 全局唯一
        std::string workdir;
        std::string exepath;
        std::vector<std::string> args;
        std::map<std::string, std::string> env;
        unsigned int delayed;  // 延迟启动时间，单位秒

        unsigned int pid;
        zce_timestamp starttime;
        zce_timestamp endtime;

        std::string extra;  // 扩展字段
    };

    using ExitCallback = std::function<void(int)>;

    Process(SubProcessHost* host, ProcessInfo info, bool debug = false,
            ExitCallback exit_cb = nullptr);

    ~Process();

    int startProcess();

    int upsert();

    int kill(int signum = 0);

    int pid() const;

    void setContextPtr(const zce::SmartPtr<zce::Object>& context_ptr);

    const zce::SmartPtr<zce::Object>& getContextPtr() const;

    const ProcessInfo& processInfo() const;

    ProcessInfo& processInfo();

    bool isRunning() const;

    static bool isProcessExists(unsigned long process_id, const std::string& process_name);

    // override from IStream
    void on_open(bool passive, const zce_sockaddr_t& remote) override;

    void on_packet(const zce::zdp::zdp_head& head, const zce::RefBlock& dblock,
                   const zce::RefBlock& org_full, const zce::Any& ctx) override;

    void on_close() override;
};

class ZCE_API SubProcessHost : public Object {
    friend class Process;
    struct Impl;
    Impl* pimpl_;
  public:
    using ConnectCallback = std::function<void(const zce::SmartPtr<Process>&)>;
    using DisconnectCallback = std::function<void(const zce::SmartPtr<Process>&)>;
    using DataCallback = std::function<void(
        const zce::SmartPtr<Process>&, const zce::zdp::zdp_head&, zce::RefBlock, const zce::Any&)>;
    using ProcessPreCheckCallback = std::function<int(const zce::SmartPtr<zce::Process>&)>;

    struct HostContext {
        std::string config_path = "subprocess.db";
        std::string table_name = "subprocess";
        bool debug_mode = false;
        ProcessPreCheckCallback precheck_cb = nullptr;
        ConnectCallback connect_cb = nullptr;
        DisconnectCallback disconnect_cb = nullptr;
        DataCallback data_cb = nullptr;
    };

    SubProcessHost(const zce::SmartPtr<zce::Reactor>& reactor_ptr, HostContext context);

    ~SubProcessHost() override;

    const zce::SmartPtr<zce::Reactor>& reactor_ptr() const;

    const HostContext& context() const;

    void stopAllSubProcess();

    void checkDelayedStart();

    zce::SmartPtr<Process> createSubProcess(const Process::ProcessInfo& process_info, bool debug_mode);

    int invoke(const zce::SmartPtr<Process>& subprocess_ptr);

    int stopSubProcess(const std::string& name);

    int querySubProcess(const std::string& name, zce::SmartPtr<Process>& subprocess_ptr);

    const std::map<std::string, zce::SmartPtr<Process>>& queryAllSubProcess() const;
};

class SubProcess : virtual public Object {
    struct Impl;
    Impl* pimpl_;

  public:
    using ConnectCallback = std::function<void(zce::SmartPtr<SubProcess>)>;
    using DisconnectCallback = std::function<void(zce::SmartPtr<SubProcess>)>;
    using DataCallback = std::function<void(zce::SmartPtr<SubProcess>, const zce::zdp::zdp_head&,
                                            zce::RefBlock, const zce::Any&)>;

    explicit SubProcess(const SmartPtr<Reactor>& reactor);

    ~SubProcess() override;

    int connectProcess(const std::string& pipe_id, SmartPtr<IStream> process_istream);

    int connectProcess(const std::string& pipe_id, ConnectCallback connect_cb,
                       DisconnectCallback disconnect, DataCallback data_cb);

    void close();

    SmartPtr<IStream> getStreamPtr();
};

}  // namespace zce