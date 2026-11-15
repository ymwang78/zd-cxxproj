#pragma once

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <atomic>
#include <memory>
#include <vector>
#include "IProject.h"
#include "IRpcChannel.h"
#include "IRpcChannelFactory.h"
#include "IStormRouter.h"
#include "XocExport.h"
#include "XocInstanceInfo.h"

/**
 * @brief ProjectHost 类 - Solution 和 ProjectInstance 之间的中间层
 *
 * ProjectHost 代表一个项目宿主，可以包含多个 ProjectInstance。
 * - ProjectHost 会持久化到 Solution 文件中
 * - ProjectInstance 不持久化，每次从服务器动态加载
 *
 * 层级关系：
 * Solution (1) → ProjectHost (N) → ProjectInstance (N)
 *
 * 本类是 DLL 插件的公开面（IProject::getProjectHost() 返回它），因此头文件里**不得
 * 出现任何 zce 类型**：服务代理（VirtualMachineProxy）与 StormClient 这两个 zce 成员
 * 藏在 Impl 里，实例元数据一律用 XocInstanceInfo，通信能力经 IRpcChannel /
 * IStormRouter 两个纯虚接口下发。
 *
 * 本类实现 IRpcChannelFactory：插件的 Runtime 层只需要"给我一条通道"，持有那个接口
 * 就够了，不必把整个宿主拖进去（#249）。这里与 StormBackend 的取舍**刻意相反**——
 * 那个接口不给继承是因为它是绕过账本的裸后端，而 createRpcChannel() 本来就是公开的、
 * 无账本可绕，所以直接继承，不再额外套一层适配器。
 */
class XOC_API ProjectHost : public QObject, public IRpcChannelFactory {
    Q_OBJECT

  public:
    explicit ProjectHost(const QString& name, const QString& description = QString(),
                         QObject* parent = nullptr);
    ~ProjectHost();

    // 基本属性
    QString getName() const { return m_name; }
    void setName(const QString& name);

    QString getDescription() const { return m_description; }
    void setDescription(const QString& description);

    QString getHostKey() const { return m_hostKey; }
    void setHostKey(const QString& key);

    // 服务端点信息（可选，用于从特定服务器加载实例）
    QString getServiceHost() const { return m_serviceHost; }
    void setServiceHost(const QString& host);

    int getServicePort() const { return m_servicePort; }
    void setServicePort(int port);

    int getStormPort() const { return m_stormPort; }
    // 注意：storm_port 从服务器自动获取，不提供 setStormPort

    const QString& getHostTopic() const noexcept { return m_hostTopic; }

    QString getVersion() const { return m_version; }

    QString getEndpointKey() const;

    void newProject(const QString& projectName, const QString& projectType,
                    const QString& describe);

    // vmtype/vmpath 由调用方从项目类型元数据（ProjectTypeInfo）取得，不在此硬编码。
    // configData 是 QByteArray 而非 zce::RefBlock：RefBlock 背后是 zce 内存池，
    // 不能过 DLL 边界，核心内部再包回 RefBlock 去发 uploadVM。
    void loadProject(const QString& projectName, const QString& vmType, const QString& vmPath,
                     const QByteArray& configData, bool forceReplace = false);

    void deleteProject(IProject* project);
    // Deletion is a host operation and also works for instances without a loaded plugin.
    void deleteProjectInstance(const QString& instanceName);
    void renameProjectInstance(const QString& oldInstanceName, const QString& newInstanceName);
    void startProject(const QString& instanceName);
    void stopProject(const QString& instanceName);
    void restartProject(const QString& instanceName);

    // 本地项目管理（基于 ProjectInstance 的本地对象）
    int addProject(IProjectPtr project);
    void removeProject(int index);
    // 移除并析构一个已加载的工程：先 emit projectRemoved，槽返回后再析构。
    // 在嵌套事件循环里（模态对话框的 exec()、QMenu::exec()、QInputDialog / QMessageBox 的
    // 静态函数）或有模态窗口时调用，只登记、不析构——那时栈上的对话框和它的调用方都还拿着
    // 页面和工程（#342）。等回到主循环、模态窗口都关了才真正移除，所以调用返回时工程可能
    // 还在 getProjects() 里；重复调用只算一次。
    void removeProject(IProject* project);
    IProject* getProject(int index) const;
    // 按工程名（IProject::getProjectName）查，是"同名工程是否已存在"那种语义。
    IProject* findProjectByName(const QString& projectName) const;
    // 按远端实例名（IRemoteProject::getRemoteInstance().vmName）查已加载的工程。工程名不保证
    // 等于实例名（KeelSim 连上后报的是组态里的 project_name），所以树节点、双击打开、实例同步
    // 一律走这个，不走 findProjectByName。
    IProject* findProjectByInstance(const QString& instanceName) const;
    const std::vector<IProjectPtr>& getProjects() const { return m_projects; }
    int getProjectCount() const { return m_projects.size(); }
    void clearProjects();

