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
#include <zce/zdp_base_proto.h>
#include <zce/zvm.h>
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
    PROCESS_S2MQUEFYVM_REQ = 0x0000,
    PROCESS_S2MQUEFYVM_RES = 0x0001,
    PROCESS_S2MUPDATEVM_REQ = 0x0002,
    PROCESS_S2MUPDATEVM_RES = 0x0003,
    PROCESS_S2MHBEAT_REQ = 0x0008,
    PROCESS_S2MHBEAT_RES = 0x0009,

    PROCESS_M2SQUIT_REQ = 0x100,
    PROCESS_M2SQUIT_RES = 0x101,
    PROCESS_M2SHBEAT_REQ = 0x102,
    PROCESS_M2SHBEAT_RES = 0x103,
};

class SubProcessHost;

class ZCE_API Process : public zce::zdp::zdp_stream {
    struct Impl;
    Impl* pimpl_;

  public:
    struct ZCE_API ProcessInfo : public zdp_base::zvm_t {
        std::string exepath;
        unsigned int delayed;  // 延迟启动时间，单位秒
        unsigned int pid;
        zce_timestamp starttime;
        zce_timestamp endtime;
        zce::RefBlock dblock;
        std::string extra;
    };

    using ExitCallback = std::function<void(int)>;

    Process(SubProcessHost* host, zdp_base::zvm_t info, bool debug, zce::RefBlock content, 
            ExitCallback exit_cb = nullptr);

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

class ZCE_API SubProcessHost : public ::zce::zvm::Machine {
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
        std::string metadb_path = "subvm.db";
        std::string table_name = "subvm";
        bool debug_mode = false;
        ProcessPreCheckCallback precheck_cb = nullptr;
        ConnectCallback connect_cb = nullptr;
        DisconnectCallback disconnect_cb = nullptr;
        DataCallback data_cb = nullptr;
        std::string vmname = "VMHost";
        std::string vmaddr = "0.0.0.0";
        unsigned short vmport = (unsigned short)~0;
        unsigned short stormport = (unsigned short)~0;
        zce_uint64 hosttopic = ((zce_uint64)rand() * zce_thread_id());
        std::string host_dir = ".";
    };

    SubProcessHost(const zce::SmartPtr<zce::zvm::VirtualMachineStub>& stub_ptr,
                   const zce::SmartPtr<zce::Reactor>& reactor_ptr, HostContext context);

    ~SubProcessHost() noexcept override;

    const zce::SmartPtr<zce::Reactor>& reactor_ptr() const noexcept;

    const HostContext& context() const noexcept;

    void checkDelayedStart();

    zce::SmartPtr<Process> createSubProcess(zdp_base::zvm_t process_info, bool debug_mode,
                                            zce::RefBlock content);

    int invoke(const zce::SmartPtr<Process>& subprocess_ptr);

    int stopSubProcess(const std::string& name);

    int querySubProcess(const std::string& name, zce::SmartPtr<Process>& subprocess_ptr);

    const std::map<std::string, zce::SmartPtr<Process>>& queryAllSubProcess() const;

    virtual int start() override;

    virtual void stop() override;

    virtual int call_dblock(zce_int64 objid, const std::string& method, zce::RefBlock& dblock,
                            int mstimetout,
                            const zce::zvm::VirtualMachineStub::response_cb& response) override;
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