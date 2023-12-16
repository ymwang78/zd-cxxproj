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

    int rpc_call_dblock(const zce_smartptr<zce_object>& vmptr,
        const std::string& method,
        zce_dblock& dblock,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response);

    //int lpc_call_dblock(const std::string& svrname,
    //    const std::string& method,
    //    zce_dblock& dblock,
    //    const std::function<void(int error_code, const zce_dblock& retdata)>& response);

    template<typename T>
    int rpc_call_builtin(const zce_smartptr<zce_object>& vmptr,
        const std::string& method,
        const T& t,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response) {
        zce_dblock dblock;
        int ret = zdp::zds_pack_builtin(0, 0, t, 0);
        if (ret < 0)
            return ret;
        ZCE_MBACQUIRE(dblock, ret);
        if (dblock.capacity() < ret)
            return ZCE_ERROR_MALLOC;
        ret = zdp::zds_pack_builtin(dblock.rd_ptr(), (int)dblock.capacity(), t, 0);
        if (ret < 0)
            return ret;
        dblock.wr_ptr(ret);
        return rpc_call_dblock(vmptr, method, dblock, response);
    }

    template<typename T>
    int rpc_call(const zce_smartptr<zce_object>& vmptr,
        const std::string& method,
        const T& t,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response) {
        zce_dblock dblock;
        int ret = zdp::zds_pack(0, 0, t, 0);
        if (ret < 0)
            return ret;
        ZCE_MBACQUIRE(dblock, ret);
        if (dblock.capacity() < ret)
            return ZCE_ERROR_MALLOC;
        ret = zdp::zds_pack(dblock.rd_ptr(), dblock.capacity(), t, 0);
        if (ret < 0)
            return ret;
        dblock.wr_ptr(ret);
        return rpc_call_dblock(vmptr, method, dblock, response);
    }
};