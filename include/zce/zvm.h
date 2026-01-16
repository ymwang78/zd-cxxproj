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
#include <zce/zdp_base_proto.h>
#include <zce/zce_dblock.h>
#include <zce/zce_mbpool.h>
#include <zce/zce_singleton.h>
#include <zce/zds_schema.h>
#include <zce/zce_task_queue.h>

extern "C" {
typedef struct lua_State lua_State;
typedef void (*lpfn_lual_openlibs)(lua_State* L);
}
class zdl_module;
class zdl_struct;
class zvm_channel;

namespace zce {

class Reactor;
class Scheduler;

namespace zdp {
struct zds_context_t;
template <typename T>
int zds_pack(zce_byte* buffer, int buffer_size, const T& t, zds_context_t* ctx, bool some_flag);
template <typename T>
int zds_pack_builtin(zce_byte* buffer, int buffer_size, const T& t, zds_context_t* ctx,
                     bool some_flag);
template <typename T>
constexpr bool is_builtin_type();
}  // namespace zdp

namespace zvm {

class VirtualMachineStubPimpl;

class VirtualMachineStub : public zce::Object {
    zce::SmartPtr<VirtualMachineStubPimpl> pimpl_ptr_;

  public:
    typedef std::function<void(int error_code, zce::RefBlock&& retdata)> response_cb;

    VirtualMachineStub();

    ~VirtualMachineStub();

    const std::string& hostDir() const noexcept;

    std::string vmHomeDir(const zdp_base::zvm_t& vm) const noexcept;

    const zce::SmartPtr<VirtualMachineStubPimpl>& pimpl_ptr() const noexcept { return pimpl_ptr_; }

    int initStub(const zce::SmartPtr<zce::Scheduler>&, const zce::SmartPtr<zce::Reactor>&,
                 const std::string& u8_host_dir);

    int listen(const char* host, unsigned short port);

    zce::SmartPtr<zce::Object> boot(const std::string& svc_name, const zdp_base::zvm_t& vm,
                                    zce::RefBlock args);

    zce::SmartPtr<zce::Object> boot(const std::string& svc_name, const std::string& host,
                                    unsigned short port, bool ssl, int default_timeout,
                                    std::function<void()> open_cb, std::function<void()> close_cb);

    zce::SmartPtr<zce::Object> get_vm(const std::string& svc_name) const;

    void destroy(const zce::SmartPtr<zce::Object>& vm);

    int rpc_call_dblock(const zce::SmartPtr<zce::Object>& vmptr, zce_int64 objectid,
                        const std::string& method, zce::RefBlock&& dblock, int mstimeout,
                        const response_cb& response);

    template <typename T>
    int rpc_call_builtin(const zce::SmartPtr<zce::Object>& vmptr, zce_int64 objectid,
                         const std::string& method, const T& t, int mstimeout,
                         const response_cb& response) {
        zce::RefBlock dblock;
        int ret = zce::zdp::zds_pack_builtin(0, 0, t, 0, true);
        if (ret < 0) return ret;
        ZCE_MBACQUIRE(dblock, ret);
        if ((int)dblock.space() < ret) return ZCE_ERROR_MALLOC;
        ret = zce::zdp::zds_pack_builtin(dblock.rd_ptr_cow(), (int)dblock.space(), t, 0, true);
        if (ret < 0) return ret;
        dblock.wr_ptr(ret);
        return rpc_call_dblock(vmptr, objectid, method, std::move(dblock), mstimeout, response);
    }

    template <typename T>
    int rpc_call_msg(
        const zce::SmartPtr<zce::Object>& vmptr, zce_int64 objectid, const std::string& method,
        const T& t, int mstimeout,
        const std::function<void(int error_code, const zce::RefBlock& retdata)>& response) {
        zce::RefBlock dblock;
        int ret = zce::zdp::zds_pack(0, 0, t, 0, true);
        if (ret < 0) return ret;
        ZCE_MBACQUIRE(dblock, ret);
        if ((int)dblock.space() < ret) return ZCE_ERROR_MALLOC;
        ret = zce::zdp::zds_pack(dblock.rd_ptr_cow(), (int)dblock.space(), t, 0, true);
        if (ret < 0) return ret;
        dblock.wr_ptr(ret);
        return rpc_call_dblock(vmptr, objectid, method, std::move(dblock), mstimeout, response);
    }

