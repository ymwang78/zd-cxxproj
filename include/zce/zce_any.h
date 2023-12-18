#pragma once

#include <zce/zce_types.h>
#include <zce/zce_object.h>

class ZCE_API zce_any
{
    enum _any_types {
        any_int64 = 0,
        any_rawptr,
        any_object,
        any_ipv4,
        any_ipv6,
    };

    union {
        zce_int64 i64_;
        void* rawptr_;
        zce_object* obj_;
        struct in_addr ipv4_;
        struct in6_addr ipv6_;
    } u_;
    
    zce_uint16 len_or_port_ = 0;
    zce_byte padding_ = 0;
    zce_byte type_ = 0;

public:

    zce_any() noexcept : zce_any((zce_int64)0) {
    }

    zce_any(const zce_any& rhs) {
        len_or_port_ = rhs.len_or_port_;
        padding_ = rhs.padding_;
        type_ = rhs.type_;
        switch (type_) {
        case any_int64:
            u_.i64_ = rhs.u_.i64_;
            break;
        case any_rawptr:
            u_.rawptr_ = rhs.u_.rawptr_;
            break;
        case any_object:
            u_.obj_ = rhs.u_.obj_;
            if (u_.obj_)
                u_.obj_->__addref();
            break;
        case any_ipv4:
            u_.ipv4_ = rhs.u_.ipv4_;
            break;
        case any_ipv6:
            u_.ipv6_ = rhs.u_.ipv6_;
            break;
        }
    }

    zce_any(zce_any&& rhs) noexcept {
        len_or_port_ = rhs.len_or_port_;
        padding_ = rhs.padding_;
        type_ = rhs.type_;
        switch (type_) {
        case any_int64:
            u_.i64_ = rhs.u_.i64_;
            break;
        case any_rawptr:
            u_.rawptr_ = rhs.u_.rawptr_;
            break;
        case any_object:
            u_.obj_ = rhs.u_.obj_;
            break;
        case any_ipv4:
            u_.ipv4_ = rhs.u_.ipv4_;
            break;
        case any_ipv6:
            u_.ipv6_ = rhs.u_.ipv6_;
            break;
        }
        rhs.type_ = any_int64;
        rhs.u_.i64_ = 0;
    }

    zce_any& operator=(const zce_any& rhs) {
        if (this != &rhs) {
            this->~zce_any();
            new (this) zce_any(rhs);
        }
        return *this;
    }

    ~zce_any() {
        switch (type_) {
        case any_object:
            if (u_.obj_)
                u_.obj_->__decref();
            break;
        }
    }

    zce_any(zce_int64 i64) noexcept : type_(any_int64) {
        u_.i64_ = i64;
    }

    zce_any(zce_object* obj) noexcept : type_(any_object) {
        u_.obj_ = obj;
        if (u_.obj_)
            u_.obj_->__addref();
    }

    zce_any(const struct in_addr& ipv4, unsigned short port) noexcept : type_(any_ipv4) {
        u_.ipv4_ = ipv4;
        len_or_port_ = port;
    }

    zce_any(const struct in6_addr& ipv6, unsigned short port) noexcept : type_(any_ipv6) {
        u_.ipv6_ = ipv6;
        len_or_port_ = port;
    }

    inline bool is_object() const noexcept {
        return type_ == any_object;
    }

    inline zce_object* object() const noexcept {
        ZCE_ASSERT_RETURN(type_ == any_object, 0);
        return u_.obj_;
    }
};