#pragma once
/* ***************************************************************
//  zce::Service   version:  1.0  date: 2006/6/22
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
//
// **************************************************************/
#include <zce/zce_config.h>
#include <zce/zce_reactor.h>
#include <zce/zce_handler.h>
#include <vector>
#include <string>

namespace zce {
class Allocator;
class TaskDelegator;
class RefBlock;
class Pipe;
class Tty;
class SubProcessHost;
class SubProcess;


struct ZCE_API AppOptions {
    std::string mode;        // daemon, work, service
    std::string guid;        // --guid <guid>
    std::string pidfile;     // --pidfile <path>
    std::string logsuffix;   // --logsuffix <suffix>
    std::string configpath;  // --configpath <path>

    std::string vmname;               // --vmname <vm name>
    std::string vmpath;               // --vmpath <vm path>
    std::string vmaddr;               // --vmaddr <vm listen address>
    unsigned short vmport;            // --vmport <vm listen port>
    std::vector<std::string> extras;  // 存放未定义参数

    std::string help_target;
#ifdef _WIN32
    std::string service_action;     // install, remove, start, stop, restart, status
    std::string service_name;       // <name>
    std::string service_exec_path;  // 可执行文件路径及参数
    std::string service_display;
#endif
};

class ZCE_API Service : public Reactor {
    struct ServiceStdInput : public IStream {
        Service* service_;
        ServiceStdInput(Service* service) : service_(service) {}
        virtual void on_read(zce::RefBlock& dblock, const zce::Any&) override;
    };

  protected:
    static zce::Service* instance_;
    AppOptions options_{};
    std::string name_;
    bool exit_success_;
    volatile bool running_;
    SmartPtr<zce::Pipe> pipe_;
    SmartPtr<zce::Tty> tty_;
    SmartPtr<Signal> signal_hup_;
    SmartPtr<Signal> signal_int_;
    SmartPtr<Signal> signal_term_;
    SmartPtr<Timer> timer_;
    zce::SmartPtr<zce::SubProcessHost> process_host_;
    zce::SmartPtr<zce::SubProcess> sub_process_;

#ifdef _WIN32
    SERVICE_STATUS_HANDLE win_service_handle_;
    SERVICE_STATUS win_service_status_{};
#endif

    int onReactorStart() override;

    void onReactorStop() override;

    virtual void onTimer();

    virtual void onStdinCommand(std::string line);

    virtual bool onDaemonStart();

    virtual void onDaemonStop();

    virtual bool onWorkerStart() = 0;

    virtual bool onWorkerStop() = 0;

  public:
    Service(const char* name);

    ~Service() override;

    virtual bool shutdownDaemonAndWorker();

    virtual void onSignal(int);

    int runWorkerProcess();

    int main(int&, char*[]);

    static Service* instance();

    bool isDaemonProcess() const;

    bool isWorkProcess() const;

    std::string name() const;

  protected:
    

    virtual void print_object_stat();

#ifdef _WIN32
    static void _cbWindowsServiceMain(DWORD argc, LPSTR* argv);
    static void _cbWindowsServiceCtrlHandler(DWORD);

    int startWindowsService();
    void onWindowsServiceMain(DWORD, LPSTR*);

    bool waitForServiceState(SC_HANDLE, DWORD, SERVICE_STATUS&);

  public:
    int installWindowsService(bool, const std::string&, const std::string&, const std::string&,
                              const std::vector<std::string>&);

    int removeWindowsService(bool, const std::string&);

    int startWindowsService(const std::string&, const std::vector<std::string>&);

    int queryWindowsServiceStatus(const std::string&);

    int stopWindowsService(const std::string&);

    void showServiceStatus(const std::string& msg, SERVICE_STATUS& status);
#else
  protected:
    int run_daemon(int, char*[]);
    std::string pid_file_;
    pid_t work_process_;
    const char* exepath_;
    int pipe_[2];
#endif  // _WIN32
};

}  // namespace zce