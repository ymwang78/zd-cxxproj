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
#include <functional>
class zce_reactor;
class zce_task_queue;

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
        const zce_smartptr<zce_task_queue>& syncque,
        unsigned msecond, 
        bool repeat = true);

    ~zce_timer();

    int start(const std::function<void(void)>& cb);

    void cancel();

    void handle_timeout();

    void on_close();

};
