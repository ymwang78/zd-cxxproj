#pragma once

// ***************************************************************
//  XocFakeStormRouter —— IStormRouter 的假实现（header-only，测试专用）
//  -------------------------------------------------------------
//  This file is a part of project KeelDeck.
// ***************************************************************
//
// 与 XocFakeRpcChannel.h 配套。插件里有好几条判定看的是"Storm 连没连上"：
//   - SoftSensorModel::withEchoGuard —— 编辑成功但 Storm 没连着时要**主动**重下载
//     一份组态，否则界面永远停在旧值上；
//   - 导入模型包的兜底 timer —— Storm 连着就等 3 s 回显，没连着就立刻重下载。
// 真路由要拖起 StormClient，这两条分支于是从来没有用例。这里只做一件事：让用例
// 说了算地翻这个状态，并把订阅账本如实记下来。
//
// 注意这**不是** tests/test_storm_router_ledger.cpp 里的 FakeStormBackend：那个假的是
// 路由器底下的 StormBackend（测路由器自己的账本），这个假的是路由器本身（测插件）。

#include "IStormRouter.h"

#include <QObject>
#include <QPointer>
#include <QString>
#include <algorithm>
#include <map>
#include <utility>
#include <vector>

namespace xoc_test {

class FakeStormRouter : public IStormRouter {
  public:
    bool isConnected() const override { return m_connected; }

    /**
     * @brief 订阅（**幂等**，与 ProjectHostStormRouter::subscribe() 逐条对齐）
     *
     * 账本里已有这个 topic 就直接回 0，**不再下发**——真路由正是这么做的，理由写在
     * 它那边：同一个 topic 打开工程时会被订两次（框架一次、插件一次），真往服务端重复
     * subscribe 会让它对同一条广播多推一份。而且幂等判据是**账本**不是连接状态。
     *
     * 只有新 topic 才下发；未连接时下发失败（底层此刻直接回 -1），但账本照记，
     * 连上后由 primeStormSubscriptions 整体重放。
     */
    int subscribe(const QString& topic) override {
        if (topic.isEmpty()) return -1;
        if (std::find(m_topics.begin(), m_topics.end(), topic) != m_topics.end()) return 0;
        m_topics.push_back(topic);
        ++m_dispatchedSubscribes;
        return m_connected ? 0 : -1;
    }

    int unsubscribe(const QString& topic) override {
        if (topic.isEmpty()) return -1;
        m_topics.erase(std::remove(m_topics.begin(), m_topics.end(), topic), m_topics.end());
        ++m_dispatchedUnsubscribes;
        return 0;
    }

    qint64 topicId(const QString& topic) const override {
        if (!m_connected) return 0;
        auto it = m_topicIds.find(topic);
        return it == m_topicIds.end() ? 0 : it->second;
    }

    void addConnectionListener(QObject* context, ConnectionCallback onStateChanged) override {
        removeConnectionListener(context);
        m_listeners.push_back({QPointer<QObject>(context), std::move(onStateChanged)});
    }

    void removeConnectionListener(QObject* context) override {
        m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(),
                                         [context](const Listener& l) {
                                             return l.context.isNull() || l.context == context;
                                         }),
                          m_listeners.end());
    }

    // ---- 编排 ----

    /// 翻连接状态并通知监听器（状态没变就什么都不做，与真路由一致）
    void setConnected(bool connected) {
        if (m_connected == connected) return;
        m_connected = connected;
        pruneDeadListeners();
        // 遍历**副本**：回调里可能 add/removeConnectionListener，那会动到正在遍历的
        // 容器上。真路由（ProjectHostStormRouter::notifyConnectionState）就是这么写的，
        // 而且这条正是插件里常见的写法——SampleRemoteProject 在析构里摘监听器
        const std::vector<Listener> listeners = m_listeners;
        for (const Listener& l : listeners) {
            if (l.context && l.callback) l.callback(connected);
        }
    }

    /// 给某个 topic 安一个 id（只在 connected 时被 topicId() 返回）
    void setTopicId(const QString& topic, qint64 id) { m_topicIds[topic] = id; }

    const std::vector<QString>& topics() const { return m_topics; }
    /// **真正下发到底层**的订阅次数（幂等挡掉的那些不算）——重复订阅的回归就靠它
    int dispatchedSubscribes() const { return m_dispatchedSubscribes; }
    int dispatchedUnsubscribes() const { return m_dispatchedUnsubscribes; }
    int listenerCount() const { return static_cast<int>(m_listeners.size()); }

  private:
    struct Listener {
        QPointer<QObject> context;
        ConnectionCallback callback;
    };

    void pruneDeadListeners() {
        m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(),
                                         [](const Listener& l) { return l.context.isNull(); }),
                          m_listeners.end());
    }

    bool m_connected = false;
    std::vector<QString> m_topics;
    std::map<QString, qint64> m_topicIds;
    std::vector<Listener> m_listeners;
    int m_dispatchedSubscribes = 0;
    int m_dispatchedUnsubscribes = 0;
};

}  // namespace xoc_test
