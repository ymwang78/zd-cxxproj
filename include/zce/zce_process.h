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
class ZCE_API Process : virtual public Object {
    struct Impl;
    Impl* pimpl_;
  public:
    using ExitCallback = std::function<void(int)> ;

    Process(const SmartPtr<Reactor>& reactor, const char* process_path,
            std::vector<std::string> args, const char* work_dir = "",
            std::map<std::string, std::string> env = {},
            ExitCallback exit_cb = nullptr);

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

    ~SubProcess() override ;

    int connect(const std::string& pipe_name, SmartPtr<IStream> process_istream);
};


}  // namespace zce