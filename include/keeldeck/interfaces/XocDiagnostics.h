#pragma once

#include "XocExport.h"
#include <QString>

/**
 * @brief 插件可用的日志与错误码格式化
 *
 * 插件不能用 ZCE_ERROR / keelDeckLog：前者是 libzce 的宏（要 zlog 全局状态），后者的头
 * 文件 KeelDeck.h 直接 include 了 zce_config.h 与 libidh。这里给出一层不带 zce 类型的
 * 转发，实现编在核心 DLL 里，插件的日志因此和框架落在同一个 LogView / 同一个 zlog 文件。
 *
 * 级别取值与 zce 的 ZLOG_* 一致（0..5），插件不必知道那个枚举。
 */
enum class XocLogLevel : int {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Fatal = 5,
};

/// 写一条日志（线程安全：日志层自己带锁，可从任意线程调用）
XOC_API void xocLog(XocLogLevel level, const QString& message);

/**
 * @brief printf 风格的重载
 *
 * 存在的理由只有一个：把大量既有的 ZCE_ERROR((ZLOG_x, "fmt", ...)) 调用点原样搬过来，
 * 不必逐条改写成 QString 拼接（改写就意味着逐条重新校对文案）。新代码优先用上面的
 * QString 版本。格式化在核心侧做，单条上限 2048 字节，超出截断。
 *
 * 注意重载决议：字面量 "text" 精确匹配 const char*，走的是本重载而不是 QString 版本，
 * 与被替换掉的 keelDeckLog 完全一致——含 %% 的字面量该怎么写还怎么写。
 */
XOC_API void xocLog(XocLogLevel level, const char* format, ...);

/**
 * @brief 把 RPC 返回码格式化成 "0xXXXXXXXX (描述)"
 *
 * IRpcChannel 的回调给的是 zce 的负错误码，描述表在 libzce 里，插件查不到，
 * 所以要走这个函数拿可读文本。
 *
 * 返回的描述可能是中文（ZMPC 码走核心的 ZMpcErrorText 文案）。上面 xocLog 的
 * printf 重载按 UTF-8 解码缓冲区，所以要 toUtf8() 而不是 toLocal8Bit()；
 * 能直接用 QString 重载的就别绕这一圈。
 */
XOC_API QString xocFormatRpcError(int errcode);

/**
 * @brief 同上，但优先采用调用方给出的描述
 *
 * 解包失败一类的错误，应答里带的 errdesc 比错误码表更具体；description 为空时
 * 退回单参版本的行为（负错误码去查 zce 的描述表）。
 */
XOC_API QString xocFormatRpcError(int errcode, const QString& description);
