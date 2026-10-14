#pragma once
// ***************************************************************
//  zce::Singleton   version:  1.0   -  date: 2002/03/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#if defined(_WIN32)
#    include <windows.h>
#endif
#include <zce/zce_sync.h>
#include <atomic>
#include <type_traits>

namespace zce {

/**
 * @brief A process-wide instance of T, created on first use.
 *
 * instance() may be called from any thread: the pointer is published with release semantics and
 * read with acquire semantics, so a thread that sees it also sees the object fully constructed.
 * L serialises the creation; with L = MutexNull two threads that both make the first call may
 * each create one, so such a singleton must be first used from a single thread.
 */
template <typename T, typename L = zce::Mutex>
class ZCE_API Singleton {
  public:
    /** @brief Makes v the instance, without taking a reference to it. */
    static inline void setInstance(T* v) { instance_.store(v, std::memory_order_release); }

    /** @brief The instance, created by the first call if setInstance() has not set one. */
    static inline T* instance() {
        T* ptr = instance_.load(std::memory_order_acquire);
        if (ptr == nullptr) {
            zce::Guard<L> g(lock_);
            ptr = instance_.load(std::memory_order_acquire);
            if (ptr == nullptr) {
                ptr = new T;
                // The singleton's own reference is counted before any other thread can see the
                // object: one that took and dropped the first SmartPtr to it would delete it.
                if constexpr (std::is_base_of<zce::Object, T>::value) {
                    ptr->__addref();
                }
                instance_.store(ptr, std::memory_order_release);
            }
        }
        return ptr;
    }

    /** @brief Drops the instance; the next instance() creates a new one. */
    static inline void release() {
        zce::Guard<L> g(lock_);
        T* ptr = instance_.load(std::memory_order_acquire);
        if (ptr != nullptr) {
            if constexpr (std::is_base_of<zce::Object, T>::value) {
                ptr->__decref();
            } else {
                delete ptr;
            }
            instance_.store(nullptr, std::memory_order_release);
        }
    }

  protected:
    Singleton() {}

    virtual ~Singleton() {
        zce::Guard<L> g(lock_);
        T* ptr = instance_.load(std::memory_order_acquire);
        if (ptr != nullptr) {
            delete ptr;
            instance_.store(nullptr, std::memory_order_release);
        }
    }

  private:
    Singleton(const Singleton&) {}
    Singleton& operator=(const Singleton&) {}

  private:
    static std::atomic<T*> instance_;
    static L lock_;
};

template <typename T, typename L>
std::atomic<T*> Singleton<T, L>::instance_{nullptr};

template <typename T, typename L>
L Singleton<T, L>::lock_;

}  // namespace zce
