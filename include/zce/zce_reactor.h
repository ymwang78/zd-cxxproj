#pragma once
// ***************************************************************
//  zce::Reactor   version:  1.0     date: 2002/07/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include <zce/zce_object.h>
#include <zce/zce_task.h>
#include <zce/zce_singleton.h>
#include <deque>
#include <functional>

namespace zce {

class Allocator;
class DnsResolve;
class Task;
class Thread;
class Timer;

class ZCE_API Reactor : public zce::TaskDelegator {
    struct pimpl;
    struct pimpl* pimpl_;

    friend class ReactorThread;
    friend class ReactorInPlaceThread;

  public:
    Reactor(const char* name = nullptr);

    ~Reactor();

    const std::string& name() const;

    unsigned long thread_id() const;

    bool isStart() const;

    void* loop_t() const;

    int setThreadPriority(int priority);

    // won't start loop if in_place is true, caller should call startLoop() to start loop

    int start(bool in_place = false);

    int startLoop();

    void stop();

    int dns_resolve(const std::string& domain, const zce::SmartPtr<zce::DnsResolve>& resolve_ptr);

    SmartPtr<Timer> scheduleTimer(zce::SmartPtr<TaskQueue> task_queue, int msec_span, bool repeat,
                                  std::function<void(Timer*)> timer_func);

    int delegateTask(const zce::SmartPtr<zce::Task>& task_ptr) override;

    int delegate_delay(const zce::SmartPtr<zce::Task>& task_ptr, int ms_second);

    int delegateRelease(zce::Object* obj) override;

    void delegate_work();

    void* alloc(unsigned size);

    void zfree(void*);

    virtual int onReactorStart() { return 0; };

    virtual void onReactorStop() {};

  private:
    int loop();

    void terminate();
};

typedef ::zce::SmartPtr<::zce::Reactor> ReactorPtr;

typedef ::zce::Singleton<::zce::Reactor, ::zce::MutexNull> ReactorSigt;

}  // namespace zce
