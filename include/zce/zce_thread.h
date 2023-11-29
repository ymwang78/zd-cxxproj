/* ***************************************************************
//  zce_thread   version:  1.0  date: 6/5/2015
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// **************************************************************/
#ifndef __zce_thread_h__
#define __zce_thread_h__

#include <zce/zce_object.h>
#include <zce/zce_sync.h>

class ZCE_API zce_thread : virtual public zce_object
{
    struct pimpl;
    struct pimpl* pimpl_;

public:
    zce_thread(const char* name = NULL);
    virtual ~zce_thread();

    virtual void run() = 0;
    virtual void terminate() = 0;

    int start();
    void join();
    unsigned long id() const;

    bool operator==(const zce_thread&) const;
    bool operator!=(const zce_thread&) const;
    bool operator<(const zce_thread&) const;

    void __runi();
    void __done();

private:
    zce_thread(const zce_thread&) {};         // Copying is forbidden
    void operator=(const zce_thread&) {};      // Assignment is forbidden
};

typedef zce_smartptr<zce_thread> zce_thread_ptr;

#endif // __zce_thread_h__
