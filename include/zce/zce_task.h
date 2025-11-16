#pragma once
// ***************************************************************
//  zce::Task   version:  1.0   -  date: 2003/03/27
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#pragma once

#include <zce/zce_config.h>
#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>
#include <zce/zce_thread.h>
#include <deque>
#include <future>
#include <optional>
#include <functional>

namespace zce {

struct TaskResultBase {
    enum class Status { Success, TaskException, SubmitFailed };
    Status status;
    std::string error_message;
    bool is_ok() const { return status == Status::Success; }
};

template <typename T>
struct TaskResult : public TaskResultBase {
    std::optional<T> value;

    static TaskResult<T> success(T val) {
        TaskResult<T> res;
        res.status = Status::Success;
        res.value = std::move(val);
        return res;
    }

    static TaskResult<T> error(Status s, std::string msg) {
        TaskResult<T> res;
        res.status = s;
        res.error_message = std::move(msg);
        return res;
    }
};

template <>
struct TaskResult<void> : public TaskResultBase {
    static TaskResult<void> success() {
        TaskResult<void> res;
        res.status = Status::Success;
        return res;
    }

    static TaskResult<void> error(Status s, std::string msg) {
        TaskResult<void> res;
        res.status = s;
        res.error_message = std::move(msg);
        return res;
    }
};

class ZCE_API Task : virtual public zce::Object {
  protected:
    const char* task_name_;

  public:
    Task(const char* const name);

    inline const char* const name() const { return task_name_; }

    virtual void call() = 0;
};

typedef zce::SmartPtr<Task> TaskPtr;

class ZCE_API TaskDelegator : virtual public zce::Object {
  public:
    virtual int delegateTask(const TaskPtr& task_ptr) = 0;

    virtual int delegateRelease(zce::Object* obj) = 0;

    // unknow buggy
    template <typename F, typename... Args>
    auto _delegateFuture(const char* name, F&& f, Args&&... args)
        -> std::shared_ptr<std::promise<TaskResult<decltype(f(args...))>>> {
        using ReturnType = decltype(f(args...));
        using ResultType = TaskResult<ReturnType>;

        auto promise_ptr = std::make_shared<std::promise<ResultType>>();

        class FutureTask : public Task {
            std::shared_ptr<std::promise<ResultType>> promise_;
            std::function<ReturnType()> func_;

          public:
            FutureTask(const std::shared_ptr<std::promise<ResultType>>& p, F&& f, Args&&... args)
                : Task("future_task_no_except"), promise_(p) {
                func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            }

            virtual void call() override {
                try {
                    if constexpr (std::is_same_v<ReturnType, void>) {
                        func_();
                        promise_->set_value(ResultType::success());
                    } else {
                        promise_->set_value(ResultType::success(func_()));
                    }
                } catch (const std::exception& e) {
                    promise_->set_value(
                        ResultType::error(ResultType::Status::TaskException, e.what()));
                } catch (...) {
                    promise_->set_value(ResultType::error(ResultType::Status::TaskException,
                                                          "An unknown error occurred."));
                }
            }
        };

        auto task = zce::SmartPtr<Task>(
            new FutureTask(promise_ptr, std::forward<F>(f), std::forward<Args>(args)...));

        int ret = delegateTask(task);

        if (ret != 0) {
            promise_ptr->set_value(
                ResultType::error(ResultType::Status::SubmitFailed, "Failed to schedule task."));
        }

        return promise_ptr;
    }

    template<typename F>
    class Fr_task : public zce::Task {
        zce::SmartPtr<zce::TaskDelegator> delegator_;
        zce::Semaphore* sem_;
        F f_;

      public:
        Fr_task(const char* name, zce::TaskDelegator* delegate_ptr, zce::Semaphore* sem, F f)
            : zce::Task(name ? name : "delegateTask"), delegator_(delegate_ptr), sem_(sem), f_(f) {
#ifdef _DEBUG
            if (sem_) {  // ensure sem is 0
                bool isget = sem_->try_acquire();
                ZCE_ASSERT_TEXT(!isget, "deadlock detected!");
                if (isget) sem_->release();
            }
#endif
        }
        virtual void call() {
            try {
                f_();
            } catch (const std::exception& ex) {
                ZCE_ASSERT_TEXT(false, ex.what());
            } catch (...) {
                ZCE_ASSERT_TEXT(false, "unknow exception");
            }
            if (sem_) sem_->release();
        }
    };

