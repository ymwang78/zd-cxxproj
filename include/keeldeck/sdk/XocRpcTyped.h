#pragma once

// ***************************************************************
//  XocRpcTyped —— 插件侧的类型化 RPC 语法糖（header-only，可选）
//  -------------------------------------------------------------
//  This file is a part of project KeelDeck.
// ***************************************************************
//
// IRpcChannel 只搬字节：call(method, QByteArray payload, ...) → cb(errcode, QByteArray)。
// 打包/解包由插件自己用 ptl 生成的 *_pack 代码完成。本头文件把"按参数列表打包 →
// 发起调用 → 按结果列表解包"这三步包成一个模板函数，等价于 zce 的 callTwoWayAsync，
// 但底座换成了 IRpcChannel。
//
// 为什么放在 framework/sdk/ 而不是 framework/interfaces/：
//   本文件 include 了 <zce/zds_schema.h>，而 framework/interfaces/ 有"零 zce 类型"的
//   硬约束（CI grep 卡）。ABI 面必须干净，语法糖不必——它是模板，整份编进插件自己的
//   二进制，不跨 DLL 边界，也**不要**试图从核心 DLL 导出它的实例化。
//
// 插件侧 zce 白名单：本头文件只用到 zds 的序列化模板（zds_pack_multi / zds_unpack*），
// 那是无状态的纯字节运算，不碰 Reactor / VirtualMachine / BlockPool / Storm 这些带
// 进程级单例的东西。用 tools/check_plugin_symbols.ps1 固化这条边界，不靠口头约定。

#include "IRpcChannel.h"
// zds_schema.h 只给出泛型模板与内建类型的重载。具体消息类型的 zds_pack/zds_unpack
// 是 ptl 生成的普通重载函数，MSVC 对**限定名**的依赖调用是在模板定义处定候选集的，
// 所以这里必须和 zce/zvm.h 一样先把 zdp_base 的那批重载引进来，否则候选集里只剩
// _dummy_t 兜底重载，实例化时报"无法将参数 1 转换为 _dummy_t&"。
// 插件如果要传自己 ptl 生成的类型，同样要在包含本头文件之前 include 它的 *_pack.h。
#include <zce/zdp_base_pack.h>
#include <zce/zds_schema.h>
#include <QByteArray>
#include <QObject>
#include <QString>
#include <cstring>
#include <string>
#include <tuple>
#include <utility>

/**
 * @brief 类型化 RPC 的结果
 *
 * errcode < 0 时 values 保持默认构造值；用 xocFormatRpcError(errcode) 取可读文本。
 * 解包失败也会体现为负的 errcode（来自 zds 的返回码）。
 *
 * errdesc / errorMessage() 与被替换掉的 zce::zvm::VirtualMachineProxy::RpcResult
 * 逐字对齐（成功 "success"、解包失败 "unpack result failed"、传输失败留空），
 * 好让既有调用点的 xocFormatRpcError(errcode, errdesc) 文案一个字都不用改。
 */
template <typename... Results>
struct XocRpcResult {
    int errcode{0};
    std::string errdesc;
    std::tuple<Results...> values;

    bool isSuccess() const noexcept { return errcode >= 0; }

    const std::string& errorMessage() const noexcept { return errdesc; }

    template <std::size_t I>
    auto& get() & {
        return std::get<I>(values);
    }

    template <std::size_t I>
    const auto& get() const& {
        return std::get<I>(values);
    }

    template <std::size_t I>
    auto&& get() && {
        return std::get<I>(std::move(values));
    }
};

