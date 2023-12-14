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

    int boot(const std::string& name,
        const std::string& path);

    int rpc_call_dblock(const std::string& host, 
        unsigned short port,
        const std::string& svrname,
        const std::string& method,
        zce_dblock& dblock,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response);

    int lpc_call_dblock(const std::string& svrname,
        const std::string& method,
        zce_dblock& dblock,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response);

    template<typename... Args>
    int lpc_call(const std::string& svrname,
        const std::string& method,
        const std::function<void(int error_code, const zce_dblock& retdata)>& response,
        Args... args) {
        zce_dblock dblock;
        zds_push(dblock, args...);
        return lpc_call_dblock(svrname, method, dblock, response);
    }
};