#pragma once

#include <QString>
#include <functional>

class QObject;

/**
 * @brief Storm 广播的订阅与路由（框架提供，插件只用不实现）
 *
 * 一个 ProjectHost 只有一个 StormClient，由框架持有。插件按 topic 名订阅，收到的广播
 * 由框架按 topic 匹配到对应的 IRemoteProject::handleStormMessage。插件不接触 StormClient、
 * 不接触 topic id 的计算规则、更不参与重连——重连与 epoch（防止旧回调覆盖新连接）
 * 全部关在框架内部，插件只收 connected/disconnected 两个通知。
 *
 * 订阅按 topic **名字**做，不是 id：id 由服务端在连接时分配，重连后可能变化，让插件
 * 存 id 就等于让它处理重连语义。框架在重连后自动用名字重新订阅。
 */
class IStormRouter {
  public:
    virtual ~IStormRouter() = default;

    /// 连接状态回调：connected=true 表示已连上（重连成功也会来一次）
    using ConnectionCallback = std::function<void(bool connected)>;

    virtual bool isConnected() const = 0;

    /**
     * @brief 订阅一个 topic（幂等）
     *
     * **框架已经替你订过本工程的 stormTopic**：打开工程时框架就会订阅，插件只有需要
     * 额外 topic 时才要自己调。重复订阅同一个 topic 不会真的再打一次底层 subscribe——
     * 那会让服务端对同一条广播多推一份。
     *
     * @return >= 0 成功；< 0 失败（未连接时也算失败，连上后框架会照账本自动补订阅）
     */
    virtual int subscribe(const QString& topic) = 0;

    virtual int unsubscribe(const QString& topic) = 0;

    /**
     * @brief topic 名 → 当前连接上的 topic id
     *
     * 只用于和 handleStormMessage 收到的 topic 做比对（一个插件订阅了多个 topic 时
     * 拿它做分流）。不要持久化：重连后失效。未连接时返回 0。
     */
    virtual qint64 topicId(const QString& topic) const = 0;

    /**
     * @brief 注册连接状态监听
     *
     * 回调 marshal 到 context 所属线程执行；context 销毁后自动注销。
     * 同一个 context 重复注册会覆盖上一次。
     */
    virtual void addConnectionListener(QObject* context, ConnectionCallback onStateChanged) = 0;

    virtual void removeConnectionListener(QObject* context) = 0;
};
