#pragma once
// ***************************************************************
//  zce_thread   version:  1.0   -  date: 2003/02/15
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

class ZCE_API zce_thread : virtual public zce_object {
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

    zce_thread(const char* name = NULL);
    virtual ~zce_thread();

    virtual void run() = 0;
    virtual void terminate() = 0;

    int start();
    void join();
    int set_priority(THREAD_PRIORITY v);
    unsigned long id() const;

    bool operator==(const zce_thread&) const;
    bool operator!=(const zce_thread&) const;
    bool operator<(const zce_thread&) const;

    void __runi();
    void __done();

  private:
    zce_thread(const zce_thread&){};      // Copying is forbidden
    void operator=(const zce_thread&){};  // Assignment is forbidden
};

typedef zce_smartptr<zce_thread> zce_thread_ptr;
