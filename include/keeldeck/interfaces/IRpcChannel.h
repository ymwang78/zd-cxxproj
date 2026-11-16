#pragma once

#include <QByteArray>
#include <QString>
#include <functional>
#include <memory>

class QObject;

/**
 * @brief 面向单个远程实例的异步 RPC 通道（框架提供，插件只用不实现）
 *
 * 这是 B 模式插件形态的核心：插件 DLL 不链接 libzce 的运行时（Reactor / VirtualMachine /
 * BlockPool），RPC 能力由框架核心以本纯虚接口下发。接口里只出现 Qt 与标准库类型，
 * 引用计数内存块 ↔ QByteArray 的转换、内存池的 acquire/release 全部关在核心 DLL 内部。
 *
 * === 线程模型 ===
 *
 * 所有回调（onOpen / onClose / call 的结果）都由框架 marshal 到调用时传入的 context
 * 所属线程再执行，并且 context 被销毁后回调直接丢弃（内部用 QPointer 守卫）。插件因此
 * 不再需要自己写 callFunctionCallbackInQt + QPointer 那一套——那是本接口取代的旧惯例。
 *
 * call() 本身可以在任意线程调用，返回值只表示"请求是否成功投递"。
 *
 * === 载荷 ===
 *
 * payload / 回调里的 QByteArray 是 zds 序列化后的**裸参数体**，语义与 zce 的
 * callTwoWayAsync 打包结果完全一致：把入参依次 zds_pack 得到的字节流，不含函数名、
 * 不含 RPC 信封。插件用 ptl 生成的 *_pack 代码自行打包/解包（无状态代码，在白名单内）。
 * 无参调用传空 QByteArray。
 *
 * === 生命周期 ===
 *
 * 通道由框架创建、插件持有（IRpcChannelPtr）。析构即关闭；虚析构在核心 DLL 里实现，
 * 所以跨 DLL delete 是安全的（内存回到核心的堆上）。
 */
class IRpcChannel {
  public:
    virtual ~IRpcChannel() = default;

    /// RPC 结果回调：errcode < 0 表示失败（用 xocFormatRpcError 取可读文本），此时 payload 为空
    using ResultCallback = std::function<void(int errcode, const QByteArray& payload)>;

    /// 连接状态回调
    using StateCallback = std::function<void()>;

    /**
     * @brief 建立连接（异步）
     *
     * @param context 回调的线程归属与生命周期守卫；为空则回调不会被调用
     * @param onOpen  连接建立后回调，可能被多次调用（底层重连）
     * @param onClose 连接断开后回调
     * @return true 表示已成功发起；false 表示参数非法或底层拒绝，此时两个回调都不会触发
     */
    virtual bool open(QObject* context, StateCallback onOpen, StateCallback onClose) = 0;

    /// 主动关闭；已在飞的请求回调不会再触发
    virtual void close() = 0;

    virtual bool isOpen() const = 0;

    /// "host:port" 形式的对端地址，仅用于日志与界面展示
    virtual QString endpoint() const = 0;

    /**
     * @brief 发起一次两路 RPC
     *
     * @param method    远端方法名
     * @param payload   zds 打包后的参数体（无参传空）
     * @param timeoutMs 超时毫秒；超时也走 done，errcode 为负
     * @param context   回调的线程归属与生命周期守卫
     * @param done      结果回调，成功发起时保证恰好被调用一次
     * @return >= 0 表示已投递；< 0 表示当场失败且 done 不会被调用
     */
    virtual int call(const QString& method, const QByteArray& payload, int timeoutMs,
                     QObject* context, ResultCallback done) = 0;

    /**
     * @brief 建第二条到同一对端的独立通道
     *
     * 用于把大块数据（历史趋势、模型包上传）从控制面挪开：几十 MB 的响应留在主连接上
     * 会把管理指令的响应帧堵在后面。返回的通道有自己的 TCP、自己的 open/close 回调，
     * 它断开不影响主通道的连接状态——调用方**不要**拿它的 onClose 去重置工程连接态。
     *
     * @param tag 仅用于日志区分，可空
     * @return 新通道；失败返回空指针
     */
    virtual std::unique_ptr<IRpcChannel> createAuxChannel(const QString& tag = QString()) = 0;

    /// 尚未回调的请求数。空闲回收辅助通道前必须确认为 0，否则 destroy 会丢掉响应回调
    virtual int inFlightCount() const = 0;

    /// 最后一次 call() 的时间（QDateTime::currentMSecsSinceEpoch()）；从未调用过为 0
    virtual qint64 lastActivityMs() const = 0;
};

using IRpcChannelPtr = std::unique_ptr<IRpcChannel>;
