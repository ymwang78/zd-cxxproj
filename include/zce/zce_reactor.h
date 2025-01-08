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
#include <deque>

class zce_thread;
class zce_allocator;
class zce_task;
class zce_dnsresolve;

class ZCE_API zce_reactor : virtual public zce_object {
    struct pimpl;
    struct pimpl* pimpl_;

    friend class zce_reactor_thread;

  public:
    zce_reactor();

    ~zce_reactor();

    unsigned long thread_id() const;

    void* loop_t() const;

    int start();

    void stop();

    int dns_resolve(const std::string& domain, const zce_smartptr<zce_dnsresolve>& resolve_ptr);

    int delegate_task(const zce_smartptr<zce_task>& task_ptr, int mstimeafter = 0);

    void delegate_work();

    void* alloc(unsigned size);

    void zfree(void*);

    virtual int on_start() { return 0; };

    template <typename F>
    int delegate(bool bwait, const char* name, F f);

  private:
    int loop();

    void terminate();
};

#include <zce/zce_task.h>

template <typename F>
int zce_reactor::delegate(bool bwait, const char* name, F f) {
    class Fr_task : public zce_task {
        zce_smartptr<zce_reactor> reactor_ptr_;
        zce_semaphore* sem_;
        F f_;

      public:
        Fr_task(const char* name, zce_reactor* reactor_ptr, zce_semaphore* sem, F f)
            : zce_task(name ? name : "delegate_task"), reactor_ptr_(reactor_ptr), sem_(sem), f_(f) {
            if (sem_) {
                bool wait = sem_->try_acquire();
                ZCE_ASSERT_TEXT(wait, "deadlock detected!");
            }
        }
        virtual void call() {
            try {
                f_();
            } catch (const std::exception& ex) {
                ZCE_ASSERT_TEXT(false, ex.what());
            } catch (...) {
                ZCE_ASSERT_TEXT(false, "unknow exception");
            }
            if (sem_) sem_->release();
        }
    };

    if (bwait && zce_thread_id() == thread_id()) {
        ZCE_ASSERT(false);
        f();
        return 0;
    }

    zce_tss::global_t* tss = bwait ? zce_tss::get_global() : 0;
    zce_smartptr<zce_task> task_ptr(new Fr_task(name, this, bwait ? tss->sem_ : 0, f));
    int ret = delegate_task(task_ptr);
    if (ret >= 0 && bwait) {
        zce_guard<zce_semaphore> g(*tss->sem_);
    }
    return ret;
};
