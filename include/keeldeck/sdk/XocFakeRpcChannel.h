#pragma once

// ***************************************************************
//  XocFakeRpcChannel —— 可编排时序的假 RPC 通道（header-only，测试专用）
//  -------------------------------------------------------------
//  This file is a part of project KeelDeck.
// ***************************************************************
//
// 为什么需要它（issue #249）：插件里被评审抓到的 P1 全是**异步时序**问题——"首屏回包
// 晚于其后若干拍广播"、"拉取在飞时切软测量"、"提交在飞期间按钮必须一直灰着"。真通道
// 在没有后端的测试进程里只会**同步立即失败**，这类顺序根本构造不出来，只能靠人读代码
// 发现。这里把通道换成一份可编排的假实现：响应先入队，由用例说了算的那一刻才投递。
//
// 与 tests/test_storm_router_ledger.cpp 里的 FakeStormBackend 同一路数，只是那边的缝
// 是 StormBackend，这边是 IRpcChannelFactory。
//
// 三条设计约束：
//   1. **零 zce、零核心**：只 include framework/interfaces/ 与 Qt，编进测试自己的二进制；
//   2. **不自己跑事件循环**：什么时候投递完全由用例决定（flushPending / answer），
//      否则"回包排在第几拍广播之后"就又变成运气问题了；
//   3. **语义对齐 VmRpcChannel**：close() 丢在飞回包且**不**触发 onClose（世代作废），
//      context 销毁后回调直接丢弃，isOpen() 在 open() 成功后立刻为真（连接还没建立）。
//
// 典型用法：
//
//   auto factory = std::make_shared<xoc_test::FakeRpcChannelFactory>();
//   factory->ledger().replyTo("downloadProject", 0, packed_config);
//   SoftSensorModel model(factory.get(), &fake_storm, nullptr);
//   model.setInstance(instance);
//   model.connectInstance(nullptr);       // 通道建好、downloadProject 已投递
//   ... 这中间可以先喂若干拍 Storm 广播 ...
//   factory->ledger().flushPending();     // 现在才让回包到达

#include "IRpcChannel.h"
#include "IRpcChannelFactory.h"
#include "XocInstanceInfo.h"

#include <QByteArray>
#include <QDateTime>
#include <QObject>
#include <QPointer>
#include <QString>
#include <algorithm>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace xoc_test {

/// open() 的行为
enum class FakeOpenMode {
    Immediate,  ///< open() 里就地回 onOpen（多数用例要的那种）
    Deferred,   ///< open() 返回 true 但连接还没建立，等用例 openNow()
    Reject,     ///< open() 返回 false，两个回调都不会来
};

/// 一次 call() 的流水
struct FakeRpcCall {
    int serial = 0;      ///< 全局递增，跨主/辅通道给出唯一的先后顺序
    QString channel;     ///< 通道标签（主通道 "main"，辅通道是 createAuxChannel 的 tag）
    QString method;
    QByteArray payload;  ///< zds 打包后的裸参数体，用例可以自己解回来断言参数
    int timeoutMs = 0;
};

class FakeRpcChannel;

/**
 * @brief 一组假通道共享的账本：编排 + 流水 + 投递队列
 *
 * 编排按**方法名**做、不分通道：辅通道是被测代码按需自己建的，用例够不着它，
 * 要是还得按通道编排就没法用了。流水与在飞计数则是**按通道**分得清的（记了 channel
 * 标签，inFlightCount() 只数自己那条），#249 里"辅通道的排队与按需重建"就靠这个。
 *
 * 投递队列是**全局**一条：主通道的 submitLabSample 与辅通道的 downloadHisDataRange
 * 谁先谁后要能表达出来，各通道各排一队就说不清了。
 */
class FakeRpcLedger {
  public:
    // =====================================================================
    // 编排：这个方法回什么
    // =====================================================================

    /// 常驻响应：之后每次调用都回这一份
    void replyTo(const QString& method, int errcode, const QByteArray& payload = QByteArray()) {
        Script& script = m_scripts[method];
        script.has_sticky = true;
        script.sticky = Reply{errcode, payload, false, 0};
    }

    /// 一次性响应：按压入顺序消耗，用完之后回落到常驻那份（没有就挂着）
    void replyOnceTo(const QString& method, int errcode,
                     const QByteArray& payload = QByteArray()) {
        m_scripts[method].queued.push_back(Reply{errcode, payload, false, 0});
    }

