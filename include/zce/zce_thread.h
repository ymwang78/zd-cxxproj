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

class ZCE_API Thread : virtual public zce_object {

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

    virtual void run() = 0;
    virtual void terminate() = 0;

    int start();
    void join();
    int set_priority(THREAD_PRIORITY v);
    unsigned long id() const;

    bool operator==(const Thread&) const;
    bool operator!=(const Thread&) const;
    bool operator<(const Thread&) const;

    void __runi();
    void __done();

  private:

    Thread(const Thread&) {};      // Copying is forbidden

    void operator=(const Thread&) {};  // Assignment is forbidden
};

typedef zce_smartptr<Thread> zce_thread_ptr;

}  // namespace zce
