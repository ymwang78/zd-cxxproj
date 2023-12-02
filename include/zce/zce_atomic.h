#ifndef __zce_atomic_h__
#define __zce_atomic_h__

/*
    ATOMIC_USE_ARMSPIN: arm SINGLE core
    ATOMIC_USE_GCCBUILTIN: http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html
    ATOMIC_USE_MUTEX: use one zce_mutex to implement atomic
*/

#include <zce/zce_inc.h>

#if defined(__linux) && defined(__arm__) && defined(ATOMIC_ARM_SINGLECORE) && (!defined(ATOMIC_USE_MUTEX))
//#define ATOMIC_USE_ARMSPIN
#endif

#if defined(ATOMIC_USE_MUTEX) || defined(ATOMIC_USE_ARMSPIN)
extern long ext_atomic_add(volatile long *atomic, long val);
#endif

class ZCE_API zce_atomic_long
{
public:
    zce_atomic_long(long c = 0) noexcept;

    long operator ++ ();
    long operator -- ();

    long value() const;
private:
    volatile long value_;
};

inline zce_atomic_long::zce_atomic_long(long c) noexcept
: value_ (c)
{
}

inline long zce_atomic_long::operator ++ ()
{
#ifdef _WIN32
    return InterlockedIncrement(const_cast<long*>(&this->value_));
#elif defined(__APPLE__)
    return OSAtomicIncrement32((int*)&this->value_);
#elif defined(ATOMIC_USE_MUTEX) || defined(ATOMIC_USE_ARMSPIN)
    return ext_atomic_add(&this->value_, 1);
#else
    return __sync_add_and_fetch(&this->value_, 1);
#endif
}

inline long zce_atomic_long::operator -- ()
{
#ifdef _WIN32
    return InterlockedDecrement(const_cast<long*>(&this->value_));
#elif defined (__APPLE__)
    return OSAtomicDecrement32((int*)&this->value_);
#elif defined(ATOMIC_USE_MUTEX) || defined(ATOMIC_USE_ARMSPIN)
     return ext_atomic_add(&this->value_, -1);
#else
    return __sync_add_and_fetch(&this->value_, -1);
#endif
}

inline long zce_atomic_long::value() const
{
    return this->value_;
}

class ZCE_API zce_atomic_long_guard
{
    zce_atomic_long& atomic_;
    long value_;

public:
    zce_atomic_long_guard(zce_atomic_long& v)
    :atomic_(v){
        value_ = ++atomic_;
    }

    ~zce_atomic_long_guard() {
        --atomic_;
    }

    long value() const {
        return value_;
    }
};

#endif // __zce_atomic_h__