namespace xoc_rpc_detail {

/// 把入参依次 zds 打包成裸参数体；无参返回空 QByteArray
template <typename... Args>
inline int packArgs(QByteArray& out, const Args&... args) {
    out.clear();
    if constexpr (sizeof...(Args) == 0) {
        return 0;
    } else {
        int need = zce::zdp::zds_pack_multi(nullptr, 0, nullptr, true, args...);
        if (need < 0) return need;
        if (need == 0) return 0;
        out.resize(need);
        int written = zce::zdp::zds_pack_multi(reinterpret_cast<zce_byte*>(out.data()), need,
                                               nullptr, true, args...);
        if (written < 0) {
            out.clear();
            return written;
        }
        out.resize(written);
        return written;
    }
}

// 逐个类型解包，语义与 zce/zvm.h 的 unpack_recursive_impl 保持一致（只是数据源从
// RefBlock 换成裸缓冲区）：内建类型走 zds_unpack_builtin，vector 走 zds_unpack_array，
// 其余走 zds_unpack(has_prefix=true)。任何一步失败即整体失败。
template <typename... T>
inline auto unpackAt(const zce_byte*, int, std::size_t)
    -> std::enable_if_t<sizeof...(T) == 0, std::pair<int, std::tuple<>>> {
    return {0, {}};
}

template <typename First, typename... Rest>
inline std::pair<int, std::tuple<First, Rest...>> unpackAt(const zce_byte* buf, int size,
                                                           std::size_t offset) {
    int read = 0;
    First value{};
    const zce_byte* cursor = buf + offset;
    const int remain = size - static_cast<int>(offset);
    if (remain <= 0) return {-1, {}};

    if constexpr (zce::zdp::is_builtin_type<First>()) {
        read = zce::zdp::zds_unpack_builtin(value, cursor, remain, nullptr);
    } else if constexpr (zce::zdp::is_vector<First>()) {
        read = zce::zdp::zds_unpack_array(value, cursor, remain, nullptr);
    } else {
        read = zce::zdp::zds_unpack(value, cursor, remain, nullptr, true);
    }
    if (read < 0) return {read, {}};

    auto rest = unpackAt<Rest...>(buf, size, offset + static_cast<std::size_t>(read));
    if (rest.first < 0) return {rest.first, {}};

    return {read + rest.first,
            std::tuple_cat(std::make_tuple(std::move(value)), std::move(rest.second))};
}

/**
 * @brief 把 IRpcChannel 的字节回调包成类型化回调
 *
 * @param empty_payload_is_error
 *        true（新代码的默认）：声明了返回值却收到空响应体判失败——否则调用方会拿着
 *        一堆默认构造值当真数据用；无返回值的调用应当写成 xocRpcCall<>(...)。
 *        false：按 zce::zvm 的 callTwoWayAsync 旧语义放行，errcode 原样交出、
 *        values 留默认值。**只给从 zvm 搬过来、且服务端真的用"空数据块 + errcode"
 *        应答的老接口用**（zmpc 的 setModelValue / setValue 就是这种：返回码走
 *        errcode 通道，载荷恒为空，判成失败会把 NOCHANGE 这类非负成功码吃掉）。
 */
template <typename... Results, typename F>
inline IRpcChannel::ResultCallback typedCallback(F&& done, bool empty_payload_is_error = true) {
    return [cb = std::forward<F>(done), empty_payload_is_error](
               int errcode, const QByteArray& out) mutable {
        XocRpcResult<Results...> result;
        result.errcode = errcode;
        if (errcode < 0) {
            cb(std::move(result));
            return;
        }
        if constexpr (sizeof...(Results) == 0) {
            // 无返回值的调用：拿到什么都不解，errcode 就是全部结论
            result.errdesc = "success";
            cb(std::move(result));
            return;
        } else {
            if (out.isEmpty()) {
                if (empty_payload_is_error) {
                    result.errcode = static_cast<int>(ZCE_ERROR_SHRTLEN);
                } else {
                    result.errdesc = "success";
                }
                cb(std::move(result));
                return;
            }
        }
        auto parsed = unpackAt<Results...>(reinterpret_cast<const zce_byte*>(out.constData()),
                                           static_cast<int>(out.size()), 0);
        if (parsed.first < 0) {
            result.errcode = parsed.first;
            result.errdesc = "unpack result failed";
        } else {
            result.errdesc = "success";
            result.values = std::move(parsed.second);
        }
        cb(std::move(result));
    };
}

}  // namespace xoc_rpc_detail

/**
 * @brief 类型化的异步 RPC
 *
 * ```cpp
 * xocRpcCall<zdp_base::zvm_host_t, std::vector<zdp_base::zvm_t>>(
 *     *channel, this, QStringLiteral("listVM"), 15000,
 *     [this](XocRpcResult<zdp_base::zvm_host_t, std::vector<zdp_base::zvm_t>> r) {
 *         if (!r.isSuccess()) { ...xocFormatRpcError(r.errcode)... return; }
 *         const auto& vms = r.get<1>();
 *     });
 * ```
 *
 * 回调由 IRpcChannel 保证在 context 所属线程执行、且 context 销毁后不再触发，
 * 所以这里不需要再套 QPointer。
 *
 * @return >= 0 已投递；< 0 当场失败（打包失败或通道拒绝），done 不会被调用
 */
template <typename... Results, typename... Args, typename F>
int xocRpcCall(IRpcChannel& channel, QObject* context, const QString& method, int timeoutMs,
               F&& done, const Args&... args) {
    QByteArray payload;
    const int packed = xoc_rpc_detail::packArgs(payload, args...);
    if (packed < 0) return packed;

    // 声明了返回值却收到空响应体一律判失败——不能报成功后把默认构造值交出去。
    return channel.call(method, payload, timeoutMs, context,
                        xoc_rpc_detail::typedCallback<Results...>(std::forward<F>(done), true));
}
