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
#include <atomic>
#include <optional>
#include <utility>

namespace zce {

class ZCE_API Semaphore {
    struct pimpl;
    struct pimpl* pimpl_;

  public:
    Semaphore(int initcount);
    ~Semaphore();
    void acquire();
    void release();
    bool try_acquire();

  private:
    Semaphore(const Semaphore&) {};  // noncopyable
    void operator=(const Semaphore&) {};
};

class ZCE_API MutexNull {
  public:
    inline MutexNull() {};
    ~MutexNull() {};

    void acquire() {};
    void release() {};
    bool try_acquire() { return true; };

  private:
    MutexNull(const MutexNull&) {};  // noncopyable
    void operator=(const MutexNull&) {};
};

class ZCE_API Mutex {
    struct pimpl;
    struct pimpl* pimpl_;

  public:
    Mutex();
    ~Mutex();

    void acquire();
    void release();
    bool try_acquire();

  private:
    Mutex(const Mutex&) {};  // noncopyable
    void operator=(const Mutex&) {};
};

class ZCE_API MutexReadWrite {
    struct pimpl;
    struct pimpl* pimpl_;

  public:
    MutexReadWrite();

    ~MutexReadWrite();

    void acquire_read();

    void acquire_write();

    void release_read();

    void release_write();

  private:
    MutexReadWrite(const MutexReadWrite&) {};  // noncopyable
    void operator=(const MutexReadWrite&) {};
};

template <typename T>
class ZCE_API Guard {
    T& lock_;

  public:
    // = Initialization and termination methods.
    Guard(T& l) : lock_(l) { lock_.acquire(); };
    ~Guard() { lock_.release(); };
};

template <typename T>
class ZCE_API GuardRead {
    T& lock_;

  public:
    // = Initialization and termination methods.
    GuardRead(T& l) : lock_(l) { lock_.acquire_read(); };
    ~GuardRead() { lock_.release_read(); };
};

template <typename T>
class ZCE_API GuardWrite {
    T& lock_;

  public:
    // = Initialization and termination methods.
    GuardWrite(T& l) : lock_(l) { lock_.acquire_write(); };
    ~GuardWrite() { lock_.release_write(); };
};

template <typename T>
class LockRead {
    const T& obj_;
    zce::MutexReadWrite& lock_;

  public:
    LockRead(const T& obj, zce::MutexReadWrite& lock) : obj_(obj), lock_(lock) {
        lock_.acquire_read();
    }

    ~LockRead() { lock_.release_read(); }

    const T* operator->() const { return &obj_; }

    const T& operator*() const { return obj_; }

    const T& get() const { return obj_; }

    struct TempUnLock {
        LockRead& lock_read_;
        TempUnLock(LockRead& l) : lock_read_(l) { lock_read_.lock_.release_read(); };
        ~TempUnLock() { lock_read_.lock_.acquire_read(); }
    };

    struct TempUnLock tempUnlock() { return TempUnLock(*this); };

    struct TempLockWrite {
        LockRead& lock_read_;

        TempLockWrite(LockRead& l) : lock_read_(l) {
            lock_read_.lock_.release_read();
            lock_read_.lock_.acquire_write();
        };
        ~TempLockWrite() { 
            lock_read_.lock_.release_write();
            lock_read_.lock_.acquire_read();
        }

        T* operator->() const { return (T*)(&lock_read_.obj_); }

        T& operator*() const { return lock_read_.obj_; }

        T& get() const { return lock_read_.obj_; }
    };

    struct TempLockWrite tempLockWrite() { return TempLockWrite(*this); }
};

template <typename T>
class LockWrite {
    zce::MutexReadWrite& lock_;
    T& obj_;

  public:
    LockWrite(T& obj, zce::MutexReadWrite& lock) : obj_(obj), lock_(lock) { lock_.acquire_write(); }

    ~LockWrite() { lock_.release_write(); }

    T* operator->() const { return &obj_; }

    T& operator*() const { return obj_; }

    T& get() const { return obj_; }

    struct TempUnLock {
        LockWrite& lock_;
        TempUnLock(LockWrite& l) : lock_(l) { lock_.lock_.release_write(); };
        ~TempUnLock() { lock_.lock_.acquire_write(); }
    };

    struct TempUnLock tempUnlock() { return TempUnLock(*this); };
};

template <typename T>
class Lock {
    zce::Mutex& lock_;
    T& obj_;

  public:
    Lock(T& obj, zce::Mutex& lock) : obj_(obj), lock_(lock) { lock_.acquire(); }

    ~Lock() { lock_.release(); }

    T* operator->() const { return &obj_; }

    T& operator*() const { return obj_; }

    T& get() const { return obj_; }

    struct TempUnLock {
        Lock& lock_read_;
        TempUnLock(Lock& l) : lock_read_(l) { lock_read_.lock_.release(); };
        ~TempUnLock() { lock_read_.lock_.acquire(); }
    };

    struct TempUnLock tempUnlock() { return TempUnLock(*this); };
};

// execution permit

class ExecPermit {
  public:
    class ExecPermitGuard {
      public:
        ExecPermitGuard() noexcept : state_(nullptr) {}

        explicit ExecPermitGuard(std::atomic<int>* state) noexcept : state_(state) {}

        ~ExecPermitGuard() noexcept { release(); }

        ExecPermitGuard(const ExecPermitGuard&) = delete;
        ExecPermitGuard& operator=(const ExecPermitGuard&) = delete;

        ExecPermitGuard(ExecPermitGuard&& other) noexcept : state_(other.state_) {
            other.state_ = nullptr;
        }

        ExecPermitGuard& operator=(ExecPermitGuard&& other) noexcept {
            if (this != &other) {
                release();
                state_ = other.state_;
                other.state_ = nullptr;
            }
            return *this;
        }

        bool valid() const noexcept { return state_ != nullptr; }

        explicit operator bool() const noexcept { return valid(); }

        void release() noexcept {
            if (state_ != nullptr) {
                state_->store(0, std::memory_order_release);
                state_ = nullptr;
            }
        }

      private:
        std::atomic<int>* state_;
    };

    ExecPermit() noexcept : state_(0) {}

    ExecPermit(const ExecPermit&) = delete;
    ExecPermit& operator=(const ExecPermit&) = delete;

    std::optional<ExecPermitGuard> tryAcquire() noexcept {
        int expected = 0;
        bool ok = state_.compare_exchange_strong(expected, 1, std::memory_order_acquire,
                                                 std::memory_order_relaxed);
        if (!ok) {
            return std::nullopt;
        }
        return ExecPermitGuard(&state_);
    }

    bool isBusy() const noexcept { return state_.load(std::memory_order_relaxed) != 0; }

  private:
    std::atomic<int> state_;
};

}  // namespace zce
