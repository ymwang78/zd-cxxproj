#pragma once

#include <zce/zce_list.h>
#include <vector>
#include <new>  // for placement new

template <typename T, bool is_auto_shrink = false>
class zce_array {  // skew heap
    struct slot_t {
        union {
            struct {
                int magic;
                T item;
            } data_;
            struct {
                int parent;  // 为了方便删除，记录父节点
                int left;
                int right;
            } heap_node_;
        };
        slot_t() : heap_node_{-1, -1, -1} {}
        ~slot_t() {}

        // 显式定义拷贝构造函数
        slot_t(const slot_t& other) {
            if (other.in_use_) {
                data_.magic = other.data_.magic;
                new (&data_.item) T(other.data_.item);
            } else {
                heap_node_ = other.heap_node_;
            }
        }

        // 显式定义赋值运算符
        slot_t& operator=(const slot_t& other) {
            if (this != &other) {
                if (in_use_) {
                    data_.item.~T();
                }
                if (other.in_use_) {
                    data_.magic = other.data_.magic;
                    new (&data_.item) T(other.data_.item);
                } else {
                    heap_node_ = other.heap_node_;
                }
            }
            return *this;
        }

        bool in_use_ = false;
    };

    std::vector<slot_t> slots_;
    int free_head_;  // 堆的根节点索引，-1表示空
    int cur_top_;

  public:
    explicit zce_array(size_t capacity) : free_head_(-1), cur_top_(0) { slots_.resize(capacity); }

    ~zce_array() {
        clear();
    }

    template <typename U>
    long long insert_item(U&& val) {
        int index;
        if (free_head_ != -1) {
            // 从堆中弹出最小元素
            index = free_head_;
            int left = slots_[index].heap_node_.left;
            int right = slots_[index].heap_node_.right;
            free_head_ = merge_skew_heap(left, right);
            if (free_head_ != -1) slots_[free_head_].heap_node_.parent = -1;
        } else {
            if (cur_top_ >= (int)slots_.size()) {
                return -1;  // 没有空间了
            }
            index = cur_top_++;
        }

        static zce_atomic_long _magic;
        new (&slots_[index].data_.item) T(std::forward<U>(val));
        slots_[index].data_.magic = (++_magic) & 0x7fffffff;
        slots_[index].in_use_ = true;
        return ((long long)slots_[index].data_.magic << 32) | index;
    }

    long long alloc_item() { return insert_item(T()); }

    bool is_valid(long long handle) const noexcept {
        int index = handle & 0xffffffff;
        int magic = handle >> 32;
        return index >= 0 && index < cur_top_ && slots_[index].in_use_ &&
               magic == slots_[index].data_.magic;
    }

    void release_item(long long handle) {
        int index = handle & 0xffffffff;
        int magic = handle >> 32;

        ZCE_ASSERT_RETURN(index >= 0 && index < cur_top_ && slots_[index].in_use_ &&
                              magic == slots_[index].data_.magic, );

        // 析构对象

        slots_[index].data_.item.~T();
        slots_[index].data_.magic = 0;
        slots_[index].in_use_ = false;

        if (index == cur_top_ - 1) {
            // 若释放的是最高索引的元素，可直接回退cur_top_
            --cur_top_;

            if (is_auto_shrink) {
                // 检查是否有连续的空闲元素
                while (cur_top_ > 0 && !slots_[cur_top_ - 1].in_use_) {
                    // 释放连续的空闲元素, 从skew heap中弹出
                    int cur_free = cur_top_ - 1;
                    int parent = slots_[cur_free].heap_node_.parent;

                    int left = slots_[cur_free].heap_node_.left;
                    int right = slots_[cur_free].heap_node_.right;
                    int new_top = merge_skew_heap(left, right);
                    if (parent != -1) {
                        if (slots_[parent].heap_node_.left == cur_free) {
                            slots_[parent].heap_node_.left = new_top;
                        } else {
                            ZCE_ASSERT(slots_[parent].heap_node_.right == cur_free);
                            slots_[parent].heap_node_.right = new_top;
                        }
                        // slots_[cur_free].heap_node_.parent = -1;
                        // free_head_ = merge_skew_heap(free_head_, cur_free);
                    } else {
                        ZCE_ASSERT(free_head_ == cur_free);
                        free_head_ = new_top;
                    }

                    --cur_top_;
                }
            }
            return;
        }

        // 将该节点作为单独的小堆插入主堆
        slots_[index].heap_node_.parent = -1;
        slots_[index].heap_node_.left = -1;
        slots_[index].heap_node_.right = -1;
        free_head_ = merge_skew_heap(free_head_, index);
    }

    T& operator[](long long handle) {
        static T _empty;
        int index = handle & 0xffffffff;
        int magic = handle >> 32;
        ZCE_ASSERT_RETURN(index >= 0 && index < cur_top_ && slots_[index].in_use_ &&
                              magic == slots_[index].data_.magic,
                          _empty);
        return slots_[index].data_.item;
    }

    const T& operator[](long long handle) const {
        static T _empty;
        int index = handle & 0xffffffff;
        int magic = handle >> 32;
        ZCE_ASSERT_RETURN(index >= 0 && index < cur_top_ && slots_[index].in_use_ &&
                              magic == slots_[index].data_.magic,
                          _empty);
        return slots_[index].data_.item;
    }

    void clear() {
        for (int i = 0; i < cur_top_; ++i) {
            if (slots_[i].in_use_) {
                slots_[i].data_.magic = 0;
                slots_[i].data_.item.~T();
                slots_[i].in_use_ = false;
            }
        }
        cur_top_ = 0;
        free_head_ = -1;
    }

  private:
    // Skew Heap合并函数
    // 将堆root1和root2合并成一个并返回新根节点
    int merge_skew_heap(int root1, int root2) {
        if (root1 == -1) return root2;
        if (root2 == -1) return root1;

        // 比较索引大小保证堆根是最小索引
        if (root2 < root1) {
            // 交换root1和root2确保root1是较小的索引
            int temp = root1;
            root1 = root2;
            root2 = temp;
        }

        // root1作为根
        // Skew Heap的merge:
        // 合并root1的右子树和root2
        int r = slots_[root1].heap_node_.right;
        slots_[root1].heap_node_.right = merge_skew_heap(r, root2);

        // Skew Heap特性：交换左右子树
        int l = slots_[root1].heap_node_.left;
        int new_r = slots_[root1].heap_node_.right;
        if (new_r != -1) {
            slots_[new_r].heap_node_.parent = root1;
        }
        // 交换左右子树
        slots_[root1].heap_node_.left = new_r;
        slots_[root1].heap_node_.right = l;

        return root1;
    }
};