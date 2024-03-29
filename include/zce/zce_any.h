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
#include <zce/zce_types.h>
#include <zce/zce_object.h>
#include <map>
#include <vector>

class zce_object;
class zce_dblock;

class ZCE_API zce_any
{
    enum _any_types {
        any_int64 = 0,
        any_double,
        any_rawptr,
        any_ipv4,
        any_ipv6,

        any_object,
        any_dblock,
        any_intarray,
        any_fltarray,
        any_dblarray,
        any_str,        //string always as a 0 in tail, but not calculated in length
        any_boolarray,  //not fully supported
        any_dict,
        any_vector,
    };
    struct {
        union {
            zce_int64 i64_;
            zce_double dbl_;
            void* rawptr_;
            zce_byte bytearray_inplace_[sizeof(struct in6_addr)];
            char str_inplace_[sizeof(struct in6_addr)];
            struct in_addr ipv4_;
            struct in6_addr ipv6_;

            zce_object* obj_;
            zce_dblock* dblock_;
            zce_byte* bytearray_;
            char* str_;
            std::vector<bool>* boolvec_;
            std::map<zce_any, zce_any>* dict_;
            std::vector<zce_any>* vec_;
        } u_;

        zce_uint16 len_or_port_ = 0;
        zce_uint16 padding_ : 6;
        zce_uint16 type_ : 5;
        zce_uint16 shiftbits_ : 3;
        zce_uint16 signed_ : 1;
        zce_uint16 outplace_ : 1;
    } data_;

    template<typename T>
    constexpr _any_types _to_type() const {
        if constexpr (std::is_same<T, zce_float>::value) {
            return any_fltarray;
        }
        else if constexpr (std::is_same<T, zce_double>::value) {
            return any_dblarray;
        }
        else if constexpr (std::is_same<T, char>::value) {
            return any_str;
        }
        else if constexpr (std::is_integral<T>::value) {
            return any_intarray;
        }
        else {
            static_assert (std::is_same<T, zce_byte>::value, "must be integral type");
            return any_intarray;
        }
    }

    //called by template constructor
    zce_any(const zce_byte* buf, size_t len, _any_types any_types, bool issigned, int shiftbits) noexcept;

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

    bool operator!=(const zce_any& rhs) const noexcept {
        return !(operator==(rhs));
    }

    bool operator<(const zce_any& rhs) const noexcept;

    zce_any(int i32 = 0) noexcept 
        : data_{} {
        data_.u_.i64_ = i32;
    }

    zce_any(zce_int64 i64) noexcept 
        : data_{} {
        data_.u_.i64_ = i64;
    }

    zce_any(zce_double dbl) noexcept 
        : data_{} {
        data_.type_ = any_double;
        data_.u_.dbl_ = dbl;
    }

    zce_any(const void* raw) noexcept
        : data_{} {
        data_.type_ = any_rawptr;
        data_.u_.rawptr_ = (void*)raw;
    }

    zce_any(zce_object* obj) noexcept 
        : data_{} {
        data_.type_ = any_object;
        data_.u_.obj_ = obj;
        if (data_.u_.obj_)
            data_.u_.obj_->__addref();
    }

    template<typename T>
    zce_any(const T* barray, size_t len) noexcept {
        new (this) zce_any((const zce_byte*)barray,
            (size_t)len * sizeof(T),
            _to_type<T>(),
            std::is_signed<T>::value,
            zce_bits_msb_index(sizeof(T)));
    }

    zce_any(const std::vector<bool>& vec) noexcept;

    zce_any(const char* str, size_t len) noexcept;

    zce_any(const zce_byte* buf, size_t len, zce_byte fixarr_u) noexcept;

    zce_any(const struct in_addr& ipv4, unsigned short port) noexcept 
        : data_{} {
        data_.type_ = any_ipv4;
        data_.u_.ipv4_ = ipv4;
        data_.len_or_port_ = port;
    }

    zce_any(const struct in6_addr& ipv6, unsigned short port) noexcept 
        : data_{} {
        data_.type_ = any_ipv6;
        data_.u_.ipv6_ = ipv6;
        data_.len_or_port_ = port;
    }

    static zce_any create_dict();

    static zce_any create_vector();

    static zce_any create_dblock();

    inline bool is_i64() const noexcept {
        return data_.type_ == any_int64;
    }

    inline zce_int64 i64() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_int64, 0);
        return data_.u_.i64_;
    }

    inline bool is_double() const noexcept {
        return data_.type_ == any_double;
    }

    inline zce_double dbl() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_double || data_.type_ == any_int64, 0);
        if (data_.type_ == any_int64)
            return (zce_double)data_.u_.i64_;
        return data_.u_.dbl_;
    }

    inline bool is_object() const noexcept {
        return data_.type_ == any_object;
    }

    inline zce_object* object() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_object, 0);
        return data_.u_.obj_;
    }

    inline bool is_dblock() const noexcept {
        return data_.type_ == any_dblock;
    }

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
        return data_.type_ == any_intarray 
            || data_.type_ == any_fltarray 
            || data_.type_ == any_dblarray;
    }

    template <typename T>
    inline const T* array() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == _to_type<T>() && sizeof(T) == 1 << data_.shiftbits_, 0);
        if (data_.outplace_)
            return (T*)data_.u_.bytearray_;
        return (T*)data_.u_.bytearray_inplace_;
    }

    inline int arraylen() const noexcept {
        ZCE_ASSERT_RETURN(is_array(), 0);

        if (data_.outplace_)
            return (data_.padding_ << 16) | data_.len_or_port_;
        return data_.padding_;
    }

    inline bool is_str() const noexcept {
        return data_.type_ == any_str;
    }

    inline const char* str() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_str, 0);
        if (data_.outplace_)
            return data_.u_.str_;
        return data_.u_.str_inplace_;
    }

    inline int strlen() const noexcept {
        ZCE_ASSERT_RETURN(data_.type_ == any_str, 0);
        if (data_.outplace_)
            return (data_.padding_ << 16) | data_.len_or_port_;
        return data_.padding_;
    }

    inline bool is_dict() const noexcept {
        return data_.type_ == any_dict;
    }

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

    inline bool is_boolvec() const noexcept {
        return data_.type_ == any_boolarray;
    }

    inline const std::vector<bool>& boolvec() const noexcept {
        static std::vector<bool> empty;
        ZCE_ASSERT_RETURN(data_.type_ == any_boolarray, empty);
        return *data_.u_.boolvec_;
    }

    inline bool is_vector() const noexcept {
        return data_.type_ == any_vector;
    }

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

    template<typename T> 
    T* get() noexcept {
        if constexpr (std::is_same<T, zce_float>::value) {
            return (T*)array<zce_float>();
        }
        else if constexpr (std::is_same<T, zce_double>::value) {
            return (T*)array<zce_double>();
        }
        else if constexpr (std::is_same<T, char>::value) {
            return (T*)str();
        }
        else if constexpr (std::is_integral<T>::value) {
            return (T*)array<zce_byte>();
        }
        else if constexpr (std::is_same<T, zce_byte>::value) {
            return (T*)array<zce_byte>();
        }
        else {
            if (is_object()) {
                return dynamic_cast<T*>(object());
            }
            return nullptr;
        }
    }
};