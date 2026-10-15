#pragma once
// ***************************************************************
//  zce_atomic   version:  1.0  date: 2002/05/23
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

/*
    ATOMIC_USE_ARMSPIN: arm SINGLE core
    ATOMIC_USE_GCCBUILTIN: http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html
    ATOMIC_USE_MUTEX: use one zce::Mutex to implement atomic
*/

#include <zce/zce_inc.h>

#if defined(__linux) && defined(__arm__) && defined(ATOMIC_ARM_SINGLECORE) && \
    (!defined(ATOMIC_USE_MUTEX))
// #define ATOMIC_USE_ARMSPIN
#endif

#if defined(ATOMIC_USE_MUTEX) || defined(ATOMIC_USE_ARMSPIN)
extern long ext_atomic_add(volatile long* atomic, long val);
#endif

namespace zce {

class ZCE_API AtomicLong {
  public:
    AtomicLong(long c = 0) noexcept;

    long operator++() noexcept;
    long operator--() noexcept;

    long value() const noexcept;

  private:
    volatile long value_;
};

inline AtomicLong::AtomicLong(long c) noexcept : value_(c) {}

// GCC and Clang, Apple's included, use the compiler builtins on the whole long. Apple used to go
// through OSAtomicIncrement32/Decrement32 on a cast int*, but long is 64-bit on every Apple target
// (LP64), so only its low half moved: counting 0 down to -1 left value() reading 4294967295, and
// past INT_MAX ++ returned a negative int.
inline long AtomicLong::operator++() noexcept {
#ifdef _WIN32
    return InterlockedIncrement((long*)(&this->value_));
#elif defined(ATOMIC_USE_MUTEX) || defined(ATOMIC_USE_ARMSPIN)
    return ext_atomic_add(&this->value_, 1);
#else
    return __sync_add_and_fetch(&this->value_, 1);
#endif
}

inline long AtomicLong::operator--() noexcept {
#ifdef _WIN32
    return InterlockedDecrement((long*)(&this->value_));
#elif defined(ATOMIC_USE_MUTEX) || defined(ATOMIC_USE_ARMSPIN)
    return ext_atomic_add(&this->value_, -1);
#else
    return __sync_add_and_fetch(&this->value_, -1);
#endif
}

// Other threads may be counting while this one reads (Object::__get_ref_count(), a Socket's
// close_refcnt_, the ObjectMonitor counts), so the read has to be atomic as well: a plain read of
// value_ is a data race, and ThreadSanitizer reports it against the increments. It is ordered like
// them: ReadAcquire is the same instruction as a sequentially consistent load on x86/x64 and ARM64.
inline long AtomicLong::value() const noexcept {
#ifdef _WIN32
    return ReadAcquire(&this->value_);
#elif defined(ATOMIC_USE_MUTEX) || defined(ATOMIC_USE_ARMSPIN)
    return ext_atomic_add(const_cast<volatile long*>(&this->value_), 0);
#else
    return __atomic_load_n(&this->value_, __ATOMIC_SEQ_CST);
#endif
}

class ZCE_API AtomicLongGuard {
    AtomicLong& atomic_;
    long value_;

  public:
    inline AtomicLongGuard(AtomicLong& v) : atomic_(v) { value_ = ++atomic_; }

    inline ~AtomicLongGuard() { --atomic_; }

    inline long value() const { return value_; }
};

}  // namespace zce
