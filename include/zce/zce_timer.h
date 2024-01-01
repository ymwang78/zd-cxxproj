#pragma once
// ***************************************************************
//  zce_timer   version:  1.0   -  date: 2003/02/15
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
class zce_reactor;
class zce_task_queue;

class ZCE_API zce_timer_doozer : virtual public zce_object
{
public:
    virtual void handle_timeout(const zce_any& arg) = 0;
};

class ZCE_API zce_timer : public zce_object
{
    ZCE_OBJECT_DECLARE;
    struct pimpl;
    struct pimpl *pimpl_;

    class zce_timer_delegate;
    class zce_timer_handler;

    void do_delegate(bool bstart);

public:
    zce_timer(const zce_smartptr<zce_reactor>& reactor, 
        const zce_smartptr<zce_timer_doozer>& doozerptr, 
        unsigned msecond, 
        bool repeat = true, 
        const zce_any& arg = zce_any());

    ~zce_timer();

    void set_queue(const zce_smartptr<zce_task_queue>&);

    void start();

    void cancel();

    void handle_timeout();

    void on_close();
};

//deprecated, use zce_timer_queue_tpl instead
template<typename _t>
class zce_timer_tpl : public zce_timer_doozer
{
public:
    zce_timer_tpl(_t* p) : p_(p) {
    }
public:
    void handle_timeout(const zce_any& arg) {
        p_->handle_timeout(arg);
    }
private:
    _t*    p_;
};

#include <zce/zce_task.h>
#include <zce/zce_task_queue.h>

template<typename _t>
class zce_timer_queue_tpl : public zce_timer_doozer, public zce_task
{
public:
	zce_timer_queue_tpl(const zce_smartptr<_t>& p, zce_smartptr<zce_task_queue> queueptr) 
		: zce_task("zce_timer_queue_tpl"), p_(p), queue_ptr_(queueptr) {
	}

    void stop() { //should sync in same queue
        p_ = 0;
    }

	void handle_timeout(const zce_any& arg) { 
        if (p_ == 0) //run in reactor thread, may race, just for conv
            return;
		arg_ = arg;
		queue_ptr_->enqueue(zce_smartptr<zce_task>(this));
	}

	virtual void call() {
        zce_smartptr<_t> p = p_;
        if (p) {
            p->handle_timeout(arg_);
        }
	}
private:
	zce_smartptr<zce_task_queue> queue_ptr_;
	zce_any arg_;
	zce_smartptr<_t> p_;
};
