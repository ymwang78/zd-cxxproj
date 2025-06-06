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
#include <zce/zce_task_queue.h>
#include <functional>
class zce_reactor;
class zce_task_queue;
class zce_timer_doozer;

class ZCE_API zce_timer : public zce_object
{
    friend class zce_timer_doozer;
    ZCE_OBJECT_DECLARE;
    struct pimpl;
    zce_smartptr<pimpl> pimpl_;

public:
    zce_timer(const zce_smartptr<zce_reactor>& reactor, 
        const zce_smartptr<zce_task_queue>& syncque,
        unsigned msecond, 
        bool repeat = true);

    ~zce_timer();

    int start(const zce_smartptr<zce_timer_doozer>& doozer_ptr);

    int start(const std::function<void(void)>& cb, bool noaccumulate = false);

    void cancel();

};

class ZCE_API zce_timer_doozer : public zce_object
{
    friend struct zce_timer::pimpl;

    bool noaccumulated_;
    zce_atomic_long accum_count_;

    void do_timeout();

public:
    zce_timer_doozer(bool noaccum = false);
    virtual bool will_trigger();
    virtual void handle_timeout() = 0;
};
