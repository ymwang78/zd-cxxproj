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
}  // namespace zce

namespace zce {

    // guid在全系统唯一，考虑到同一个服务可能被多个部署，例如HOSTVM/HOSTPY，需要确保唯一来包装管道唯一
struct ZCE_API SubProcessInfo {
    std::string name;  // name在本进程内唯一
    std::string guid;
    unsigned int pid;
    std::string workdir;
    std::string exepath;
    std::vector<std::string> args;
    std::map<std::string, std::string> env;
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

    int invoke(std::string name, std::string work_dir, std::string exepath,
               const std::vector<std::string>& exeargs,
               const std::map<std::string, std::string>& env);

    int querySubProcessInfo(const std::string& name, struct SubProcessInfo& info);
};

class ZCE_API Process : virtual public Object {
    struct Impl;
    Impl* pimpl_;

  public:
    using ExitCallback = std::function<void(int)>;

    Process(const SmartPtr<Reactor>& reactor, const char* process_path,
            std::vector<std::string> args, const char* work_dir = "",
            std::map<std::string, std::string> env = {}, ExitCallback exit_cb = nullptr);

    ~Process();

    int start(std::string pipe_name, SmartPtr<IStream> subprocess_istream);

    int kill(int signum = 0);

    int pid() const;

    static bool isProcessExists(unsigned long process_id);
};

class SubProcess : virtual public Object {
    struct Impl;
    Impl* pimpl_;

  public:
    explicit SubProcess(const SmartPtr<Reactor>& reactor);

    ~SubProcess() override;

    int connect(const std::string& pipe_name, SmartPtr<IStream> process_istream);
};

}  // namespace zce