#pragma once
#include <zce/zce_object.h>
#include <zce/zce_sync.h>
#include <type_traits>

template <typename T, typename Lock = zce_mutex_null>
class zce_ring : public zce_object {
  public:
    zce_ring(const zce_ring&) = delete;

    zce_ring& operator=(const zce_ring&) = delete;

    explicit zce_ring(size_t capacity) : _capacity(capacity), _size(0), _head(0), _tail(0) {
        _buffer.resize(capacity);
    }

    zce_ring(zce_ring&& other) noexcept {
        zce_guard<Lock> lock(_mutex);
        _buffer = std::move(other._buffer);
        _capacity = other._capacity;
        _size = other._size;
        _head = other._head;
        _tail = other._tail;
        other._capacity = 0;
        other._size = 0;
        other._head = 0;
        other._tail = 0;
    }

    zce_ring& operator=(zce_ring&& other) noexcept {
        if (this != &other) {
            zce_guard<Lock> lock(_mutex);
            zce_guard<Lock> lock_other(other._mutex);
            _buffer = std::move(other._buffer);
            _capacity = other._capacity;
            _size = other._size;
            _head = other._head;
            _tail = other._tail;
            other._capacity = 0;
            other._size = 0;
            other._head = 0;
            other._tail = 0;
        }
        return *this;
    }

    /** ��ն��У���������ָ�� */
    void reset() {
        zce_guard<Lock> lock(_mutex);
        _head = 0;
        _tail = 0;
        _size = 0;
    }

    /** ���Ԫ�ص����У��Զ������������ݣ� */
    void push(const T& item) {
        zce_guard<Lock> lock(_mutex);

        _buffer[_tail] = item;
        _tail = (_tail + 1) % _capacity;

        if (_size == _capacity) {
            _head = (_head + 1) % _capacity;  // �����������������ϵ�����
        } else {
            ++_size;
        }
    }

    /** ��ȡ����Ԫ�أ��������Ϊ�գ������ȴ��� */
    bool pop(T& item) {
        zce_guard<Lock> lock(_mutex);
        if (_size == 0) return false;

        item = _buffer[_head];
        _head = (_head + 1) % _capacity;
        --_size;

        return true;
    }

    /** ����д�루֧�� `memcpy` ���٣��Զ������������ݣ� */
    void write(size_t count, const T* src) {
        zce_guard<Lock> lock(_mutex);

        if (count >= _capacity) {
            // ���д�����ݳ���������������ֻ�������µ� `_capacity` ������
            src += (count - _capacity);
            count = _capacity;
            _head = 0;
            _tail = 0;
            _size = 0;
        }

        size_t first_part = std::min(count, _capacity - _tail);
        size_t second_part = count - first_part;

        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memcpy(&_buffer[_tail], src, first_part * sizeof(T));
            if (second_part > 0) {
                std::memcpy(&_buffer[0], src + first_part, second_part * sizeof(T));
            }
        } else {
            std::copy(src, src + first_part, _buffer.begin() + _tail);
            if (second_part > 0) {
                std::copy(src + first_part, src + count, _buffer.begin());
            }
        }

        _tail = (_tail + count) % _capacity;

        if (_size + count > _capacity) {
            _head = (_head + (_size + count - _capacity)) % _capacity;
            _size = _capacity;
        } else {
            _size += count;
        }
    }

    /** ��ȡ���Ԫ�أ�ɾ���Ѷ�ȡ���ݣ�֧�� `memcpy` ���٣� */
    size_t read(size_t count, T* dst) {
        zce_guard<Lock> lock(_mutex);
        size_t available = peek_no_lock(count, dst);

        _head = (_head + available) % _capacity;
        _size -= available;

        return available;
    }

    /** Ԥ�����Ԫ�أ������޸� `_head`��֧�� `memcpy` ���٣� */
    size_t peek(size_t count, T* dst) {
        zce_guard<Lock> lock(_mutex);
        return peek_no_lock(count, dst);
    }

    /** �����Ƿ�Ϊ�� */
    bool empty() const {
        zce_guard<Lock> lock(_mutex);
        return _size == 0;
    }

    /** �����Ƿ����� */
    bool full() const {
        zce_guard<Lock> lock(_mutex);
        return _size == _capacity;
    }

    /** ��ȡ��ǰԪ�ظ��� */
    size_t size() const {
        zce_guard<Lock> lock(_mutex);
        return _size;
    }

    /** ��ȡ�������� */
    size_t capacity() const { return _capacity; }

  private:
    size_t peek_no_lock(size_t count, T* dst) {
        size_t available = std::min(count, _size);

        if (available == 0) return 0;

        size_t first_part = std::min(available, _capacity - _head);
        size_t second_part = available - first_part;

        // �Ż�������ǻ������ͣ�ʹ�� `memcpy` ����
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memcpy(dst, &_buffer[_head], first_part * sizeof(T));
            if (second_part > 0) {
                std::memcpy(dst + first_part, &_buffer[0], second_part * sizeof(T));
            }
        } else {
            std::copy(_buffer.begin() + _head, _buffer.begin() + _head + first_part, dst);
            if (second_part > 0) {
                std::copy(_buffer.begin(), _buffer.begin() + second_part, dst + first_part);
            }
        }
        return available;
    }

    std::vector<T> _buffer;  // ���λ�����
    size_t _capacity;        // �������
    size_t _size;            // ��ǰԪ�ظ���
    size_t _head;            // ͷ��������ȡλ�ã�
    size_t _tail;            // β������д��λ�ã�

    mutable Lock _mutex;
};
