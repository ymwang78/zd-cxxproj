#pragma once
// ***************************************************************
//  zce_any   version:  1.0   -  date: 2023/08/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <zce/zce_bits.h>
#include <zce/zce_log.h>
#include <zce/zce_object.h>
#include <zce/zce_types.h>

#include <map>
#include <vector>
#include <limits>

class zce_object;
class zce_dblock;

class ZCE_API zce_any {
    enum _any_types {
        any_int64 = 0,
        any_double,
        any_rawptr,
        any_ipv4,
        any_ipv6,
        any_datetime,

        any_object,
        any_dblock,
        any_intarray,
        any_fltarray,
        any_dblarray,
        any_str,        // string always as a 0 in tail, but not calculated in length
        any_boolarray,  // not fully supported
        any_dict,
        any_vector,
    };

    static constexpr int MAX_UNION_BYTES = sizeof(struct in6_addr);

    struct {
        union {
            zce_int64 i64_[MAX_UNION_BYTES / sizeof(zce_double)];
            zce_double dbl_[MAX_UNION_BYTES / sizeof(zce_double)];
            void* rawptr_[MAX_UNION_BYTES / sizeof(void*)];
            zce_byte bytearray_inplace_[MAX_UNION_BYTES];
            char str_inplace_[MAX_UNION_BYTES];
            struct in_addr ipv4_;
            struct in6_addr ipv6_;

            zce_object* obj_;
            zce_dblock* dblock_;  // string or bytearray limit to 4M, use dblock if larger
            zce_byte* bytearray_;
            char* str_;
            std::vector<bool>* boolvec_;
            std::map<zce_any, zce_any>* dict_;
            std::vector<zce_any>* vec_;
        } u_;
        zce_uint16 len_or_port_;
        zce_uint16 subtype_indicate_;  // defined by app
        zce_uint32 quality_ : 16;
        zce_uint32 padding_ : 6;
        zce_uint32 type_ : 5;
        zce_uint32 shiftbits_ : 3;
        zce_uint32 signed_ : 1;
        zce_uint32 outplace_ : 1;
    } data_;

    template <typename T>
    constexpr _any_types _to_type() const {
        if constexpr (std::is_same<T, zce_float>::value) {
            return any_fltarray;
        } else if constexpr (std::is_same<T, zce_double>::value) {
            return any_dblarray;
        } else if constexpr (std::is_same<T, char>::value) {
            return any_str;
        } else if constexpr (std::is_integral<T>::value) {
            return any_intarray;
        } else {
            static_assert(std::is_same<T, zce_byte>::value, "must be integral type");
            return any_intarray;
        }
    }

    // called by template constructor
    zce_any(const zce_byte* buf, size_t len, _any_types any_types, bool issigned,
            int shiftbits) noexcept;

  public:
    zce_any(const zce_any& rhs);

    zce_any(zce_any&& rhs) noexcept;

    ~zce_any();

    zce_any& operator=(const zce_any& rhs) {
        if (this != &rhs) {
            this->~zce_any();
            new (this) zce_any(rhs);
        }
        return *this;
    }

    bool operator==(const zce_any& rhs) const noexcept;

    bool operator!=(const zce_any& rhs) const noexcept { return !(operator==(rhs)); }

    bool operator<(const zce_any& rhs) const noexcept;

    zce_any(int i32 = 0) noexcept : data_{} {
        data_.type_ = any_int64;
        data_.u_.i64_[0] = i32;
    }

    zce_any(zce_int64 i64, bool is_datetime = false) noexcept : data_{} {
        data_.type_ = is_datetime ? any_datetime : any_int64;
        data_.u_.i64_[0] = i64;
    }

    zce_any(zce_double dbl) noexcept : data_{} {
        data_.type_ = any_double;
        data_.u_.dbl_[0] = dbl;
    }

    zce_any(const void* raw) noexcept : data_{} {
        data_.type_ = any_rawptr;
        data_.u_.rawptr_[0] = (void*)raw;
    }

