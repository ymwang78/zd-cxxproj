#pragma once
// ***************************************************************
//  TaskQueue   version:  1.0   -  date: 2002/11/15
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
#include <zce/zce_task.h>
#include <zce/zce_atomic.h>
#include <deque>
#include <algorithm>

namespace zce {

class Scheduler;

class ZCE_API TaskQueue : public Task, public TaskDelegator {
    ZCE_OBJECT_DECLARE;

  protected:
    zce::SmartPtr<zce::Scheduler> scheduler_ptr_;

    zce::AtomicLong inque_;

    zce::SmartPtr<TaskQueue> proxy_ptr_;

    std::deque<zce::SmartPtr<Task>> deque_;

    std::vector<zce::SmartPtr<zce::Object>> release_vec_;

    zce::Mutex task_lock_;

    unsigned cont_proc_;

    bool paused_;

  public:
    TaskQueue(const zce::SmartPtr<Scheduler>& scheduler_ptr, unsigned contproc = 10,
              const char* name = 0);

    const zce::SmartPtr<Scheduler>& scheduler_ptr() const noexcept { return scheduler_ptr_; }

    int try_queue_length();  // if locked return -1

    void pause();

    int resume();

    void attach(const zce::SmartPtr<TaskQueue>&);

    int delegateTask(const zce::SmartPtr<Task>& task_ptr) override;

    // 有些对象的释放有同步要求，必须在相关队列或者线程释放
    int delegateRelease(zce::Object* obj) override;

    virtual void call();
};

using TaskQueuePtr = zce::SmartPtr<TaskQueue>;

template <typename QueueSubType>
class TaskMapReduce : public zce::Object {
    zce::SmartPtr<QueueSubType>* queue_vec_;
    size_t queue_size_;
    zce::AtomicLong remain_tasks_;
    std::vector<int> queue_idx_vec_;  // task assign queue idx

  public:
    using ProcFunc = std::function<void(int, const std::vector<int>&)>;

    using FinalFunc = std::function<void(size_t)>;

    TaskMapReduce(zce::SmartPtr<QueueSubType>* queue_vec, size_t queue_size,
                  std::vector<int>&& queue_idx_vec)
        : queue_vec_(queue_vec),
          queue_size_(queue_size),
          queue_idx_vec_(std::forward<std::vector<int>>(queue_idx_vec)) {}

    int mapReduce(ProcFunc process_task, FinalFunc final_callback) {
        if (queue_vec_ == 0 || queue_size_ == 0 || queue_idx_vec_.empty()) {
            return ZCE_ERROR_INVALID;
        }

        std::vector<bool> if_work_queue(queue_size_, false);
        for (int queue_idx : queue_idx_vec_) {
            if (queue_idx < 0 || queue_idx >= static_cast<int>(queue_size_)) {
                return ZCE_ERROR_INVALID;
            }
            if_work_queue[queue_idx] = true;
        }

        size_t work_queue_count = std::count(if_work_queue.begin(), if_work_queue.end(), true);
        if (work_queue_count == 0) return ZCE_ERROR_INVALID;

        remain_tasks_ = (long)work_queue_count;

        zce::SmartPtr<TaskMapReduce> this_ptr(this);
        for (size_t i = 0; i < queue_size_; ++i) {
            if (!if_work_queue[i]) continue;
            queue_vec_[i]->delegate(false, __FUNCTION__, [=]() {
                process_task((int)i, this_ptr->queue_idx_vec_);
                if (--this_ptr->remain_tasks_ == 0) {
                    final_callback(work_queue_count);
                }
            });
        }

        return 0;
    }
};

}  // namespace zce