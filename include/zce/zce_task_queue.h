#pragma once
// ***************************************************************
//  zce_task_queue   version:  1.0   -  date: 2002/11/15
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

class zce_schedule;

class ZCE_API zce_task_queue : public zce_task, public zce_task_delegator {
    ZCE_OBJECT_DECLARE;

  protected:
    zce_smartptr<zce_schedule> scheduler_ptr_;

    zce_atomic_long inque_;

    zce_smartptr<zce_task_queue> proxy_ptr_;

    std::deque<zce_smartptr<zce_task>> deque_;

    std::vector<zce_smartptr<zce_object>> release_vec_;

    zce_mutex task_lock_;

    unsigned cont_proc_;

    bool paused_;

  public:
    zce_task_queue(const zce_smartptr<zce_schedule>& scheduler_ptr, unsigned contproc = 10,
                   const char* name = 0);

    int try_queue_length();  // if locked return -1

    void pause();

    int resume();

    void attach(const zce_smartptr<zce_task_queue>&);

    int delegate_task(const zce_smartptr<zce_task>& task_ptr, int mstimeafter = 0) override;

    // 有些对象的释放有同步要求，必须在相关队列或者线程释放
    int delegate_release(zce_object* obj) override;

    virtual void call();

};


template <typename QueueSubType, typename Params, typename Results>
class zce_task_map_reduce : public zce_object {
    zce_smartptr<QueueSubType>* queue_vec_;
    size_t queue_size_;
    zce_atomic_long remain_tasks_;
    std::vector<int> queue_idx_vec_;
    Params params_;
    Results result_;

  public:
    template <typename QueueIdxVecType, typename ParamsType, typename ResultsType>
    zce_task_map_reduce(zce_smartptr<QueueSubType>* queue_vec, size_t queue_size,
                        QueueIdxVecType&& queue_idx_vec, ParamsType&& params, ResultsType&& result)
        : queue_vec_(queue_vec),
          queue_size_(queue_size),
          queue_idx_vec_(std::forward<QueueIdxVecType>(queue_idx_vec)),
          params_(std::forward<ParamsType>(params)),
          result_(std::forward<ResultsType>(result)) {}

    template <typename ProcFunc, typename FinalFunc>
    int map_reduce(ProcFunc process_task, FinalFunc final_callback) {
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

        zce_smartptr<zce_task_map_reduce> this_ptr(this);
        for (size_t i = 0; i < queue_size_; ++i) {
            if (!if_work_queue[i]) continue;
            queue_vec_[i]->delegate(false, __FUNCTION__, [=]() {
                process_task((int)i, this_ptr->queue_idx_vec_, this_ptr->params_,
                             this_ptr->result_);
                if (--this_ptr->remain_tasks_ == 0) {
                    final_callback(this_ptr->params_, this_ptr->result_);
                }
            });
        }

        return 0;
    }
};