    zce_any(zce_object* obj) noexcept : data_{} {
        data_.type_ = any_object;
        data_.u_.obj_ = obj;
        if (data_.u_.obj_) data_.u_.obj_->__addref();
    }

    template <typename T>
    zce_any(const T* barray, size_t len) noexcept {
        new (this) zce_any((const zce_byte*)barray, (size_t)len * sizeof(T), _to_type<T>(),
                           std::is_signed<T>::value, zce_bits_msb_index(sizeof(T)));
    }

    zce_any(const std::vector<bool>& vec) noexcept;

    zce_any(const char* str, size_t len) noexcept;

    zce_any(const char* str) noexcept;

    zce_any(const zce_byte* buf, size_t len, zce_byte fixarr_u) noexcept;

    zce_any(const struct in_addr& ipv4, unsigned short port) noexcept : data_{} {
        data_.type_ = any_ipv4;
        data_.u_.ipv4_ = ipv4;
        data_.len_or_port_ = port;
    }

    zce_any(const struct in6_addr& ipv6, unsigned short port) noexcept : data_{} {
        data_.type_ = any_ipv6;
        data_.u_.ipv6_ = ipv6;
        data_.len_or_port_ = port;
    }

    static zce_any create_dict();

    static zce_any create_vector();

    static zce_any create_dblock();

    static zce_any create_datetime_from_msec(zce_int64 msec);

    inline int get_type() const noexcept { return data_.type_; }

    inline bool is_i64() const noexcept { return data_.type_ == any_int64; }

