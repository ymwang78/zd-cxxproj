// *s**************************************************************
//  zvm   version:  1.0   -  date: 2016/3/12
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2003 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once
#include <functional>
#include <zce/zce_dblock.h>
#include <zce/zce_mbpool.h>
#include <zce/zce_singleton.h>

class zvm_pimpl;
class zce_schedule;
class zce_reactor;

extern "C" {
    typedef struct lua_State lua_State;
    typedef void (*lpfn_lual_openlibs)(lua_State* L);
}

class zua_register
{
public:

    zua_register(lpfn_lual_openlibs openlibs);

    ~zua_register();
};

namespace zdp {
    struct zds_context_t;
    template<typename T>
    int zds_pack(zce_byte *buffer, int buffer_size, const T& t, zds_context_t* ctx, bool some_flag);
    template<typename T>
    int zds_pack_builtin(zce_byte *buffer, int buffer_size, const T& t, zds_context_t* ctx, bool some_flag);
    template<typename T>
    constexpr bool is_builtin_type();
}

class zvm : public zce_object
{
    zce_smartptr<zvm_pimpl> pimpl_ptr_;

public:
    typedef std::function<void(int error_code, zce_dblock&& retdata)> response_cb;

    zvm();

    ~zvm();

    int init(const zce_smartptr<zce_schedule>&,
        const zce_smartptr<zce_reactor>&);

    zce_smartptr<zce_object> boot(const std::string& svc_name,
        const std::string& path, zce_dblock& args);

    zce_smartptr<zce_object> boot(const std::string& svc_name,
        const std::string& host, 
        unsigned short port,
        bool ssl,
        int default_timeout);

    zce_smartptr<zce_object> get_vm(const std::string& svc_name) const;

    void destroy(const zce_smartptr<zce_object>& vm);

    int rpc_call_dblock(const zce_smartptr<zce_object>& vmptr,
        zce_int64 objectid,
        const std::string& method,
        zce_dblock&& dblock,
        const response_cb& response);

    template<typename T>
    int rpc_call_builtin(const zce_smartptr<zce_object>& vmptr,
        zce_int64 objectid,
        const std::string& method,
        const T& t,
        const response_cb& response) {
        zce_dblock dblock;
        int ret = zdp::zds_pack_builtin(0, 0, t, 0, true);
        if (ret < 0)
            return ret;
        ZCE_MBACQUIRE(dblock, ret);
        if ((int)dblock.space() < ret)
            return ZCE_ERROR_MALLOC;
        ret = zdp::zds_pack_builtin(dblock.rd_ptr_cow(), (int)dblock.space(), t, 0, true);
        if (ret < 0)
            return ret;
        dblock.wr_ptr(ret);
        return rpc_call_dblock(vmptr, objectid, method, std::move(dblock), response);
    }

    template<typename T>
    int rpc_call_msg(const zce_smartptr<zce_object>& vmptr,
        zce_int64 objectid,
        const std::string& method,
        const T& t,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response) {
        zce_dblock dblock;
        int ret = zdp::zds_pack(0, 0, t, 0, true);
        if (ret < 0)
            return ret;
        ZCE_MBACQUIRE(dblock, ret);
        if ((int)dblock.space() < ret)
            return ZCE_ERROR_MALLOC;
        ret = zdp::zds_pack(dblock.rd_ptr_cow(), (int)dblock.space(), t, 0, true);
        if (ret < 0)
            return ret;
        dblock.wr_ptr(ret);
        return rpc_call_dblock(vmptr, objectid, method, std::move(dblock), response);
    }

    template<typename T>
    int rpc_call(const zce_smartptr<zce_object>& vmptr,
        zce_int64 objectid,
        const std::string& method,
        T t,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response) {
        typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type TT;
        if constexpr (std::is_same<TT, zce_dblock>::value) {
            return rpc_call_dblock(vmptr, objectid, method, std::move(t), response);
        }
        else if constexpr (zdp::is_builtin_type<TT>()) {
            return rpc_call_builtin(vmptr, objectid, method, t, response);
        }
        else {
            return rpc_call_msg(vmptr, objectid, method, t, response);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
    void zds_push(zce_dblock& dblock,
        const char* v) {
        zds_push_string(dblock, v);
    }

    void luas_push(zce_dblock& dblock,
        const std::string& v) {
        luas_push_lstring(dblock, v.c_str(), (zce_int32)v.length());
    }

    void luas_push(zce_dblock& dblock,
        zce_int64 v) {
        luas_push_integer(dblock, v);
    }

    void luas_push(zce_dblock& dblock,
        zce_int32 v) {
        luas_push_integer(dblock, v);
    }

    void luas_push(zce_dblock& dblock,
        bool v) {
        luas_push_bool(dblock, v);
    }

    void luas_push(zce_dblock& dblock,
        void* ctx) {
        luas_push_lightuserdata(dblock, ctx);
    }

    template <typename T, typename... Args>
    void luas_push(zce_dblock& dblock,
        const T& v,
        Args... args) {
        luas_push(dblock, v);
        luas_push(dblock, args...);
    }

    template<typename... Args>
    int rpc_call(const zce_smartptr<zce_object>& vmptr,
        zce_int64 objectid,
        const std::string& method,
        const response_cb& response,
        Args... args) {
        zce_dblock dblock;
        zds_pack()
        luas_push(dblock, args...);
        return lpc_call_dblock(svrname, method, dblock);
    }
#endif
};

typedef zce_singleton<zvm> zvm_sigt;