    /// call() 当场失败（返回负数），done **不会**被调用——对应通道拒绝投递那条路径
    void failDispatchOn(const QString& method, int errcode = -1) {
        Script& script = m_scripts[method];
        script.has_sticky = true;
        script.sticky = Reply{0, QByteArray(), true, errcode};
    }

    /// 撤掉编排：之后这个方法的调用一直挂在飞，直到用例 answer() 或通道关闭。
    /// 这是**默认**行为（没编排过的方法就是挂着），显式写出来是为了让用例读得懂
    void holdCallsTo(const QString& method) { m_scripts.erase(method); }

    void clearScripts() { m_scripts.clear(); }

    // =====================================================================
    // 流水
    // =====================================================================

    const std::vector<FakeRpcCall>& calls() const { return m_calls; }

    std::vector<FakeRpcCall> callsTo(const QString& method) const {
        std::vector<FakeRpcCall> out;
        for (const FakeRpcCall& c : m_calls) {
            if (c.method == method) out.push_back(c);
        }
        return out;
    }

    int callCount(const QString& method) const {
        int n = 0;
        for (const FakeRpcCall& c : m_calls) {
            if (c.method == method) ++n;
        }
        return n;
    }

    /// 最后一次调用；从没调过返回 nullptr
    const FakeRpcCall* lastCall(const QString& method) const {
        for (auto it = m_calls.rbegin(); it != m_calls.rend(); ++it) {
            if (it->method == method) return &(*it);
        }
        return nullptr;
    }

    void clearCalls() { m_calls.clear(); }

    // =====================================================================
    // 投递
    // =====================================================================

    /// 已经有响应、正等着投递的条数
    int readyCount() const {
        return static_cast<int>(std::count_if(m_pending.begin(), m_pending.end(),
                                              [](const Pending& p) { return p.ready; }));
    }

    /// 在飞但还没有响应的条数（没编排过的方法，或用例故意挂着的）
    int heldCount() const {
        return static_cast<int>(std::count_if(m_pending.begin(), m_pending.end(),
                                              [](const Pending& p) { return !p.ready; }));
    }

    int pendingCount() const { return static_cast<int>(m_pending.size()); }

    /**
     * @brief 投递**当前**已就绪的响应，按调用顺序（serial）来
     *
     * 只投递进入本函数那一刻就绪的那批：回调里新发出去的请求即使当场就绪，也留到下一次
     * flush。这样"一轮"是可数的，用例能钉住"重拉了几次"而不是靠收敛。
     *
     * @return 实际投递了几条
     */
    int flushPending() {
        std::vector<int> serials;
        for (const Pending& p : m_pending) {
            if (p.ready) serials.push_back(p.serial);
        }
        std::sort(serials.begin(), serials.end());

        int delivered = 0;
        for (int serial : serials) {
            auto it = std::find_if(m_pending.begin(), m_pending.end(),
                                   [serial](const Pending& p) { return p.serial == serial; });
            // 上一条回调里把通道关了 / 重开了，这条已经被丢弃
            if (it == m_pending.end()) continue;
            Pending item = std::move(*it);
            m_pending.erase(it);
            ++delivered;
            // context 销毁后回调直接丢弃——与 xoc::postToContext 的守卫语义一致
            if (item.done && item.context) item.done(item.errcode, item.payload);
        }
        return delivered;
    }

    /// 一直 flush 到没有就绪响应为止（回调里又发请求、请求又就绪的链式场景）
    int flushUntilIdle(int max_rounds = 32) {
        int total = 0;
        for (int i = 0; i < max_rounds; ++i) {
            const int n = flushPending();
            if (n == 0) break;
            total += n;
        }
        return total;
    }

    /// 只投递最早的一条就绪响应
    int flushOne() {
        int best = -1;
        for (const Pending& p : m_pending) {
            if (p.ready && (best < 0 || p.serial < best)) best = p.serial;
        }
        if (best < 0) return 0;
        auto it = std::find_if(m_pending.begin(), m_pending.end(),
                               [best](const Pending& p) { return p.serial == best; });
        Pending item = std::move(*it);
        m_pending.erase(it);
        if (item.done && item.context) item.done(item.errcode, item.payload);
        return 1;
    }

    /**
     * @brief 给**最早一条还挂着**的 method 调用补一个响应（就绪，等 flush）
     *
     * 编排（replyTo）是"下一次调用回什么"，这个是"已经在飞的那次回什么"。
     * #5 那种"提交在飞期间按钮必须一直灰着"就靠它：先挂着，断言按钮灰的，再 answer。
     */
    bool answer(const QString& method, int errcode, const QByteArray& payload = QByteArray()) {
        for (Pending& p : m_pending) {
            if (!p.ready && p.method == method) {
                p.ready = true;
                p.errcode = errcode;
                p.payload = payload;
                return true;
            }
        }
        return false;
    }