    // 根据 ProjectInstance 列表同步导航树节点信息（不创建 IProject，仅保存实例列表）
    void syncProjectsFromInstances(const std::vector<XocInstanceInfo>& instances);
    const std::vector<XocInstanceInfo>& getInstances() const { return m_instances; }

    // 双击 Project 节点时加载项目（若未加载则调用 ProjectFactory::loadProject）
    IProject* getOrLoadProjectForInstance(const QString& instanceName);

    // 连接管理
    bool isConnected() const { return m_connected; }
    void setConnected(bool connected);

    bool isStormConnected() const { return m_stormConnected; }

    /**
     * @brief Storm 广播的订阅/路由入口（框架持有唯一 StormClient）
     *
     * 永不为空，宿主整个生命周期内是同一个对象。未连接时 subscribe 返回负值，
     * 连上（或重连上）后框架会自动补订阅。
     */
    IStormRouter* stormRouter() const;

    /**
     * @brief 为某个远程实例建一条 RPC 通道
     *
     * 通道所有权交给调用方（插件）；析构即关闭。需要第二条独立 TCP（大块历史/模型包
     * 上传）时用返回通道的 createAuxChannel()。
     *
     * @return 失败（实例缺 vmName/vmPort，或宿主未配置地址）返回空指针
     */
    IRpcChannelPtr createRpcChannel(const XocInstanceInfo& instance) override;

    void disconnectStorm();

    // 从服务器刷新实例列表
    void refreshInstances(bool is_retry = false);
    void refreshStorm();

    // 序列化（仅持久化 ProjectHost 元数据，不包括 ProjectInstance）
    QJsonObject toJson() const;
    bool fromJson(const QJsonObject& json);

    // Project-scoped UI state persisted inside the owning Solution file.
    QJsonObject getProjectUiViewState(const QString& projectKey,
                                      const QString& viewScope) const;
    bool setProjectUiViewState(const QString& projectKey,
                               const QString& projectName,
                               const QString& viewScope,
                               const QJsonObject& state);
    void removeProjectUiViewState(const QString& projectKey, const QString& viewScope);
    void resetProjectUiState(const QString& projectKey);
    void resetAllProjectUiStates();

    // 状态
    bool isModified() const { return m_isModified; }
    void setModified(bool modified = true);

    /**
     * @brief Storm 广播的总入口（**核心内部使用**，由 InstanceStormClient 的静态回调调用）
     *
     * 在 Storm 的 reactor 线程上被调用：host 级事件就地解包，project 级事件拷贝一份
     * 投递到 Qt 线程后按 stormTopic 路由到 IRemoteProject::handleStormMessage。
     */
    void dispatchStormBroadcast(qint64 topicId, const char* data, quint32 len);

  signals:
    void nameChanged(const QString& name);
    void descriptionChanged(const QString& description);
    void modifiedChanged(bool modified);

    // 项目列表刷新信号
    void projectsRefreshed();

    // 项目被删除信号（当远程实例不存在时）
    void projectRemoved(IProject* project);

    // 远端实例重命名完成信号
    void instanceRenamed(const QString& oldInstanceName, const QString& newInstanceName,
                         IProject* loadedProject);

    // 连接信号
    void connectionError(const QString& error);
    void refreshError(const QString& error);

    // Storm 连接状态信号
    void rpcConnectedChanged(bool connected);
    void stormConnectedChanged(bool connected);

private slots:
    void onProjectDeletionFinished(const QString& instanceName, int resultCode,
                                    const QString& errorMessage);
    void onProjectRenameFinished(const QString& oldInstanceName, const QString& newInstanceName,
                                 int resultCode, const QString& errorMessage);
    void onProjectCreationFinished(const QString& projectName, const QString& projectType,
                                    int resultCode, const QString& errorMessage);

private:
    // listVM 必须在 ProjectHost service 连接真正打开后调用；否则容易得到
    // ZVM_ERROR_INCONNECT，并且旧日志会表现成空错误字符串。
    void queryInstances();
    void requestProjectVmOperation(const QString& instanceName, const char* rpcName,
                                   const char* debugMethodName,
                                   const QString& requestedStatusTemplate,
                                   const QString& failedTitle,
                                   const QString& failedCodeTemplate,
                                   const QString& failedErrorTemplate,
                                   const QString& failedStatusTemplate);
    void applyProjectInstanceUpdate(XocInstanceInfo projectInstance);
    void setStormConnected(bool connected);
    int connectStorm(unsigned short storm_port, std::function<void()> connected_cb,
                     std::function<void()> disconnect_cb);
    void handleStormConnected(unsigned int stormEpoch);
    void handleStormDisconnected(unsigned int stormEpoch);
    void scheduleStormReconnect(unsigned int stormEpoch);
    void onHostBroadCast(qint64 topic, const char* data, quint32 len);