    inline zce_int64 i64() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_int64, 0);
        return data_.u_.i64_[0];
    }

    inline void i64(zce_int64 v) noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_int64, );
        data_.u_.i64_[0] = v;
    }

    inline bool is_double() const noexcept { return data_.type_ == any_double; }

    inline zce_double dbl() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_double || data_.type_ == any_int64, 0.0);
        if (data_.type_ == any_double) {
            return data_.u_.dbl_[0];
        }
        if (data_.type_ == any_int64) {
            constexpr zce_int64 max_precise = (1ll << 52) - 1;
            if (data_.u_.i64_[0] > max_precise) return std::numeric_limits<double>::infinity();
            if (data_.u_.i64_[0] < -max_precise) return -std::numeric_limits<double>::infinity();
            return (zce_double)data_.u_.i64_[0];
        }
        return 0.0;
    }

    inline void dbl(zce_double v) noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_double || data_.type_ == any_int64, );
        if (data_.type_ == any_int64) {
            data_.type_ = any_double;
        }
        data_.u_.dbl_[0] = v;
    }

    // detail type for int or double, byte, short, etc. specific type defined by apps
    inline zce_uint16 subtype_indicate() const noexcept { return data_.subtype_indicate_; }

    inline void subtype_indicate(zce_uint16 v) noexcept { data_.subtype_indicate_ = v; }

    inline bool is_object() const noexcept { return data_.type_ == any_object; }

    inline zce_object* object() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_object, 0);
        return data_.u_.obj_;
    }

    inline bool is_dblock() const noexcept { return data_.type_ == any_dblock; }

    inline zce_dblock* dblock() noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_dblock, 0);
        return data_.u_.dblock_;
    }

    inline const zce_dblock* dblock() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_dblock, 0);
        return data_.u_.dblock_;
    }

    int get_array_raw(const zce_byte*& buf, size_t& len, zce_byte& fixarr_v) const noexcept;

    inline bool is_array() const noexcept {
        return data_.type_ == any_intarray || data_.type_ == any_fltarray ||
               data_.type_ == any_dblarray;
    }

    template <typename T>
    inline const T* array() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == _to_type<T>() && sizeof(T) == 1 << data_.shiftbits_, 0);
        if (data_.outplace_) return (T*)data_.u_.bytearray_;
        return (T*)data_.u_.bytearray_inplace_;
    }

    inline int arraylen() const noexcept {
        ZCE_ASSERT_RETURN(is_array(), 0);

        if (data_.outplace_) return (data_.padding_ << 16) | data_.len_or_port_;
        return data_.padding_;
    }

    inline bool is_str() const noexcept { return data_.type_ == any_str; }

    inline const char* str() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_str, 0);
        if (data_.outplace_) return data_.u_.str_;
        return data_.u_.str_inplace_;
    }

    inline int strlen() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_str, 0);
        if (data_.outplace_) return (data_.padding_ << 16) | data_.len_or_port_;
        return data_.padding_;
    }

    inline bool is_dict() const noexcept { return data_.type_ == any_dict; }

    inline std::map<zce_any, zce_any>& dict() {
        static std::map<zce_any, zce_any> empty;
        ZCE_ASSERT_RETURN(data_.type_ == any_dict, empty);
        return *data_.u_.dict_;
    }

    inline const std::map<zce_any, zce_any>& dict() const noexcept {
        static std::map<zce_any, zce_any> empty;
        ZCE_ASSERT_RETURN(data_.type_ == any_dict, empty);
        return *data_.u_.dict_;
    }

    inline bool is_boolvec() const noexcept { return data_.type_ == any_boolarray; }

    inline const std::vector<bool>& boolvec() const noexcept {
        static std::vector<bool> empty;
        ZCE_ASSERT_RETURN(data_.type_ == any_boolarray, empty);
        return *data_.u_.boolvec_;
    }

    inline bool is_vector() const noexcept { return data_.type_ == any_vector; }

    inline std::vector<zce_any>& vector() {
        static std::vector<zce_any> empty;
        ZCE_ASSERT_RETURN(data_.type_ == any_vector, empty);
        return *data_.u_.vec_;
    }

    inline const std::vector<zce_any>& vector() const noexcept {
        static std::vector<zce_any> empty;
        ZCE_ASSERT_RETURN(data_.type_ == any_vector, empty);
        return *data_.u_.vec_;
    }

    template <typename T>
    T* get() noexcept {
        if constexpr (std::is_same<T, zce_float>::value) {
            return (T*)array<zce_float>();
        } else if constexpr (std::is_same<T, zce_double>::value) {
            return (T*)array<zce_double>();
        } else if constexpr (std::is_same<T, char>::value) {
            return (T*)str();
        } else if constexpr (std::is_integral<T>::value) {
            return (T*)array<zce_byte>();
        } else if constexpr (std::is_same<T, zce_byte>::value) {
            return (T*)array<zce_byte>();
        } else {
            if (is_object()) {
                return dynamic_cast<T*>(object());
            }
            return nullptr;
        }
    }

    inline bool is_addr() const noexcept {
        return data_.type_ == any_ipv6 || data_.type_ == any_int64;
    }

    inline bool is_ipv4() const noexcept { return data_.type_ == any_ipv4; }

    inline bool is_ipv6() const noexcept { return data_.type_ == any_ipv6; }

    inline const struct in_addr& ipv4() const noexcept {
        static const struct in_addr _empty {};
        ZCE_ASSERT_RETURN(data_.type_ == any_ipv4, _empty);
        return data_.u_.ipv4_;
    }

    inline const struct in6_addr& ipv6() const noexcept {
        static const struct in6_addr _empty {};
        ZCE_ASSERT_RETURN(data_.type_ == any_ipv6, _empty);
        return data_.u_.ipv6_;
    }

    inline unsigned short port() const noexcept { return data_.len_or_port_; }

    inline bool is_datetime() const noexcept { return data_.type_ == any_datetime; }

    inline zce_timestamp datetime() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_datetime, data_.u_.i64_[0]);
        return data_.u_.i64_[0];
    }

    inline unsigned short quality() const noexcept { return data_.quality_; }

    inline void quality(unsigned short q) noexcept { data_.quality_ = q; }

    std::string to_string() const noexcept;
};