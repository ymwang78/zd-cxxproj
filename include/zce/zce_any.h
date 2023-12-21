#pragma once

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
        any_bytearray,
        any_intarray,
        any_fltarray,
        any_dblarray,
        any_str,        //string always as a 0 in tail, but not calculated in length
        any_dict,
        any_vector,
    };

    union {
        zce_int64 i64_;
        zce_double dbl_;
        void* rawptr_;
        zce_byte  bytearray_inplace_[sizeof(struct in6_addr)];
        char  str_inplace_[sizeof(struct in6_addr)];
        struct in_addr ipv4_;
        struct in6_addr ipv6_;

        zce_object* obj_;
        zce_dblock* dblock_;
        zce_byte* bytearray_;
        char* str_;
        std::map<zce_any, zce_any>* dict_;
        std::vector<zce_any>* vec_;
    } u_;
    
    zce_uint16 len_or_port_ = 0;
    zce_byte padding_ = 0;
    zce_byte type_ : 5;
    zce_byte intbits_ : 2;
    zce_byte outplace_ : 1;

    template<typename T>
    zce_byte _to_type() {
        if constexpr (std::is_same<T, zce_byte > ::value) {
            return any_bytearray;
        }
        else if constexpr (std::is_same<T, zce_float>::value) {
            return any_fltarray;
        }
        else if constexpr (std::is_same<T, zce_double>::value) {
            return any_dblarray;
        }
        else if constexpr (std::is_same<T, char>::value) {
            return any_str;
        }
        else {
            return any_intarray;
        }
    }
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
        : type_(any_int64), intbits_(0), outplace_(0) {
        u_.i64_ = i32;
    }

    zce_any(zce_int64 i64) noexcept 
        : type_(any_int64), intbits_(0), outplace_(0) {
        u_.i64_ = i64;
    }

    zce_any(zce_double dbl) noexcept 
        : type_(any_double), intbits_(0), outplace_(0) {
        u_.dbl_ = dbl;
    }

    zce_any(zce_object* obj) noexcept 
        : type_(any_object), intbits_(0), outplace_(0) {
        u_.obj_ = obj;
        if (u_.obj_)
            u_.obj_->__addref();
    }

    template<typename T>
    zce_any(const T* brray, size_t len) noexcept
        : type_(_to_type<T>()), intbits_(0), outplace_(0) {
        zce_byte zds_payload = ZDS_PAYLOAD_INTEXT1;
        if constexpr (std::is_same<T, zce_byte > ::value) {
            zds_payload = ZDS_PAYLOAD_BYTE;
        }
        else if constexpr (std::is_same<T, zce_float>::value) {
            zds_payload = ZDS_PAYLOAD_FLOAT;
        }
        else if constexpr (std::is_same<T, zce_double>::value) {
            zds_payload = ZDS_PAYLOAD_DOUBLE;
        }
        else {
            static_assert(false, "not supported type");
        }
        new (this) zce_any((const zce_byte*)buf, len * sizeof(T), zds_payload,
            constexpr (std::is_signed<T>::value),
            sizeof(T));
    }

    zce_any(const char* str, size_t len) noexcept;

    zce_any(const zce_byte* buf, size_t len, zce_byte zds_payload, bool issigned, int intbits) noexcept;

    zce_any(const struct in_addr& ipv4, unsigned short port) noexcept 
        : type_(any_ipv4), intbits_(0), outplace_(0) {
        u_.ipv4_ = ipv4;
        len_or_port_ = port;
    }

    zce_any(const struct in6_addr& ipv6, unsigned short port) noexcept 
        : type_(any_ipv6), intbits_(0), outplace_(0) {
        u_.ipv6_ = ipv6;
        len_or_port_ = port;
    }

    static zce_any create_dict();

    static zce_any create_vector();

    static zce_any create_dblock();

    inline bool is_i64() const noexcept {
        return type_ == any_int64;
    }

    inline zce_int64 i64() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_int64, 0);
        return u_.i64_;
    }

    inline bool is_double() const noexcept {
        return type_ == any_double;
    }

    inline zce_double dbl() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_double, 0);
        return u_.dbl_;
    }

    inline bool is_object() const noexcept {
        return type_ == any_object;
    }

    inline zce_object* object() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_object, 0);
        return u_.obj_;
    }

    inline bool is_dblock() const noexcept {
        return type_ == any_dblock;
    }

    inline zce_dblock* dblock() noexcept {
        ZCE_ASSERT_RETURN(type_ == any_dblock, 0);
        return u_.dblock_;
    }

    inline const zce_dblock* dblock() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_dblock, 0);
        return u_.dblock_;
    }

    inline bool is_array() const noexcept {
        return type_ == any_intarray || type_ == any_bytearray || type_ == any_fltarray || type_ == any_dblarray;
    }

    template <typename T>
    inline const T* array() const noexcept {
        ZCE_ASSERT_RETURN(type_ == _to_type<T>(), 0);
        if (outplace_)
            return (T*)u_.bytearray_;
        return (T*)u_.bytearray_inplace_;
    }

    inline int arraylen() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_intarray || type_ == any_bytearray || type_ == any_fltarray || type_ == any_dblarray, 0);
        if (outplace_)
            return (padding_ << 16) | len_or_port_;
        return padding_;
    }

    inline bool is_str() const noexcept {
        return type_ == any_str;
    }

    inline const char* str() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_str, 0);
        if (outplace_)
            return u_.str_;
        return u_.str_inplace_;
    }

    inline int strlen() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_str, 0);
        if (outplace_)
            return (padding_ << 16) | len_or_port_;
        return padding_;
    }

    inline bool is_dict() const noexcept {
        return type_ == any_dict;
    }

    inline std::map<zce_any, zce_any>& dict() {
        ZCE_ASSERT_RETURN(type_ == any_dict, *(std::map<zce_any, zce_any>*)0);
        return *u_.dict_;
    }

    inline const std::map<zce_any, zce_any>& dict() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_dict, *(std::map<zce_any, zce_any>*)0);
        return *u_.dict_;
    }

    inline bool is_vector() const noexcept {
        return type_ == any_vector;
    }

    inline std::vector<zce_any>& vector() {
        ZCE_ASSERT_RETURN(type_ == any_vector, *(std::vector<zce_any>*)0);
        return *u_.vec_;
    }

    inline const std::vector<zce_any>& vector() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_vector, *(std::vector<zce_any>*)0);
        return *u_.vec_;
    }
};