    /// 给**所有**挂着的 method 调用补同一个响应
    int answerAll(const QString& method, int errcode, const QByteArray& payload = QByteArray()) {
        int n = 0;
        for (Pending& p : m_pending) {
            if (!p.ready && p.method == method) {
                p.ready = true;
                p.errcode = errcode;
                p.payload = payload;
                ++n;
            }
        }
        return n;
    }

    // =====================================================================
    // 通道注册表（辅通道是被测代码自己建的，用例得有办法够到它）
    // =====================================================================

    const std::vector<FakeRpcChannel*>& channels() const { return m_channels; }

    /// 现存的、标签匹配的**最后一条**通道。辅通道断开后会被重建，要的是新那条
    FakeRpcChannel* channel(const QString& tag) const {
        for (auto it = m_channels.rbegin(); it != m_channels.rend(); ++it) {
            if (channelTag(*it) == tag) return *it;
        }
        return nullptr;
    }

    /// 新建的通道默认用哪种 open 行为（辅通道够不着，只能这样预设）
    void setDefaultOpenMode(FakeOpenMode mode) { m_defaultOpenMode = mode; }
    FakeOpenMode defaultOpenMode() const { return m_defaultOpenMode; }

    /// 固定时钟（0 = 用真实时钟）。lastActivityMs() 的断言要可复现时用
    void setNowMs(qint64 ms) { m_nowMs = ms; }
    qint64 nowMs() const { return m_nowMs != 0 ? m_nowMs : QDateTime::currentMSecsSinceEpoch(); }

    /// 某条通道上还没回调的请求数（就绪但没 flush 的也算在飞——响应还没到调用方手里）
    int inFlightFor(const FakeRpcChannel* channel) const {
        return static_cast<int>(
            std::count_if(m_pending.begin(), m_pending.end(),
                          [channel](const Pending& p) { return p.owner == channel; }));
    }

    // ---- 以下几个由 FakeRpcChannel 调用，用例一般用不到 ----

    void registerChannel(FakeRpcChannel* channel) { m_channels.push_back(channel); }

    void unregisterChannel(FakeRpcChannel* channel) {
        m_channels.erase(std::remove(m_channels.begin(), m_channels.end(), channel),
                         m_channels.end());
    }

    /// 丢掉某条通道上所有在飞的回调（close() / 重新 open() / 析构）
    void dropChannel(const FakeRpcChannel* channel) {
        m_pending.erase(
            std::remove_if(m_pending.begin(), m_pending.end(),
                           [channel](const Pending& p) { return p.owner == channel; }),
            m_pending.end());
    }

    int recordCall(FakeRpcChannel* channel, const QString& method, const QByteArray& payload,
                   int timeoutMs, QObject* context, IRpcChannel::ResultCallback done);

  private:
    struct Reply {
        int errcode = 0;
        QByteArray payload;
        bool dispatch_fails = false;
        int dispatch_errcode = 0;
    };
    struct Script {
        std::deque<Reply> queued;
        bool has_sticky = false;
        Reply sticky;
    };
    struct Pending {
        int serial = 0;
        FakeRpcChannel* owner = nullptr;
        QPointer<QObject> context;
        IRpcChannel::ResultCallback done;
        QString method;
        int errcode = 0;
        QByteArray payload;
        bool ready = false;
    };

    /// 定义在 FakeRpcChannel 之后（要取它的 tag）
    static QString channelTag(const FakeRpcChannel* channel);

    std::map<QString, Script> m_scripts;
    std::vector<FakeRpcCall> m_calls;
    std::vector<Pending> m_pending;
    std::vector<FakeRpcChannel*> m_channels;
    FakeOpenMode m_defaultOpenMode = FakeOpenMode::Immediate;
    qint64 m_nowMs = 0;
    int m_serial = 0;
};

/**
 * @brief 一条假通道
 *
 * 连接状态由用例翻：openNow() 让 Deferred 的连接建立，closeNow() 模拟对端断开
 * （触发 onClose 并丢掉在飞回包）。close() 是插件主动关，与 VmRpcChannel 一样
 * **不**触发 onClose。
 */
