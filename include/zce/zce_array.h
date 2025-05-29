#pragma once

#include <vector>
#include <new>
#include <algorithm>
#include <zce/zce_atomic.h>
#include <zce/zce_log.h>

template <typename T, typename H = long long, bool is_auto_expand = false,
          bool is_auto_shrink = false>
class zce_array {  // skew heap

    struct mix_64 {
        static inline int next_magic() {
            static zce_atomic_long _magic(rand());
            int m = 0;
            do {
                m = (++_magic) & 0x7fffffff;
            } while (m == 0);
            return m;
        }

        static inline long long mix(int magic, int index) {
            return ((long long)magic << 32) | index;
        }

        static inline void seperate(long long handle, int& magic, int& index) {
            magic = handle >> 32;
            index = handle & 0xffffffff;
        }

        static const int LIMIT = 0x7fffffff;
    };

    struct mix_32 {
        static inline int next_magic() {
            static zce_atomic_long _magic(rand());
            int m = 0;
            do {
                m = (++_magic) & 0x7fff;
            } while (m == 0);
            return m;
        }

        static inline int mix(int magic, int index) {
            return ((int)(magic & 0x7fff) << 16) | index;
        }

        static inline void seperate(int handle, int& magic, int& index) {
            magic = handle >> 16;
            index = handle & 0xffff;
        }

        static const int LIMIT = 0xffff;
    };

    using mix_magic_t = typename std::conditional<sizeof(H) == 4, mix_32, mix_64>::type;

    struct slot_t {
        struct slot_t_data {
            unsigned int inuse : 1;
            unsigned int magic : 31;
            T item;
        };

        struct slot_t_node {
            int inuse : 1;
            int parent : 31;  // 为了方便删除，记录父节点
            int left;
            int right;
        };

        union {
            struct slot_t_data data_;
            struct slot_t_node heap_node_;
        };

        slot_t() : heap_node_{0, -1, -1, -1} {}
        ~slot_t() {}

        inline bool in_use() const { return data_.inuse; }

        // 显式定义拷贝构造函数
        slot_t(const slot_t& other) {
            if (other.in_use()) {
                data_.inuse = 1;
                data_.magic = other.data_.magic;
                new (&data_.item) T(other.data_.item);
            } else {
                heap_node_ = other.heap_node_;
            }
        }

        // 显式定义赋值运算符
        slot_t& operator=(const slot_t& other) {
            if (this != &other) {
                if (in_use()) {
                    data_.inuse = 0;
                    data_.magic = 0;
                    data_.item.~T();
                }
                if (other.in_use()) {
                    data_.inuse = 1;
                    data_.magic = other.data_.magic;
                    new (&data_.item) T(other.data_.item);
                } else {
                    heap_node_ = other.heap_node_;
                }
            }
            return *this;
        }
    };

    std::vector<slot_t> slots_;
    int free_head_;  // 堆的根节点索引，-1表示空
    int cur_top_;
    size_t used_count_;
    size_t capacity_limit_;

  public:
    explicit zce_array(size_t capacity, size_t capacity_limit = 0xffffffff)
        : free_head_(-1), cur_top_(0), used_count_(0), capacity_limit_(capacity_limit) {
        slots_.resize(capacity);
    }

    ~zce_array() { clear(); }

    size_t get_used_count() const { return used_count_; }

    template <typename U>
    H insert_item(U&& val) {
        int index;
        if (free_head_ != -1) {
            // 从堆中弹出最小元素
            index = free_head_;
            int left = slots_[index].heap_node_.left;
            int right = slots_[index].heap_node_.right;
            free_head_ = merge_skew_heap(left, right);
            if (free_head_ != -1) slots_[free_head_].heap_node_.parent = -1;
        } else {
            if (cur_top_ >= mix_magic_t::LIMIT) {
                return -1;  // 没有空间了
            }
            if (cur_top_ >= (int)slots_.size()) {
                if (is_auto_expand && slots_.size() * 2 <= capacity_limit_) {
                    slots_.resize(slots_.size() * 2);
                } else {
                    return -1;  // 没有空间了
                }
            }
            index = cur_top_++;
        }

        new (&slots_[index].data_.item) T(std::forward<U>(val));
        slots_[index].data_.inuse = 1;
        slots_[index].data_.magic = mix_magic_t::next_magic();
        ++used_count_;
        return mix_magic_t::mix(slots_[index].data_.magic, index);
    }

