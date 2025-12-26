#pragma once
// ***************************************************************
//  zce::Thread   version:  1.0   -  date: 2003/02/15
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include <zce/zce_object.h>
#include <zce/zce_sync.h>

namespace zce {

class ZCE_API Thread : virtual public Object {

    struct pimpl;
    struct pimpl* pimpl_;

  public:
    typedef enum _THREAD_PRIORITY {
        PRIORITY_HIGHEST = 2,
        PRIORITY_ABOVE_NORMAL = 1,
        PRIORITY_NORMAL = 0,
        PRIORITY_BELOW_NORMAL = -1,
        PRIORITY_LOWEST = -2,
    } THREAD_PRIORITY;

    Thread(const char* name = NULL);

    virtual ~Thread();

    const char* getName() const;

    virtual void onThreadStart() = 0;

    virtual void onThreadTerminate() = 0;

    int startThread(bool in_place = false);
    
    void joinThread();
    
    int setThreadPriority(THREAD_PRIORITY v);
    
    unsigned long getThreadId() const;

    bool operator==(const Thread&) const;
    
    bool operator!=(const Thread&) const;
    
    bool operator<(const Thread&) const;

  private:

    Thread(const Thread&) : Object() {};      // Copying is forbidden

    void operator=(const Thread&) {};  // Assignment is forbidden
};

typedef SmartPtr<Thread> zce_thread_ptr;

}  // namespace zce
