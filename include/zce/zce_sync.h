#pragma once
// ***************************************************************
//  zce_sync   version:  1.0   -  date: 2003/03/27
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_mutex_h__
#define __zce_mutex_h__

#include <zce/zce_inc.h>

class ZCE_API zce_semaphore
{
    struct pimpl;
    struct pimpl *pimpl_;

public:
    zce_semaphore(int initcount);
    ~zce_semaphore();
    void acquire();
    void release();
    bool try_acquire();
private:
    zce_semaphore(const zce_semaphore&) {};// noncopyable
    void operator=(const zce_semaphore&) {};
};

class ZCE_API zce_mutex_null
{
public:
    inline zce_mutex_null() {};
    ~zce_mutex_null() {};

    void acquire() {};
    void release() {};
    bool try_acquire() { return true; };
private:
    zce_mutex_null(const zce_mutex_null&) {};// noncopyable
    void operator=(const zce_mutex_null&) {};
};

class ZCE_API zce_mutex
{
    struct pimpl;
    struct pimpl *pimpl_;

public:
    zce_mutex();
    ~zce_mutex();

    void acquire();
    void release();
	bool try_acquire();
private:
    zce_mutex(const zce_mutex&) {};// noncopyable
    void operator=(const zce_mutex&) {};
};

class ZCE_API zce_mutex_rw
{
    struct pimpl;
    struct pimpl *pimpl_;

public:

    zce_mutex_rw();

    ~zce_mutex_rw();

    void acquire_read();

    void acquire_write();
    
    void release_read();

    void release_write();

private:
    zce_mutex_rw(const zce_mutex_rw&) {};// noncopyable
    void operator=(const zce_mutex_rw&) {};
};

template<typename T>
class ZCE_API zce_guard
{
    T& lock_;
public:
    // = Initialization and termination methods.
    zce_guard(T& l):lock_(l)
    {
        lock_.acquire();
    };
    ~zce_guard()
    {
        lock_.release();
    };
};

template<typename T>
class ZCE_API zce_guard_read
{
    T& lock_;
public:
    // = Initialization and termination methods.
    zce_guard_read(T& l) :lock_(l)
    {
        lock_.acquire_read();
    };
    ~zce_guard_read()
    {
        lock_.release_read();
    };
};

template<typename T>
class ZCE_API zce_guard_write
{
    T& lock_;
public:
    // = Initialization and termination methods.
    zce_guard_write(T& l) :lock_(l)
    {
        lock_.acquire_write();
    };
    ~zce_guard_write()
    {
        lock_.release_write();
    };
};



#endif // __zce_mutex_h__