class FakeRpcChannel : public IRpcChannel {
  public:
    FakeRpcChannel(std::shared_ptr<FakeRpcLedger> ledger, QString tag, QString endpoint)
        : m_ledger(std::move(ledger)), m_tag(std::move(tag)), m_endpoint(std::move(endpoint)) {
        m_openMode = m_ledger->defaultOpenMode();
        m_ledger->registerChannel(this);
    }

    ~FakeRpcChannel() override {
        // 真通道析构 = destroyVm()：世代作废，在飞回包与 onClose 一律不再触发
        m_ledger->dropChannel(this);
        m_ledger->unregisterChannel(this);
    }

    // ---- IRpcChannel ----

    bool open(QObject* context, StateCallback onOpen, StateCallback onClose) override {
        ++m_openCount;
        // 重复 open 视为重建连接：在飞回包与**上一代的状态回调**一起作废
        //（VmRpcChannel::open 里先 destroyVm() → generation.retire()，就是这个效果）
        m_ledger->dropChannel(this);
        retireCallbacks();

        if (m_openMode == FakeOpenMode::Reject) {
            // 契约：open() 返回 false 时两个回调都不会触发。所以这一路**一个都不留**——
            // 存下来的话，调用方留着通道再 openNow() 就能唤出一个"底层已经拒绝了这次
            // 连接"之后不可能发生的 onOpen
            m_open = false;
            return false;
        }

        m_context = context;
        m_onOpen = std::move(onOpen);
        m_onClose = std::move(onClose);
        // boot() 一成功 isOpen() 就是真，哪怕 TCP 还没接上——与真通道一致
        m_open = true;
        if (m_openMode == FakeOpenMode::Immediate) fireOpen();
        return true;
    }

    void close() override {
        m_open = false;
        m_ledger->dropChannel(this);
        // 世代作废：在飞回包**与两个状态回调**一起失效。onClose 因此不触发（这是
        // "我自己关的"），openNow() 之后也不会再唤出旧的 onOpen —— VmRpcChannel 那边
        // destroyVm() 一 retire，属于旧连接的 open_cb / close_cb 就再也进不来了。
        // 少了这一步，用例能构造出"close() 之后又重连成功"这种生产环境不可能的时序。
        retireCallbacks();
    }

    bool isOpen() const override { return m_open; }

    QString endpoint() const override { return m_endpoint; }

    int call(const QString& method, const QByteArray& payload, int timeoutMs, QObject* context,
             ResultCallback done) override {
        if (!m_open) return -1;  // 关着的通道当场失败，与 VmRpcChannel 一致
        m_lastActivityMs = m_ledger->nowMs();
        return m_ledger->recordCall(this, method, payload, timeoutMs, context, std::move(done));
    }

    std::unique_ptr<IRpcChannel> createAuxChannel(const QString& tag = QString()) override {
        ++m_auxRequests;
        if (m_auxCreationFails) return nullptr;
        const QString aux_tag = tag.isEmpty() ? QStringLiteral("aux") : tag;
        return std::make_unique<FakeRpcChannel>(m_ledger, aux_tag, m_endpoint);
    }

    int inFlightCount() const override { return m_ledger->inFlightFor(this); }

    qint64 lastActivityMs() const override { return m_lastActivityMs; }

    // ---- 编排 ----

    void setOpenMode(FakeOpenMode mode) { m_openMode = mode; }
    FakeOpenMode openMode() const { return m_openMode; }

    /**
     * @brief Deferred 模式下由用例把连接"接通"；底层重连也是再来一次 onOpen，可重复调
     *
     * close() 之后回不来：那一步已经把回调作废了，要再有连接必须重新 open()。
     * closeNow()（对端断开）之后可以——真通道那条 TCP 还在，底层自己会把它接回来。
     */
    bool openNow() {
        if (!m_onOpen || !m_context) return false;
        m_open = true;
        fireOpen();
        return true;
    }

    /// 对端断开：在飞回包全丢，onClose 触发一次。回调**不**作废——底层还会重连
    bool closeNow() {
        const bool was_open = m_open;
        m_open = false;
        m_ledger->dropChannel(this);
        if (!was_open) return false;
        ++m_closeCount;
        if (m_onClose && m_context) m_onClose();
        return true;
    }

    /// 让下一次 createAuxChannel() 返回空指针
    void setAuxCreationFails(bool fails) { m_auxCreationFails = fails; }

    const QString& tag() const { return m_tag; }
    int openCount() const { return m_openCount; }
    int closeCount() const { return m_closeCount; }
    int auxRequestCount() const { return m_auxRequests; }
    FakeRpcLedger& ledger() const { return *m_ledger; }

