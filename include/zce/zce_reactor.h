#pragma once
// ***************************************************************
//  zce_reactor   version:  1.0     date: 2002/07/31
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

class zce_thread;
class zce_allocator;
class zce_task;
class zce_dnsresolve;

class ZCE_API zce_reactor : public zce_task_delegator {
    struct pimpl;
    struct pimpl* pimpl_;

    friend class zce_reactor_thread;

  public:
    zce_reactor();

    ~zce_reactor();

    unsigned long thread_id() const;

    void* loop_t() const;

    int set_thread_priority(int priority);

    int start();

    void stop();

    int dns_resolve(const std::string& domain, const zce_smartptr<zce_dnsresolve>& resolve_ptr);

    int delegate_task(const zce_smartptr<zce_task>& task_ptr, bool wait = false) override;

    int delegate_delay(const zce_smartptr<zce_task>& task_ptr, int ms_second);

    int delegate_release(zce_object* obj) override;

    void delegate_work();

    void* alloc(unsigned size);

    void zfree(void*);

    virtual int on_start() { return 0; };

  private:
    int loop();

    void terminate();
};
