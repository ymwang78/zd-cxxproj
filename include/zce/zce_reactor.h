// ***************************************************************
//  zce_reactor   version:  1.0     date: 07/31/2002
//  -------------------------------------------------------------
//  Yongming Wang(ymwang@iipc.zju.edu.cn)
//  The Institute of System Engineering, Zhejiang University
//  -------------------------------------------------------------
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_reactor_h__
#define __zce_reactor_h__

#include <zce/zce_smartptr.h>
#include <deque>

class zce_thread;
class zce_allocator;
class zce_task;
class zce_dnsresolve;

class ZCE_API zce_reactor : virtual public zce_smartptr_mtbase
{
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

	template<typename F>
	int delegate(bool bwait, F f);

private:

    int loop();

    void terminate();
};

#include <zce/zce_task.h>

template<typename F>
int zce_reactor::delegate(bool bwait, F f)
{
	class Fr_task : public zce_task
	{
		F f_;
        zce_semaphore* sem_;
	public:
		Fr_task(zce_semaphore* sem, F f) : zce_task("Fr_task"), sem_(sem), f_(f) {
            if (sem_) {
                bool wait = sem_->try_acquire();
                ZCE_ASSERT_TEXT(wait, "deadlock detected!");
            }
		}
		virtual void call() {
			f_();
            if (sem_)
                sem_->release();
		}
	};

    if (bwait && zce_thread_id() == thread_id()) {
        ZCE_ASSERT(false);
        f();
        return 0;
    }

    zce_tss::global_t* tss = bwait ? zce_tss::get_global() : 0;
    Fr_task* task = new Fr_task(bwait ? tss->sem_ : 0, f);
	zce_smartptr<zce_task> task_ptr(task);
	int ret = delegate_task(task_ptr);
    if (ret >= 0 && bwait) {
        zce_guard<zce_semaphore> g(*tss->sem_);
    }
    return ret;
};

#endif //__zce_reactor_h__
