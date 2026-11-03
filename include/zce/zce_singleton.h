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

// zce_sync.h brings <windows.h> through zce_inc.h, after <winsock2.h> as Windows requires;
// including <windows.h> here first made this header fail on its own with winsock redefinitions.
#include <zce/zce_sync.h>
#include <atomic>
#include <type_traits>
#include <typeinfo>

/**
 * @brief Where a zce::Singleton<T, L> keeps its instance: one slot per instantiation per process.
 *
 * The slots live in libzce, not in the template. A static data member of a class template is
 * instantiated in every module that uses it, so once libzce is a DLL or shared object and the
 * code using a singleton sits in plugins of its own, each module would hold its own instance and
 * its own lock: SchedulerSigt::instance() would be one Scheduler in zce.dll and another in each
 * plugin. Asking libzce for the slot makes every module of the process share one.
 *
 * Every module reads instance directly, so that instance() stays one acquire load on hot paths
 * such as BlockPoolSigt::instance(), which each data block allocation goes through. That ties the
 * slot's layout to the toolchain, as the rest of the module boundary already is (std::function,
 * std::string and zce::SmartPtr cross it in zvm_module.h): libzce and the modules using it are
 * built with the same compiler and standard library, which ZCE_BUILD_ID is there to check. The
 * assertions below pin what the direct read relies on, a lock-free atomic the size of a pointer.
 */
struct zce_singleton_slot_t {
    std::atomic<void*> instance{nullptr};
};
static_assert(std::atomic<void*>::is_always_lock_free,
              "zce_singleton_slot_t::instance must be a lock-free atomic");
static_assert(sizeof(std::atomic<void*>) == sizeof(void*),
              "zce_singleton_slot_t::instance must be pointer-sized");

// extern "C" gives these functions plain symbol names, for .def files and dynamic lists; it does
// not make zce_singleton_slot_t a C type (see above).
extern "C" {
/**
 * @brief The slot of the type @p type, made on first request and never freed.
 *
 * Types are told apart as std::type_info::operator== does it, which holds across modules: by
 * name for types with linkage, by identity for types local to one translation unit. A module
 * that has asked for a slot must stay loaded while the process runs.
 */
ZCE_API zce_singleton_slot_t* zce_singleton_slot(const std::type_info* type);

/** @brief Takes the slot's lock, which is recursive and shared by all modules. */
ZCE_API void zce_singleton_lock(zce_singleton_slot_t* slot);

/** @brief Releases the slot's lock, taken by zce_singleton_lock(). */
ZCE_API void zce_singleton_unlock(zce_singleton_slot_t* slot);
}

namespace zce {

class Object;

/**
 * @brief A process-wide instance of T, created on first use.
 *
 * instance() may be called from any thread: the pointer is published with release semantics and
 * read with acquire semantics, so a thread that sees it also sees the object fully constructed.
 * L serialises the creation; with L = MutexNull two threads that both make the first call may
 * each create one, so such a singleton must be first used from a single thread. Any other L
 * serialises it on the slot's lock, which is recursive as zce::Mutex is.
 *
 * The instance is shared by every module of the process (see zce_singleton_slot_t). Each
 * instantiation has its own slot, so Singleton<T> and Singleton<T, MutexNull> stay two singletons.
 */
template <typename T, typename L = zce::Mutex>
class Singleton {
  public:
    /** @brief Makes v the instance, without taking a reference to it. */
    static inline void setInstance(T* v) { slot()->instance.store(v, std::memory_order_release); }

    /** @brief The instance, created by the first call if setInstance() has not set one. */
    static inline T* instance() {
        zce_singleton_slot_t* s = slot();
        T* ptr = static_cast<T*>(s->instance.load(std::memory_order_acquire));
        if (ptr == nullptr) {
            SlotGuard g(s);
            ptr = static_cast<T*>(s->instance.load(std::memory_order_acquire));
            if (ptr == nullptr) {
                ptr = new T;
                // The singleton's own reference is counted before any other thread can see the
                // object: one that took and dropped the first SmartPtr to it would delete it.
                if constexpr (std::is_base_of<zce::Object, T>::value) {
                    ptr->__addref();
                }
                s->instance.store(ptr, std::memory_order_release);
            }
        }
        return ptr;
    }

    /** @brief Drops the instance; the next instance() creates a new one. */
    static inline void release() {
        zce_singleton_slot_t* s = slot();
        SlotGuard g(s);
        T* ptr = static_cast<T*>(s->instance.load(std::memory_order_acquire));
        if (ptr != nullptr) {
            if constexpr (std::is_base_of<zce::Object, T>::value) {
                ptr->__decref();
            } else {
                delete ptr;
            }
            s->instance.store(nullptr, std::memory_order_release);
        }
    }

  protected:
    Singleton() {}

    virtual ~Singleton() {
        zce_singleton_slot_t* s = slot();
        SlotGuard g(s);
        T* ptr = static_cast<T*>(s->instance.load(std::memory_order_acquire));
        if (ptr != nullptr) {
            delete ptr;
            s->instance.store(nullptr, std::memory_order_release);
        }
    }

  private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

  private:
    static constexpr bool kSerialised = !std::is_same<L, zce::MutexNull>::value;

    // Holds the slot's lock for a scope, or nothing when L is MutexNull.
    class SlotGuard {
      public:
        explicit SlotGuard(zce_singleton_slot_t* s) : slot_(kSerialised ? s : nullptr) {
            if (slot_ != nullptr) zce_singleton_lock(slot_);
        }
        ~SlotGuard() {
            if (slot_ != nullptr) zce_singleton_unlock(slot_);
        }
        SlotGuard(const SlotGuard&) = delete;
        SlotGuard& operator=(const SlotGuard&) = delete;

      private:
        zce_singleton_slot_t* slot_;
    };

    // Each module asks libzce once and keeps the answer; the slot is the same for all of them.
    static zce_singleton_slot_t* slot() {
        static zce_singleton_slot_t* const s = zce_singleton_slot(&typeid(Singleton));
        return s;
    }
};

}  // namespace zce
