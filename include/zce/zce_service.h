#pragma once
/* ***************************************************************
//  zce_service   version:  1.0  date: 2006/6/22
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// **************************************************************/
#ifndef __zce_service_h__
#define __zce_service_h__

#include <vector>
#include <string>
#include <zce/zce_config.h>


class ZCE_API zce_service
{
    enum _PROCESS_FLAG {
        PROCESS_FLAG_CONSOLE = 1,
        PROCESS_FLAG_SERVICE,
        PROCESS_FLAG_WORK,
    };
public:

    zce_service();

    virtual ~zce_service();

    virtual bool shutdown();

    virtual void interrupt();

    virtual void handle_interrupt(int);

    int main(int&, char*[]);

    static zce_service* instance();

    bool service() const;

    std::string name() const;

    int run(int&, char*[]);

    void wait_for_shutdown();

    bool stop();

#ifdef __HASVXML__
    std::vector<unsigned char> vec_pubkey_;
    unsigned lic_num_;
    unsigned lic_exp_;

    void initlicense(const char* name);
    void set_license(void* param);
    bool checklicense();
    unsigned getlicnum()const { return lic_num_; }
    unsigned getlicexp()const { return lic_exp_; }
#endif

protected:

    virtual bool on_start(int, char*[]) = 0;

    virtual bool on_stop() = 0;

    virtual void error(const std::string&);

    virtual void warning(const std::string&);

    virtual void trace(const std::string&);

    virtual void print(const std::string&);

    void enable_interrupt();

    void disable_interrupt();

    void print_object_stat();

private:
    unsigned    process_flag_;
    bool        nohup_;
    std::string name_;
    bool exit_success_;
    volatile bool running_;
    static zce_service* instance_;

#ifdef _WIN32
    int run_service(int, char*[]);
    void terminate_service(DWORD);
    bool wait_for_service_state(SC_HANDLE, DWORD, SERVICE_STATUS&);
    void show_service_status(const std::string&, SERVICE_STATUS&);
    SERVICE_STATUS_HANDLE status_handle_;
    std::vector<std::string> service_args_;
    HANDLE work_event_;
    HANDLE work_process_;
    
    std::string guid_;

public:
    void service_main(int, char*[]);
    void control(int);
#else
    int run_daemon(int, char*[]);
    int start_workprocess_posix();
    bool close_files_;
    std::string pid_file_;
    pid_t work_process_;
    const char* exepath_;
    int pipe_[2];
#endif

#ifdef _WIN32

    void configure_service(const std::string&);

    int install_service(bool, const std::string&, const std::string&, const std::string&,
        const std::vector<std::string>&);

    int uninstall_service(bool, const std::string&);

    int start_service(const std::string&, const std::vector<std::string>&);

    int stop_service(const std::string&);

    static void set_module_handle(HMODULE);

#else

#endif
};

#endif // __zce_service_h__
