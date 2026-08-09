#pragma once
// ***************************************************************
//  zvm module   version:  1.0   -  date: 2026/08/09
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2003 - All Rights Reserved
// ***************************************************************

/**
 * @file zvm_module.h
 * @brief Public integration surface for zvm modules.
 *
 * Standalone modules (libmpc / libident / libcoin ...) only need this header to
 * register their own vmtype and Lua extension libraries. They must not include
 * the internal headers under libsrc/libzce (zvm/zvm_pimpl.h, zvm/zvm_base.h):
 * those expose zvm implementation details and welding a module to them ties it
 * to libzce's internal layout.
 */

#include <zce/zvm.h>
#include <functional>
#include <string>

/**
 * @brief Registers a Lua extension library with the zua virtual machine.
 *
 * A module declares a static instance inside its zXXX_init() so that its
 * openlibs callback is appended to the zua initialisation chain.
 */
class ZCE_API zua_register {
  public:
    /// @param openlibs Callback invoked on each new lua_State to register libraries.
    zua_register(lpfn_lual_openlibs openlibs);

    ~zua_register();
};

namespace zce {

namespace zvm {

/**
 * @brief Registers a vmtype and its Machine factory with zvm.
 *
 * A module declares a static instance inside its zXXX_init(), binding a vmtype
 * string to a factory. The host then boots the matching Machine according to
 * zvm_t::vmtype.
 */
class ZCE_API VirtualMachineRegister {
  public:
    typedef std::function<zce::SmartPtr<::zce::zvm::Machine>(
        const zdp_base::zvm_t& vm, const zce::SmartPtr<VirtualMachineStub>& this_ptr,
        zce::RefBlock& dblock)>
        lpfn_zvm_creator;

    /**
     * @param suffix  vmtype string to bind, e.g. "TaijiMPC".
     * @param creator Factory producing the Machine for that vmtype.
     */
    VirtualMachineRegister(const std::string& suffix, lpfn_zvm_creator creator);

    ~VirtualMachineRegister();
};

// ── Reverse calls (a server calling back into its caller, e.g. progress) ──────
//
// Both helpers below pass zce::SmartPtr<zce::Object> as an opaque handle on
// purpose, so that modules never need the definitions of RpcStream / Proxy:
// converting between them requires dynamic_cast on complete types, and those
// types live only in libzce's internal headers. The conversions are therefore
// kept inside libzce.

/**
 * @brief Wraps the originating stream of a remote call into an opaque handle.
 *
 * Typically called from Machine::call_dblock_from_remote() to remember the
 * caller for a later reverse call.
 *
 * @param stream Stream the current request arrived on.
 * @return Opaque handle, or null if @p stream is null.
 */
zce::SmartPtr<zce::Object> ZCE_API toObjectHandle(const zce::SmartPtr<RpcStream>& stream);

/**
 * @brief Gets, or boots, a vm pointing back at @p svc_name on the caller side.
 *
 * The returned handle can be used directly as the target of
 * VirtualMachineStub::rpc_call_*. An existing reusable vm is returned when one
 * is present, otherwise a new one is booted. Safe under concurrent callers: if
 * a racing caller wins the insert, the loser picks up the winner's vm rather
 * than failing.
 *
 * @param stream_handle Handle previously obtained from toObjectHandle().
 * @param svc_name Service name on the caller side to connect back to.
 * @param default_timeout_ms Default RPC timeout for a newly booted vm.
 * @return Handle to the vm, or null if @p stream_handle is null, is not a
 *         stream handle, or the vm could not be booted.
 */
zce::SmartPtr<zce::Object> ZCE_API getOrBootReuseVM(
    const zce::SmartPtr<zce::Object>& stream_handle, const std::string& svc_name,
    int default_timeout_ms);

}  // namespace zvm

}  // namespace zce