    H alloc_item() { return insert_item(T()); }

    bool is_valid(H handle) const noexcept {
        int index, magic;
        mix_magic_t::seperate(handle, magic, index);
        return index >= 0 && index < cur_top_ && slots_[index].in_use() &&
               magic == slots_[index].data_.magic;
    }

    void release_item(H handle) {
        int index, magic;
        mix_magic_t::seperate(handle, magic, index);

        ZCE_ASSERT_RETURN(index >= 0 && index < cur_top_ && slots_[index].in_use() &&
                              magic == slots_[index].data_.magic, );

        // 析构对象

        slots_[index].data_.item.~T();
        slots_[index].data_.inuse = 0;
        slots_[index].data_.magic = 0;
        --used_count_;

        if (index == cur_top_ - 1) {
            // 若释放的是最高索引的元素，可直接回退cur_top_
            --cur_top_;

            if (is_auto_shrink) {
                // 检查是否有连续的空闲元素
                while (cur_top_ > 0 && !slots_[cur_top_ - 1].in_use()) {
                    // 释放连续的空闲元素, 从skew heap中弹出
                    int cur_free = cur_top_ - 1;
                    remove_free_node(cur_free);
                    --cur_top_;
                }
            }
            return;
        }

        // 将该节点作为单独的小堆插入主堆
        slots_[index].heap_node_.inuse = 0;
        slots_[index].heap_node_.parent = -1;
        slots_[index].heap_node_.left = -1;
        slots_[index].heap_node_.right = -1;
        free_head_ = merge_skew_heap(free_head_, index);
    }

    // 强制设置一个空节点为当前值
    template <typename U>
    int set(H handle, U&& val) {
        int index, magic;
        mix_magic_t::seperate(handle, magic, index);
        ZCE_ASSERT_RETURN(
            (index >= cur_top_) || (index >= 0 && index < cur_top_ && !slots_[index].in_use()), -1);
        if (index < cur_top_) {
            remove_free_node(index);
        } else if (index == cur_top_) {
            cur_top_ = index + 1;
        } else {
            if (index > capacity_limit_) {
                return -1;  // 位置超限
            }
            if (index >= (int)slots_.size()) {
                slots_.resize(
                    std::min(capacity_limit_, std::max((size_t)index + 1, slots_.size() * 2)));
            }
            for (int i = cur_top_; i < index - 1; ++i) {
                slots_[i].heap_node_.inuse = 0;
                slots_[i].heap_node_.parent = -1;
                slots_[i].heap_node_.left = -1;
                slots_[i].heap_node_.right = -1;
                free_head_ = merge_skew_heap(free_head_, i);
            }
            cur_top_ = index + 1;
        }

        new (&slots_[index].data_.item) T(std::forward<U>(val));
        slots_[index].data_.inuse = 1;
        slots_[index].data_.magic = magic;
        ++used_count_;
        return 0;
    }

    T& operator[](H handle) {
        static T _empty;
        int index, magic;
        mix_magic_t::seperate(handle, magic, index);

        ZCE_ASSERT_RETURN(index >= 0 && index < cur_top_ && slots_[index].in_use() &&
                              magic == slots_[index].data_.magic,
                          _empty);
        return slots_[index].data_.item;
    }

    const T& operator[](H handle) const {
        static T _empty;
        int index, magic;
        mix_magic_t::seperate(handle, magic, index);

        ZCE_ASSERT_RETURN(index >= 0 && index < cur_top_ && slots_[index].in_use() &&
                              magic == slots_[index].data_.magic,
                          _empty);
        return slots_[index].data_.item;
    }

