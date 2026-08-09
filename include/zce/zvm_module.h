#pragma once
// ***************************************************************
//  zvm module   version:  1.0   -  date: 2026/08/09
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2003 - All Rights Reserved
// ***************************************************************
// zvm 模块接入公开接口。
//
// 独立模块（libmpc / libident / libcoin 等）向 zvm 注册自己的 vmtype 和
// Lua 扩展库时只需包含本头文件，不应再包含 libsrc/libzce 下的内部头
// （zvm/zvm_pimpl.h、zvm/zvm_base.h）——那些头暴露的是 zvm 的内部实现，
// 跟着它们走会把模块和 libzce 的内部布局焊死。
// ***************************************************************
#include <zce/zvm.h>
#include <functional>
#include <string>

// Lua 扩展库注册：模块在 zXXX_init() 里构造一个静态对象即可把自己的
// openlibs 回调挂进 zua 虚拟机的初始化链。
class ZCE_API zua_register {
  public:
    zua_register(lpfn_lual_openlibs openlibs);

    ~zua_register();
};

namespace zce {

namespace zvm {

// vmtype 注册：模块在 zXXX_init() 里构造一个静态对象，把 vmtype 字符串
// 和创建 Machine 的工厂函数登记到 zvm。之后 hostvm 侧按 zvm_t::vmtype
// boot 出对应的 Machine。
class ZCE_API VirtualMachineRegister {
  public:
    typedef std::function<zce::SmartPtr<::zce::zvm::Machine>(
        const zdp_base::zvm_t& vm, const zce::SmartPtr<VirtualMachineStub>& this_ptr,
        zce::RefBlock& dblock)>
        lpfn_zvm_creator;

    VirtualMachineRegister(const std::string& suffix, lpfn_zvm_creator creator);

    ~VirtualMachineRegister();
};

// ── 反向调用（服务端顺着来路回调发起方，如进度上报） ─────────────────────
//
// 这两个函数一律以 SmartPtr<Object> 作为不透明句柄进出，目的是让模块不必看见
// RpcStream / Proxy 的定义：两者的 dynamic_cast 都需要完整类型，而完整类型只
// 在 libzce 内部头里有。转换统一在 libzce 内部完成。

// 把 call_dblock_from_remote 收到的来路流收成句柄存下来备用。
zce::SmartPtr<zce::Object> ZCE_API toObjectHandle(const zce::SmartPtr<RpcStream>& stream);

// 顺着来路句柄取一个指向发起方 svc_name 服务的 vm，可直接作为
// VirtualMachineStub::rpc_call_* 的目标。已存在则复用，否则新建；失败返回空。
zce::SmartPtr<zce::Object> ZCE_API getOrBootReuseVM(
    const zce::SmartPtr<zce::Object>& stream_handle, const std::string& svc_name,
    int default_timeout_ms);

}  // namespace zvm

}  // namespace zce
