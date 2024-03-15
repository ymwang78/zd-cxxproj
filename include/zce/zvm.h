// ***************************************************************
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
class zvm_pimpl;

class zvm : public zce_object
{
    zce_smartptr<zvm_pimpl> pimpl_ptr_;

public:

    zvm(const zce_smartptr<zce_schedule>&,
        const zce_smartptr<zce_reactor>&);

    ~zvm();

    zce_smartptr<zce_object> boot(const std::string& svc_name,
        const std::string& path);

    zce_smartptr<zce_object> boot(const std::string& svc_name,
        const std::string& host, 
        unsigned short port);

    zce_smartptr<zce_object> get_vm(const std::string& svc_name) const;

    void destroy(const zce_smartptr<zce_object>& vm);

    int rpc_call_dblock(const zce_smartptr<zce_object>& vmptr,
        zce_int64 objectid,
        const std::string& method,
        const zce_dblock& dblock,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response);

    template<typename T>
    int rpc_call_builtin(const zce_smartptr<zce_object>& vmptr,
        zce_int64 objectid,
        const std::string& method,
        const T& t,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response) {
        zce_dblock dblock;
        int ret = zdp::zds_pack_builtin(0, 0, t, 0);
        if (ret < 0)
            return ret;
        ZCE_MBACQUIRE(dblock, ret);
        if ((int)dblock.capacity() < ret)
            return ZCE_ERROR_MALLOC;
        ret = zdp::zds_pack_builtin(dblock.rd_ptr(), (int)dblock.capacity(), t, 0);
        if (ret < 0)
            return ret;
        dblock.wr_ptr(ret);
        return rpc_call_dblock(vmptr, objectid, method, dblock, response);
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
        if ((int)dblock.capacity() < ret)
            return ZCE_ERROR_MALLOC;
        ret = zdp::zds_pack(dblock.rd_ptr(), (int)dblock.capacity(), t, 0, true);
        if (ret < 0)
            return ret;
        dblock.wr_ptr(ret);
        return rpc_call_dblock(vmptr, objectid, method, dblock, response);
    }

    template<typename T>
    int rpc_call(const zce_smartptr<zce_object>& vmptr,
        zce_int64 objectid,
        const std::string& method,
        const T& t,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response) {

        if constexpr (std::is_same<T, zce_dblock>::value) {
            return rpc_call_dblock(vmptr, objectid, method, t, response);
        }
        else if constexpr (zdp::is_builtin_type<T>()) {
            return rpc_call_builtin(vmptr, objectid, method, t, response);
        }
        else {
            return rpc_call_msg(vmptr, objectid, method, t, response);
        }
    }
};