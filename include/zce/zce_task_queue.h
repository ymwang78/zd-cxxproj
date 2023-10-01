// ***************************************************************
//  zce_task_queue   version:  1.0   -  date: 11/15/2015
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once
#ifndef __zce_task_queue_h__
#define __zce_task_queue_h__

#include <zce/zce_config.h>
#include <zce/zce_smartptr.h>
#include <zce/zce_task.h>
#include <deque>

class zce_schedule;

class ZCE_API zce_task_queue : public zce_task
{
    ZCE_OBJECT_DECLARE;
protected:

    zce_smartptr<zce_schedule> schedule_ptr_;

    zce_atomic_long inque_;

    zce_smartptr<zce_task_queue> proxy_ptr_;

    std::deque<zce_smartptr<zce_task> > deque_;

    zce_mutex task_lock_;

    unsigned cont_proc_;

    bool paused_;

public:

    zce_task_queue(const zce_smartptr<zce_schedule>& schedule_ptr, unsigned contproc = 10, const char* name = 0);

    int enqueue(const zce_smartptr<zce_task>& req);

	int try_queue_length(); //if locked return -1

    void pause();

    int resume();

    void attach(const zce_smartptr<zce_task_queue>&);

    virtual void call();

    template<typename F>
    int enqueue(F f);
};

template<typename F>
int zce_task_queue::enqueue(F f)
{
    //enqueue can't use wait because work queue maybe the same with current working
    //so deadlock if it happened
    class Tq_task : public zce_task {
        F f_;
    public:
        Tq_task(F f) : zce_task("Tq_task"), f_(f) {
        }
        virtual void call() {
            f_();
        }
    };

    zce_smartptr<zce_task> task_ptr(new Tq_task(f));
    return enqueue(task_ptr);
};

#endif // __zce_task_queue_h__
