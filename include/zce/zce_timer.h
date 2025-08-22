#pragma once
// ***************************************************************
//  Timer   version:  1.0   -  date: 2003/02/15
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
#include <functional>

namespace zce {

class Reactor;
class TaskQueue;
class TimerDoozer;

class ZCE_API Timer : public zce::Object
{
    friend class TimerDoozer;
    ZCE_OBJECT_DECLARE;
    struct pimpl;
    zce::SmartPtr<pimpl> pimpl_;

public:
    Timer(const zce::SmartPtr<Reactor>& reactor, 
        const zce::SmartPtr<TaskQueue>& syncque,
        unsigned msecond, 
        bool repeat = true);

    ~Timer();

    int getMilliSecondSpan() const;

    int start(const zce::SmartPtr<TimerDoozer>& doozer_ptr);

    int start(const std::function<void(void)>& cb, bool noaccumulate = false);

    void cancel();
};

class ZCE_API TimerDoozer : public zce::Object
{
    friend struct Timer::pimpl;

    bool noaccumulated_;
    AtomicLong accum_count_;

    void do_timeout();

public:
    TimerDoozer(bool noaccum = false);
    virtual bool will_trigger();
    virtual void handle_timeout() = 0;
};

}  // namespace zce