    template <typename F>
    int delegate(bool bwait, const char* name, F&& f) {
        #if 0
        auto promise_ptr = delegateFuture(name, std::forward<F>(f));
        if (bwait) {
            auto result_future = promise_ptr->get_future();
            ZCE_ASSERT(result_future.valid());
            result_future.wait();
        }
        #else
        if (bwait) {
            zce::Tss::zce_global_semaphore global_semaphore;
            zce::SmartPtr<zce::Task> task_ptr(new Fr_task(name, this, global_semaphore.sem, f));
            int ret = delegateTask(task_ptr);
            global_semaphore.sem->acquire();
            return ret;
        } else {
            zce::SmartPtr<zce::Task> task_ptr(new Fr_task(name, this, 0, f));
            return delegateTask(task_ptr);
        }
        #endif
        return 0;
    };
};

class ZCE_API Scheduler : public zce::Object {
    struct WorkerContext;

    class Worker;

    struct pimpl;
    struct pimpl* pimpl_;

    void doWork(WorkerContext& ctx);

  public:
    Scheduler();

    ~Scheduler();

    bool isActive() const;

    int active(int work_thread_cnt);

    void stop();

    int perform(const TaskPtr& req);

    int printCurrentTask();

    template <typename F>
    int perform(F f) {
        class FuncTask : public Task {
            F f_;

          public:
            FuncTask(F f) : zce::Task("FuncTask"), f_(f) {}
            virtual void call() { f_(); }
        };
        zce::SmartPtr<zce::Task> task_ptr(new FuncTask(f));
        return perform(task_ptr);
    };

    template <typename F, typename... Args>
    auto performFuture(F&& f, Args&&... args) -> std::future<TaskResult<decltype(f(args...))>> {
        using ReturnType = decltype(f(args...));
        using ResultType = TaskResult<ReturnType>;

        auto promise_ptr = std::make_shared<std::promise<ResultType>>();

        std::future<ResultType> result_future = promise_ptr->get_future();

        class FutureTask : public Task {
            std::shared_ptr<std::promise<ResultType>> promise_;
            std::function<ReturnType()> func_;

          public:
            FutureTask(std::shared_ptr<std::promise<ResultType>> p, F&& f, Args&&... args)
                : Task("future_task_no_except"), promise_(p) {
                func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            }

            virtual void call() override {
                try {
                    if constexpr (std::is_same_v<ReturnType, void>) {
                        func_();
                        promise_->set_value(ResultType::success());
                    } else {
                        promise_->set_value(ResultType::success(func_()));
                    }
                } catch (const std::exception& e) {
                    promise_->set_value(
                        ResultType::error(ResultType::Status::TaskException, e.what()));
                } catch (...) {
                    promise_->set_value(ResultType::error(ResultType::Status::TaskException,
                                                          "An unknown error occurred."));
                }
            }
        };

        auto task = zce::SmartPtr<Task>(
            new FutureTask(promise_ptr, std::forward<F>(f), std::forward<Args>(args)...));

        int ret = perform(task);

        if (ret != 0) {
            promise_ptr->set_value(
                ResultType::error(ResultType::Status::SubmitFailed, "Failed to schedule task."));
        }

        return result_future;
    }
};

#include <zce/zce_singleton.h>

typedef zce::Singleton<Scheduler> SchedulerSigt;

//////////////////////////////////////////////////////////////////////////

template <typename H, typename T0, typename T1, typename T2>
class DelegateTask : public Task {
    zce::SmartPtr<H> host_ptr_;
    T0 t0_;
    T1 t1_;
    T2 t2_;

  public:
    DelegateTask(H* h, const T0& t0, const T1& t1, const T2& t2)
        : Task("DelegateTask"), host_ptr_(h), t0_(t0), t1_(t1), t2_(t2) {}

    virtual void call() {
        if (host_ptr_) {
            host_ptr_->do_delegate(t0_, t1_, t2_);
        }
    }
};

}  // namespace zce
