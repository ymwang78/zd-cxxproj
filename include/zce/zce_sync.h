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
#include <zce/zce_inc.h>

namespace zce {

class ZCE_API Semaphore
{
    struct pimpl;
    struct pimpl *pimpl_;

public:
    Semaphore(int initcount);
    ~Semaphore();
    void acquire();
    void release();
    bool try_acquire();
private:
    Semaphore(const Semaphore&) {};// noncopyable
    void operator=(const Semaphore&) {};
};

class ZCE_API MutexNull
{
public:
    inline MutexNull() {};
    ~MutexNull() {};

    void acquire() {};
    void release() {};
    bool try_acquire() { return true; };
private:
    MutexNull(const MutexNull&) {};// noncopyable
    void operator=(const MutexNull&) {};
};

class ZCE_API Mutex
{
    struct pimpl;
    struct pimpl *pimpl_;

public:
    Mutex();
    ~Mutex();

    void acquire();
    void release();
	bool try_acquire();
private:
    Mutex(const Mutex&) {};// noncopyable
    void operator=(const Mutex&) {};
};

class ZCE_API MutexReadWrite
{
    struct pimpl;
    struct pimpl *pimpl_;

public:

    MutexReadWrite();

    ~MutexReadWrite();

    void acquire_read();

    void acquire_write();
    
    void release_read();

    void release_write();

private:
    MutexReadWrite(const MutexReadWrite&) {};// noncopyable
    void operator=(const MutexReadWrite&) {};
};

template<typename T>
class ZCE_API Guard
{
    T& lock_;
public:
    // = Initialization and termination methods.
    Guard(T& l):lock_(l)
    {
        lock_.acquire();
    };
    ~Guard()
    {
        lock_.release();
    };
};

template<typename T>
class ZCE_API GuardRead
{
    T& lock_;
public:
    // = Initialization and termination methods.
    GuardRead(T& l) :lock_(l)
    {
        lock_.acquire_read();
    };
    ~GuardRead()
    {
        lock_.release_read();
    };
};

template<typename T>
class ZCE_API GuardWrite
{
    T& lock_;
public:
    // = Initialization and termination methods.
    GuardWrite(T& l) :lock_(l)
    {
        lock_.acquire_write();
    };
    ~GuardWrite()
    {
        lock_.release_write();
    };
};

}  // namespace zce
