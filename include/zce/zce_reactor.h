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

    int set_thread_priority(int priority);

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
#ifdef _DEBUG
            if (sem_) {  // ensure sem is 0
                bool isget = sem_->try_acquire();
                ZCE_ASSERT_TEXT(!isget, "deadlock detected!");
                if (isget) sem_->release();
            }
#endif
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

    if (bwait) {
        zce_tss::zce_global_semaphore global_semaphore;
        zce_smartptr<zce_task> task_ptr(new Fr_task(name, this, global_semaphore.sem, f));
        int ret = delegate_task(task_ptr);
        global_semaphore.sem->acquire();
        return ret;
    } else {
        zce_smartptr<zce_task> task_ptr(new Fr_task(name, this, 0, f));
        return delegate_task(task_ptr);
    }
};
