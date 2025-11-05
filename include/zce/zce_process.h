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

// guid在全系统唯一，考虑到同一个服务可能被多个部署，例如HOSTVM/HOSTPY，需要确保唯一来包装管道唯一
struct ZCE_API SubProcessInfo {
    std::string name;  // name在本进程内唯一
    std::string guid;
    std::string workdir;
    std::string exepath;
    std::vector<std::string> args;
    std::map<std::string, std::string> env;
    unsigned int delayed;  // 延迟启动时间，单位秒
    unsigned int pid;
    zce_timestamp starttime;
    zce_timestamp endtime;
};

extern zce::zdb::Statement& operator>>(zce::zdb::Statement& stmt, zce::SubProcessInfo& inst);
extern zce::zdb::Statement& operator<<(zce::zdb::Statement& stmt, const zce::SubProcessInfo& inst);

class ZCE_API SubProcessHost : public Object {
    struct Impl;
    Impl* pimpl_;

  public:
    SubProcessHost(const zce::SmartPtr<zce::Reactor>& reactor_ptr, const std::string& config_path,
                   const char* table_name = "subprocess");

    ~SubProcessHost() override;
    
    void stopAllSubProcess();

    void checkDelayedStart();

    int invoke(std::string name, std::string guid_hint, std::string work_dir, std::string exepath,
               const std::vector<std::string>& exeargs,
               const std::map<std::string, std::string>& env, int delayed = 0);

    int querySubProcessInfo(const std::string& name, struct SubProcessInfo& info);
};

class ZCE_API Process : virtual public Object {
    struct Impl;
    Impl* pimpl_;

  public:
    using ExitCallback = std::function<void(int)>;

    Process(const SmartPtr<Reactor>& reactor, const std::string& process_path,
            std::vector<std::string> args, const char* work_dir = "",
            std::map<std::string, std::string> env = {}, ExitCallback exit_cb = nullptr);

    ~Process();

    int attachProcess(unsigned long process_id, std::string pipe_id,
                      SmartPtr<IStream> subprocess_istream);

    int startProcess(std::string pipe_id, SmartPtr<IStream> subprocess_istream);

    int kill(int signum = 0);

    int pid() const;

    static bool isProcessExists(unsigned long process_id);
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