  private:
    void fireOpen() {
        if (m_onOpen && m_context) m_onOpen();
    }

    /// 世代作废：属于这一代连接的状态回调再也不会触发（对应 CallbackGeneration::retire）
    void retireCallbacks() {
        m_context = nullptr;
        m_onOpen = nullptr;
        m_onClose = nullptr;
    }

    std::shared_ptr<FakeRpcLedger> m_ledger;
    QString m_tag;
    QString m_endpoint;
    QPointer<QObject> m_context;
    StateCallback m_onOpen;
    StateCallback m_onClose;
    FakeOpenMode m_openMode = FakeOpenMode::Immediate;
    bool m_open = false;
    bool m_auxCreationFails = false;
    int m_openCount = 0;
    int m_closeCount = 0;
    int m_auxRequests = 0;
    qint64 m_lastActivityMs = 0;
};

inline QString FakeRpcLedger::channelTag(const FakeRpcChannel* channel) {
    return channel ? channel->tag() : QString();
}

inline int FakeRpcLedger::recordCall(FakeRpcChannel* channel, const QString& method,
                                     const QByteArray& payload, int timeoutMs, QObject* context,
                                     IRpcChannel::ResultCallback done) {
    const int serial = ++m_serial;
    m_calls.push_back(FakeRpcCall{serial, channelTag(channel), method, payload, timeoutMs});

    Reply reply;
    bool has_reply = false;
    auto it = m_scripts.find(method);
    if (it != m_scripts.end()) {
        if (!it->second.queued.empty()) {
            reply = it->second.queued.front();
            it->second.queued.pop_front();
            has_reply = true;
        } else if (it->second.has_sticky) {
            reply = it->second.sticky;
            has_reply = true;
        }
    }

    if (has_reply && reply.dispatch_fails) {
        // 当场失败：不入队、不回调，与真通道 rpc_call_dblock 返回负数时一致
        return reply.dispatch_errcode < 0 ? reply.dispatch_errcode : -1;
    }

    Pending pending;
    pending.serial = serial;
    pending.owner = channel;
    pending.context = context;
    pending.done = std::move(done);
    pending.method = method;
    pending.ready = has_reply;
    if (has_reply) {
        pending.errcode = reply.errcode;
        pending.payload = reply.payload;
    }
    m_pending.push_back(std::move(pending));
    return 0;
}

/**
 * @brief IRpcChannelFactory 的假实现：每次都给一条 FakeRpcChannel
 *
 * 建通道本身也可能失败（实例缺 vmName/vmPort、宿主未配置地址），用 setCreateFails()
 * 把那条分支也覆盖到。
 */
class FakeRpcChannelFactory : public IRpcChannelFactory {
  public:
    explicit FakeRpcChannelFactory(std::shared_ptr<FakeRpcLedger> ledger = nullptr)
        : m_ledger(ledger ? std::move(ledger) : std::make_shared<FakeRpcLedger>()) {}

    IRpcChannelPtr createRpcChannel(const XocInstanceInfo& instance) override {
        m_requested.push_back(instance);
        // 真工厂对不可连接的实例就是回空；假的也照做，免得用例在一个错的前提上通过
        if (m_createFails || !instance.isConnectable()) return nullptr;
        const QString endpoint = QStringLiteral("fake:%1").arg(instance.vmPort);
        return std::make_unique<FakeRpcChannel>(m_ledger, QStringLiteral("main"), endpoint);
    }

    void setCreateFails(bool fails) { m_createFails = fails; }

    /// 每次被要一条通道都记一笔（含被拒的那些）
    const std::vector<XocInstanceInfo>& requested() const { return m_requested; }

    FakeRpcLedger& ledger() { return *m_ledger; }
    const FakeRpcLedger& ledger() const { return *m_ledger; }
    std::shared_ptr<FakeRpcLedger> ledgerPtr() const { return m_ledger; }

    /// 主通道（最后建的那条）；没有返回 nullptr
    FakeRpcChannel* mainChannel() const { return m_ledger->channel(QStringLiteral("main")); }
    /// 辅通道，默认标签是 ZssmRuntime 用的 "bulk"
    FakeRpcChannel* auxChannel(const QString& tag = QStringLiteral("bulk")) const {
        return m_ledger->channel(tag);
    }

  private:
    std::shared_ptr<FakeRpcLedger> m_ledger;
    std::vector<XocInstanceInfo> m_requested;
    bool m_createFails = false;
};

}  // namespace xoc_test
