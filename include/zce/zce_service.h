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
#include <zce/zce_process.h>
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

namespace zdp {
class Storm;
}

struct ZCE_API AppOptions : zdp_base::zvm_t {
    std::string mode;        // daemon, work, service
    std::string pidfile;     // --pidfile <path>
    std::string logsuffix;   // --logsuffix <suffix>
    std::string configpath;  // --configpath <path>
    //std::string projectdir;
    std::string help_target;
#ifdef _WIN32
    std::string service_action;     // install, remove, start, stop, restart, status
    std::string service_name;       // <name>
    std::string service_exec_path;  // 可执行文件路径及参数
    std::string service_display;
#endif
};

class ZCE_API Service : public Reactor {
    class SubProcessStream;

    struct ServiceStdInput : public IStream {
        Service* service_;
        ServiceStdInput(Service* service) : service_(service) {}
        virtual void on_read(zce::RefBlock& dblock, const zce::Any&) override;
    };

  protected:
    static zce::Service* instance_;
    AppOptions options_{};
    std::string exepath_;
    bool exit_success_;
    volatile bool running_;
    SmartPtr<zce::Pipe> pipe_;
    SmartPtr<zce::Tty> tty_;
    SmartPtr<Signal> signal_hup_;
    SmartPtr<Signal> signal_int_;
    SmartPtr<Signal> signal_term_;
    SmartPtr<Timer> timer_;
    zce::SmartPtr<zce::zdp::Storm> storm_server_;
    zce::SubProcessHost::HostContext host_context_;
    zce::SmartPtr<zce::SubProcessHost> process_host_;

    zce::SmartPtr<zce::SubProcess> sub_process_;
    zce::ObjectPtr sub_vm_;

#ifdef _WIN32
    SERVICE_STATUS_HANDLE win_service_handle_ = nullptr;
    SERVICE_STATUS win_service_status_{};
#endif

    int onReactorStart() override;

    void onReactorStop() override;

    virtual void onTimer();

    virtual void onStdinCommand(std::string line);

    virtual bool onDaemonStart();

    virtual void onDaemonStop();

    virtual bool onWorkerStart();

    virtual bool onWorkerStop() = 0;

    virtual void printServiceStatus();

  public:
    Service(const char* name);

    ~Service() override;

    virtual bool shutdownDaemonAndWorker();

    virtual void onSignal(int);

    int runWorkerProcess();

    int main(int&, const char*[]);

    static Service* instance();

    bool isDaemonProcess() const;

    bool isWorkProcess() const;

    AppOptions& options() { return options_; }

    void updateVMStatus(const zdp_base::zvm_t& zvm);

  protected:
#ifdef _WIN32
    static void WINAPI _cbWindowsServiceMain(DWORD argc, LPSTR* argv);
    static void WINAPI _cbWindowsServiceCtrlHandler(DWORD);

    int startWindowsService();

    void onWindowsServiceMain(DWORD, LPSTR*);

    bool waitForServiceState(SC_HANDLE, DWORD, SERVICE_STATUS&);

    int installWindowsService(bool, const std::string&, const std::string&, const std::string&,
                              const std::vector<std::string>&);

    int removeWindowsService(bool, const std::string&);

    int startWindowsService(const std::string&, const std::vector<std::string>&);

    int queryWindowsServiceStatus(const std::string&);

    int stopWindowsService(const std::string&);

    void showServiceStatus(const std::string& msg, SERVICE_STATUS& status);
#else
    int runPosixDaemon(int, const char*[]);
#endif  // _WIN32

    int connectToFatherProcess(bool is_fist);

    int startVMFromFather(zdp_base::zvm_t zvm, zce::RefBlock content);
};

}  // namespace zce