    // === 裸动作：直接落到 StormClient 上，不做任何记账 ===
    // 记账与重连语义全归 ProjectHostStormRouter。这三个是私有普通成员函数，
    // 由 Impl 里的适配器（Impl::Backend）转成 StormBackend 交给路由器 ——
    // 刻意**不**让 ProjectHost 继承 StormBackend：那样插件一个 static_cast 就能拿到
    // 裸后端，绕开去重与账本，把本 PR 修的重复订阅问题重新造出来。
    int stormSubscribe(const QString& topic);
    /// 按**名字**退订（libzce 自己处理 id 作用域、取消抑制与离线残留清理，见 libzce#60）
    int stormUnsubscribe(const QString& topic);
    qint64 stormTopicId(const QString& topic) const;

    qint64 stormTopicIdRawUtf8(const std::string& topic) const;

    /// 第一次连上时把订阅账本灌进底层；失败并已安排重试/重建时返回 false
    bool primeStormSubscriptions(unsigned int stormEpoch);

    /**
     * @brief 灌账本失败后在**同一条连接**上重试
     *
     * 与 scheduleStormReconnect 是两码事：那条是断线路径，底层处在 CONN_NONE、
     * StormClient 自己的 1 秒定时器会把连接接回来，我们什么都不用做；这条路径底层已经
     * CONN_REGISTED，那个定时器不会开火，等谁都等不来，只能自己把账本再灌一次。
     */
    void scheduleStormPrimeRetry(unsigned int stormEpoch);

    /// connected 收尾：清 pending、翻状态、必要时补一次"订阅已重建"的通知
    void finishStormConnected(bool notifyReconnect);

    void setHostTopicSnapshot(const QString& topic);

    friend class ProjectHostStormRouter;

  private:
    // 数据成员
    QString m_name;         // ProjectHost 名称
    QString m_description;  // 描述
    QString m_hostKey;      // 唯一标识键

    // 服务端点信息（可选）
    QString m_serviceHost;      // 服务器地址
    int m_servicePort;          // 服务端口
    int m_stormPort;            // 广播端口
    QString m_hostTopic;        // Host 级别的 topic 字符串（只在 Qt 线程读写）
    // 给 Storm 线程用的 host topic 名快照。
    // 广播回调在 reactor 线程上要立刻判断这条是不是 host 级事件，不能去读 m_hostTopic
    // ——QString 是隐式共享的，Qt 线程正好在 queryInstances 里给它赋值就会撞引用计数。
    // 也**不能**缓存 topic id：id 是服务端在每次注册时分配的，StormClient 断线自恢复后
    // 会换一批，缓存下来就会把 host 事件误判成工程广播然后丢掉。
    // 所以这里只快照名字（不可变的 std::string，靠 shared_ptr 原子换页），
    // 回调侧拿它去做一次实时的 getTopicId。
    std::shared_ptr<const std::string> m_hostTopicName;
    bool m_connected;           // 是否已连接
    bool m_stormConnected;      // Storm 是否已连接
    bool m_refreshing = false;  // 是否正在刷新实例列表
    int m_refreshRetryCount = 0; // 刷新重试计数
    // Storm 客户端代次，防止旧回调覆盖新连接。用 atomic 是因为它还要被 Storm 的
    // reactor 线程读：dispatchStormBroadcast 在那条线程上取代次快照，随广播一起投递到
    // Qt 线程，执行时代次已变就丢弃（topic id 是连接作用域的，重连后会指向别的工程）。
    std::atomic<unsigned int> m_stormConnectionEpoch{0};
    unsigned int m_stormStatusEpoch = 0;     // Storm 状态事件代次，防止延迟断开覆盖重连
    bool m_stormReconnectPending = false;    // 是否已进入 Storm 重连流程
    // 当前这个 StormClient 是否已经把框架的订阅账本灌进去过。
    // 底层同一个 client 断线自恢复时会**自己** resubscribe_all() 之后才回调我们，
    // 那时再重放一遍账本就是每个 topic 订两次（服务端会把广播推两份）。
    // 只有新 boot 出来的、账本还空着的 client 才需要我们灌。
    bool m_stormLedgerPrimed = false;
    // 在同一条已注册连接上重灌账本失败了几次；超过上限才升级成换连接
    int m_stormPrimeRetries = 0;
    QString m_version;          // 服务器版本号

    // 导航树用实例列表（从服务器同步，仅保存节点信息，不创建 IProject）
    std::vector<XocInstanceInfo> m_instances;
    // 已加载的项目列表（仅包含用户双击打开过的项目）
    std::vector<IProjectPtr> m_projects;

    QJsonObject m_projectUiStates;  // projectKey -> { projectName, updatedAt, views }

    // 状态
    bool m_isModified;
    bool m_valid = true;  // 标记对象是否仍然有效（用于保护异步回调）

    // zce 侧成员（服务代理 / StormClient / 订阅表）全部藏在这里，保持本头文件零 zce
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

using ProjectHostPtr = std::shared_ptr<ProjectHost>;
