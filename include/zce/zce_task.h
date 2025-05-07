#pragma once
// ***************************************************************
//  zce_task   version:  1.0   -  date: 2003/03/27
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#pragma once

#include <zce/zce_config.h>
#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>
#include <zce/zce_thread.h>
#include <deque>

class ZCE_API zce_task : virtual public zce_object {
  protected:
    const char* task_name_;

  public:
    zce_task(const char* const name);

    inline const char* const name() const { return task_name_; }

    virtual void call() = 0;
};

typedef zce_smartptr<zce_task> zce_task_ptr;

class ZCE_API zce_task_delegator : virtual public zce_object {
  public:
    virtual int delegate_task(const zce_smartptr<zce_task>& task_ptr, int mstimeafter = 0, bool wait = false) = 0;

    virtual int delegate_release(zce_object* obj) = 0;

    template <typename F>
    int delegate(bool bwait, const char* name, F f);
};

template <typename F>
int zce_task_delegator::delegate(bool bwait, const char* name, F f) {
    class Fr_task : public zce_task {
        zce_smartptr<zce_task_delegator> delegator_;
        zce_semaphore* sem_;
        F f_;

      public:
        Fr_task(const char* name, zce_task_delegator* delegate_ptr, zce_semaphore* sem, F f)
            : zce_task(name ? name : "delegate_task"), delegator_(delegate_ptr), sem_(sem), f_(f) {
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

    if (bwait) {
        zce_tss::zce_global_semaphore global_semaphore;
        zce_smartptr<zce_task> task_ptr(new Fr_task(name, this, global_semaphore.sem, f));
        int ret = delegate_task(task_ptr, 0, true);
        global_semaphore.sem->acquire();
        return ret;
    } else {
        zce_smartptr<zce_task> task_ptr(new Fr_task(name, this, 0, f));
        return delegate_task(task_ptr, 0, false);
    }
};

class ZCE_API zce_schedule : public zce_object {
    struct zce_worker_contex;

    class zce_worker;

    struct pimpl;
    struct pimpl* pimpl_;

    void do_work(zce_worker_contex& ctx);

  public:
    zce_schedule();

    ~zce_schedule();

    int active(int work_thread_cnt);

    void stop();

    int perform(const zce_smartptr<zce_task>& req);

    int print_curtask();

    template <typename F>
    int perform(F f) {
        class F_task : public zce_task {
            F f_;

          public:
            F_task(F f) : zce_task("F_task"), f_(f) {}
            virtual void call() { f_(); }
        };
        zce_smartptr<zce_task> task_ptr(new F_task(f));
        return perform(task_ptr);
    };
};

#include <zce/zce_singleton.h>

typedef zce_singleton<zce_schedule> zce_schedule_sigt;

//////////////////////////////////////////////////////////////////////////

template <typename H, typename T0, typename T1, typename T2>
class zce_delegate_task : public zce_task {
    zce_smartptr<H> host_ptr_;
    T0 t0_;
    T1 t1_;
    T2 t2_;

  public:
    zce_delegate_task(H* h, const T0& t0, const T1& t1, const T2& t2)
        : zce_task("zce_delegate_task"), host_ptr_(h), t0_(t0), t1_(t1), t2_(t2) {}

    virtual void call() {
        if (host_ptr_) {
            host_ptr_->do_delegate(t0_, t1_, t2_);
        }
    }
};
