// ***************************************************************
//  zce_task   version:  1.0   -  date: 4/27/2012
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_task_h__
#define __zce_task_h__

#include <zce/zce_config.h>
#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>
#include <deque>



class ZCE_API zce_task : virtual public zce_object
{
protected:

    const char* task_name_;

public:

    zce_task(const char* const name);

    inline const char* const name() const {
        return task_name_;
    }

    virtual void call() = 0;
};

typedef zce_smartptr<zce_task> zce_task_ptr;


class ZCE_API zce_schedule : public zce_object
{
    struct zce_worker_contex;

    class zce_worker;

    struct pimpl;
    struct pimpl *pimpl_;

    void do_work(zce_worker_contex& ctx);

public:
    zce_schedule();

    ~zce_schedule();

    int active(int work_thread_cnt);

    void stop();

    int perform(const zce_smartptr<zce_task>& req);

    int print_curtask();

	template<typename F>
	int perform(F f) {
		class F_task : public zce_task
		{
			F f_;
		public:
			F_task(F f) : zce_task("F_task"), f_(f) {
			}
			virtual void call() {
				f_();
			}
		};
		zce_smartptr<zce_task> task_ptr(new F_task(f));
		return perform(task_ptr);
	};
};

#include <zce/zce_singleton.h>

typedef zce_singleton<zce_schedule> zce_schedule_sigt;

//////////////////////////////////////////////////////////////////////////

template <typename H, typename T0, typename T1, typename T2>
class zce_delegate_task : public zce_task
{
    zce_smartptr<H> host_ptr_;
    T0 t0_;
    T1 t1_;
    T2 t2_;

public:
    zce_delegate_task(H* h, const T0& t0, const T1& t1, const T2& t2)
        :zce_task("zce_delegate_task"),
        host_ptr_(h), t0_(t0), t1_(t1), t2_(t2) {

    }

    virtual void call() {
        if (host_ptr_) {
            host_ptr_->do_delegate(t0_, t1_, t2_);
        }
    }
};

#endif