    template <typename T>
    int rpc_call(
        const zce::SmartPtr<zce::Object>& vmptr, zce_int64 objectid, const std::string& method, T t,
        int mstimeout,
        const std::function<void(int error_code, const zce::RefBlock& retdata)>& response) {
        typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type TT;
        if constexpr (std::is_same<TT, zce::RefBlock>::value) {
            return rpc_call_dblock(vmptr, objectid, method, std::move(t), mstimeout, response);
        } else if constexpr (zce::zdp::is_builtin_type<TT>()) {
            return rpc_call_builtin(vmptr, objectid, method, t, mstimeout, response);
        } else {
            return rpc_call_msg(vmptr, objectid, method, t, mstimeout, response);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

typedef zce::Singleton<VirtualMachineStub> VirtualMachineStubSigt;

class RpcServant;
class RpcStream;
class VirtualMachineStubPimpl;

class Machine : public zce::TaskQueue {
  protected:
    std::string vm_name_;
    zce::SmartPtr<VirtualMachineStub> stub_ptr_;
    std::map<std::string, zce::SmartPtr<zdl_module>> modules_;

  public:
    Machine(const std::string& vm_name, const zce::SmartPtr<VirtualMachineStub>& stub_ptr);

    virtual ~Machine();

    const std::string& vm_name() const noexcept { return vm_name_; }

    const zce::SmartPtr<VirtualMachineStub>& stub_ptr() const noexcept { return stub_ptr_; }

    zce::SmartPtr<Machine> get_vm(const std::string& sname) const;

    const zce::SmartPtr<zce::Reactor> reactor_ptr() const noexcept;

    zce::SmartPtr<::zce::zvm::RpcServant> rpc_serve(const char* host, unsigned short port, bool ssl,
                                                    const char* cert, const char* key,
                                                    const char* method_prefix = "");

    virtual int start() = 0;

    virtual void stop() = 0;

    virtual int call_dblock(zce_int64 objid, const std::string& method, ::zce::RefBlock& dblock,
                            int mstimeout, const VirtualMachineStub::response_cb& response) = 0;

    virtual int call_dblock_from_remote(zce_int64 objid, const std::string& method,
                                        ::zce::RefBlock& dblock,
                                        const ::zce::SmartPtr<RpcStream>& stream_ptr,
                                        const VirtualMachineStub::response_cb& response);

    int registe_meta(const char* filepath);

    zce::SmartPtr<zdl_module> get_module_by_name(const std::string& name) const;

    zce::SmartPtr<zdl_struct> get_struct(const std::string& module, const std::string& name) const;
};

namespace detail {

template <typename... T>
auto unpack_recursive_impl(const zce::RefBlock&, size_t, const char*)
    -> std::enable_if_t<sizeof...(T) == 0, std::pair<int, std::tuple<>>> {
    return {0, {}};
}

template <typename First, typename... Rest>
inline std::pair<int, std::tuple<First, Rest...>> unpack_recursive_impl(const zce::RefBlock& data,
                                                                        size_t offset,
                                                                        const char* func) {
    int bytes_read = 0;
    First current_value{};
    if constexpr (zce::zdp::is_builtin_type<First>()) {
        bytes_read = zce::zdp::zds_unpack_builtin(current_value, data.rd_ptr() + offset,
                                                  data.length() - offset, nullptr);
    } else if constexpr (zce::zdp::is_vector<First>()) {
        bytes_read = zce::zdp::zds_unpack_array(current_value, data.rd_ptr() + offset,
                                                data.length() - offset, nullptr);
    } else {
        bytes_read = zce::zdp::zds_unpack(current_value, data.rd_ptr() + offset,
                                          data.length() - offset, nullptr, true);
    }

    if (bytes_read < 0) {
        ZCE_ERROR((ZLOG_ERROR, "unpack %s failed for one of the types: 0x%x", func, bytes_read));
        return {bytes_read, {}};
    }

    auto rest_pair = unpack_recursive_impl<Rest...>(data, offset + bytes_read, func);

    if (rest_pair.first < 0) {
        return {rest_pair.first, {}};
    }

    std::tuple<First, Rest...> result_tuple =
        std::tuple_cat(std::make_tuple(std::move(current_value)), std::move(rest_pair.second));

    return {bytes_read + rest_pair.first, std::move(result_tuple)};
}

template <typename... Results>
inline std::pair<int, std::tuple<Results...>> unpack_to_tuple(const zce::RefBlock& data,
                                                              const char* func) {
    // 从偏移量 0 开始调用递归实现
    return unpack_recursive_impl<Results...>(data, 0, func);
}

}  // namespace detail

class VirtualMachineProxy : public zce::Object {
  protected:
    virtual int doCallTwoWay(const char* func, const zce::RefBlock& input, int mstimeout,
                             std::function<void(int errcode, const zce::RefBlock& output)> cb) = 0;

  public:
    template <typename... RESULTs>
    struct RpcResult {
        int errcode;
        std::string errdesc;
        std::tuple<RESULTs...> data;
        bool isSuccess() const { return errcode >= 0; }

        const std::string& errorMessage() const { return errdesc; }

        template <std::size_t I>
        auto& get() & {
            return std::get<I>(data);
        }

        template <std::size_t I>
        const auto& get() const& {
            return std::get<I>(data);
        }

        template <std::size_t I>
        auto&& get() && {
            return std::get<I>(std::move(data));
        }
    };

    template <typename ARG>
    int pack(zce::RefBlock& dblock, const ARG& input) {
        int ret = zce::zdp::zds_pack(0, 0, input, 0, true);
        if (ret < 0) return ret;
        ZCE_MBACQUIRE(dblock, ret);
        if ((int)dblock.space() < ret) return ZCE_ERROR_MALLOC;
        ret = zce::zdp::zds_pack(dblock.rd_ptr_cow(), (int)dblock.space(), input, 0, true);
        if (ret < 0) return ret;
        dblock.wr_ptr(ret);
        return ret;
    }

    template <typename... Results, typename... Args, typename F>
    int callTwoWayAsync(const char* func, int mstimeout, F&& async_cb, Args&&... args) {
        int ret = 0;
        ZTRACE(func);

        zce::RefBlock dblock;
        if constexpr (sizeof...(Args) > 0) {
            ret = zce::zdp::zds_pack_multi(0, 0, nullptr, true, std::forward<Args>(args)...);
            if (ret < 0) return ret;
            if (ret > 0) {
                ZCE_MBACQUIRE(dblock, ret);
                if ((int)dblock.space() < ret) return ZCE_ERROR_MALLOC;
                ret = zce::zdp::zds_pack_multi(dblock.wr_ptr_cow(), (int)dblock.space(), nullptr,
                                               true, std::forward<Args>(args)...);
                if (ret < 0) return ret;
                dblock.wr_ptr(ret);
            }
        }

        int tick_begin_call = zce_tick();
        ret = doCallTwoWay(func, dblock, mstimeout, [=](int errcode, const zce::RefBlock& retdata) {
            int tick_end_call = zce_tick();
            if (tick_end_call - tick_begin_call > 1000) {
                ZTRACE("Slow RPC", func, tick_end_call - tick_begin_call, "size", retdata.length());
            }
            RpcResult<Results...> result;
            result.errcode = errcode;
            if (errcode < 0) {
                result.errdesc = "callTwoWay failed";
                ZCE_ERROR((ZLOG_ERROR, "call %s ret: 0x%x, desc: %s", func, errcode,
                           result.errdesc.c_str()));
                async_cb(std::move(result));
                return;
            }
            auto [unpack_code, data_tuple] = detail::unpack_to_tuple<Results...>(retdata, func);
            int tick_parsed = zce_tick();
            if (tick_parsed - tick_end_call > 300) {
                ZTRACE("Slow Unpack", func, tick_parsed - tick_begin_call);
            }
            result.errcode = unpack_code;
            if (unpack_code < 0) {
                result.errdesc = "unpack result failed";
            } else {
                result.errdesc = "success";
                result.data = std::move(data_tuple);  // ✨ 移动解包后的元组
            }
            async_cb(std::move(result));
        });

        if (ret < 0) {
            ZCE_ERROR((ZLOG_ERROR, "rpc call %s failed: 0x%x", func, ret));
            return ret;
        }
        return ret;
    }
};

}  // namespace zvm

}  // namespace zce