    void clear() {
        for (int i = 0; i < cur_top_; ++i) {
            if (slots_[i].in_use()) {
                slots_[i].data_.inuse = 0;
                slots_[i].data_.magic = 0;
                slots_[i].data_.item.~T();
            }
        }
        cur_top_ = 0;
        free_head_ = -1;
        used_count_ = 0;
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

    // 删除当前节点，返回新的子节点的根
    int remove_free_node(int nodeid) {
        int parent = slots_[nodeid].heap_node_.parent;
        int left = slots_[nodeid].heap_node_.left;
        int right = slots_[nodeid].heap_node_.right;
        int new_top = merge_skew_heap(left, right);
        if (parent != -1) {
            if (slots_[parent].heap_node_.left == nodeid) {
                slots_[parent].heap_node_.left = new_top;
            } else {
                ZCE_ASSERT(slots_[parent].heap_node_.right == nodeid);
                slots_[parent].heap_node_.right = new_top;
            }
        } else {
            ZCE_ASSERT(free_head_ == nodeid);
            free_head_ = new_top;
        }
        return new_top;
    }

  public:
    // 非const迭代器
    class iterator {
      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

      private:
        zce_array* array_;
        int index_;
        // 跳过未使用的槽
        void advance_to_valid() {
            while (index_ < array_->cur_top_ && !array_->slots_[index_].in_use()) {
                ++index_;
            }
        }

      public:
        iterator(zce_array* array, int index) : array_(array), index_(index) {
            if (array_) {
                advance_to_valid();
            }
        }
        iterator& operator++() {
            ++index_;
            advance_to_valid();
            return *this;
        }
        iterator operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        reference operator*() { return array_->slots_[index_].data_.item; }
        pointer operator->() { return &(operator*()); }
        bool operator==(const iterator& other) const {
            return array_ == other.array_ && index_ == other.index_;
        }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        // 允许 const_iterator 从非const迭代器构造
        friend class const_iterator;
    };

    // const 迭代器
    class const_iterator {
      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

      private:
        const zce_array* array_;
        int index_;
        void advance_to_valid() {
            while (index_ < array_->cur_top_ && !array_->slots_[index_].in_use()) {
                ++index_;
            }
        }

      public:
        const_iterator(const zce_array* array, int index) : array_(array), index_(index) {
            if (array_) {
                advance_to_valid();
            }
        }
        // 允许从非const迭代器转换
        const_iterator(const iterator& it) : array_(it.array_), index_(it.index_) {}

        const_iterator& operator++() {
            ++index_;
            advance_to_valid();
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        reference operator*() const { return array_->slots_[index_].data_.item; }
        pointer operator->() const { return &(operator*()); }
        bool operator==(const const_iterator& other) const {
            return array_ == other.array_ && index_ == other.index_;
        }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }
    };

    // 提供 begin/end 接口
    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, cur_top_); }
    const_iterator begin() const { return const_iterator(this, 0); }
    const_iterator end() const { return const_iterator(this, cur_top_); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
};

#include <unordered_map>
#include <functional>
#include <optional>

template <typename T, typename KeyType, typename GetKeyFunc>
class zce_indexed_array {
  public:
    zce_indexed_array() : key_func_([](const T& item) -> KeyType { return item.name; }) {}

    explicit zce_indexed_array(std::function<KeyType(const T&)> key_func)
        : key_func_(std::move(key_func)) {}

    template <typename T_S>
    int add(T_S&& s_param, bool replace_if_exists = false) {
        KeyType key = key_func_(s_param);
        auto it = name_to_index_.find(key);

        if (it != name_to_index_.end()) {
            if (replace_if_exists) {
                data_[it->second] = std::forward<T_S>(s_param);
                return (int)it->second;
            } else {
                ZCE_ASSERT(false);
                return -1;
            }
        } else {
            data_.push_back(std::forward<T_S>(s_param));
            name_to_index_[key] = data_.size() - 1;
            return (int)(data_.size() - 1);
        }
    }

    int get_index(const KeyType& key) const {
        auto it = name_to_index_.find(key);
        if (it != name_to_index_.end()) {
            return (int)it->second;
        }
        return -1;
    }

    std::optional<const T*> find(const KeyType& key) const {
        auto it = name_to_index_.find(key);
        if (it != name_to_index_.end()) {
            return &data_[it->second];
        }
        return std::nullopt;
    }

    const std::vector<T>& data() const { return data_; }

    T& operator[](size_t index) {
        if (index < data_.size()) {
            return data_[index];
        }
        throw std::out_of_range("Index out of bounds");
    }

    const T& operator[](size_t index) const {
        if (index < data_.size()) {
            return data_[index];
        }
        throw std::out_of_range("Index out of bounds");
    }

    void clear() {
        data_.clear();
        name_to_index_.clear();
    }

    void reserve(size_t n) {
        data_.reserve(n);
        name_to_index_.reserve(n);
    }

    size_t size() const { return data_.size(); }

  private:
    std::vector<T> data_;
    std::unordered_map<KeyType, size_t> name_to_index_;
    std::function<KeyType(const T&)> key_func_;
};
