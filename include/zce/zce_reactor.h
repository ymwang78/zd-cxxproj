#pragma once
// ***************************************************************
//  zce::Reactor   version:  1.0     date: 2002/07/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include <zce/zce_object.h>
#include <zce/zce_task.h>
#include <deque>

namespace zce {

class Allocator;
class DnsResolve;
class Task;
class Thread;

class ZCE_API Reactor : public zce::TaskDelegator {
    struct pimpl;
    struct pimpl* pimpl_;

    friend class ReactorThread;

  public:
    Reactor();

    ~Reactor();

    unsigned long thread_id() const;

    void* loop_t() const;

    int set_thread_priority(int priority);

    int start();

    void stop();

    int dns_resolve(const std::string& domain, const zce_smartptr<zce::DnsResolve>& resolve_ptr);

    int delegateTask(const zce_smartptr<zce::Task>& task_ptr) override;

    int delegate_delay(const zce_smartptr<zce::Task>& task_ptr, int ms_second);

    int delegateRelease(zce_object* obj) override;

    void delegate_work();

    void* alloc(unsigned size);

    void zfree(void*);

    virtual int on_start() { return 0; };

  private:
    int loop();

    void terminate();
};

}  // namespace zce
