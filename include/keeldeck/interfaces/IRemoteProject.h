#pragma once

#include "IProject.h"
#include "XocInstanceInfo.h"
#include <QByteArrayView>
#include <QIcon>
#include <QString>
#include <QStringList>
#include <functional>

/**
 * @brief 远程项目（vmtype 插件）通用接口
 *
 * 框架层（ProjectHost / MainWindow / InstanceService / SolutionTreeModel）只依赖本接口
 * 与 IProject，不得出现具体插件类型（如 TaijiMPCProject）。插件把自己的实例初始化、
 * 连接、组态上传、状态展示、Storm 广播分发逻辑收敛到本接口的实现里。
 *
 * 本头文件（连同 framework/interfaces/ 的其余部分）是 DLL 插件的公开 ABI：签名里只许
 * 出现 Qt 与标准库类型，不许出现 libzce 的整数别名、引用计数内存块，或 ptl 生成的
 * zvm_t 之类结构体。服务端下发的实例元数据以 XocInstanceInfo 形式过界，转换只发生在
 * 核心 DLL 内部（见 framework/core/InstanceInfoConvert.h）。
 * tools/check_interface_purity.cmake（CTest 里的 test_interface_purity）会把这条约束卡住。
 *
 * 约定：实现类必须同时继承 QObject，并提供 `statusChanged(QString)` 信号（框架用
 * 字符串式 connect 订阅，用于项目树状态刷新）。
 */
class IRemoteProject : public IProject {
  public:
    ~IRemoteProject() override = default;

    /**
     * @brief 远程连接完成回调：success=false 时 status 为失败原因
     */
    using RemoteConnectCallback = std::function<void(bool success, const QString& status)>;

    // === 实例生命周期 ===

    /**
     * @brief 初始化/更新远程实例信息（取代 ProjectHost 中的 TaijiMPC 特例逻辑）
     * @param instance 服务器下发的实例描述（listVM / UPDATEVM 广播）
     * @param serviceHost 服务器地址
     * @param stormPort Storm 广播端口
     */
    virtual bool initRemoteInstance(const XocInstanceInfo& instance, const QString& serviceHost,
                                    int stormPort) = 0;

    /**
     * @brief 当前远程实例信息（stormTopic / vmPort / vmStatus 等由框架通用逻辑读取）
     */
    virtual const XocInstanceInfo& getRemoteInstance() const = 0;

    /**
     * @brief 发起远程连接（异步）
     *
     * 返回 false 表示当场失败且 done 未被调用、也不会再被调用；
     * 返回 true 表示 done 会（且仅会）被回调一次（可能在返回前同步失败回调）。
     * 再次调用会废弃上一次尚未完成的回调。
     */
    virtual bool connectRemoteInstance(RemoteConnectCallback done) = 0;

    virtual bool isRemoteConnected() const = 0;

    /// Host-managed projects remain editable while their worker is stopped.
    virtual bool requiresInstanceEndpoint() const { return true; }

    // === 组态上传/导出（含插件自己的文件对话框等 UI 交互） ===

    virtual bool uploadProjectConfig() = 0;
    virtual bool exportProjectConfig() { return false; }
    virtual bool canExportProjectConfig() const { return false; }

    // === 树节点状态展示（已加载实例；未加载实例走 ProjectTypeInfo 的 provider） ===

    virtual QString getInstanceStatusText() const = 0;
    virtual QIcon getInstanceStatusIcon() const = 0;

    /**
     * @brief 实例是否处于"已启动"状态（含插件自定义状态位；用于右键菜单 Start 使能）
     */
    virtual bool isInstanceStarted() const { return false; }

    /**
     * @brief 停止实例前需要确认的进行中操作名（本地化文本；空 = 无需确认）
     */
    virtual QStringList getActiveOperationNames() const { return QStringList(); }

    /**
     * @brief 实例是否仍在运行（删除保护用）
     *
     * 比 isOperationRunning() 更宽：还要包含插件的"运行中"状态位（如 zmpc 的
     * ProjectStatusRunning），即便当前没有任何具体操作在进行，也不允许删除。
     * 默认回退到具体操作判定。
     */
    virtual bool isInstanceRunning() const { return isOperationRunning(); }

    // === Storm 广播路由（取代 InstanceService 中的硬编码分发） ===

    /**
     * @brief 处理路由到本项目 stormTopic 的广播
     *
     * @param topic 当前连接上的 topic id，仅用于插件自己订阅了多个 topic 时分流；
     *              不要持久化，重连后会变（见 IStormRouter::topicId）
     * @param data  广播体。**只在本次调用期间有效**，需要留存请自行拷贝
     * @return true 表示已处理
     */
    virtual bool handleStormMessage(qint64 topic, QByteArrayView data) = 0;
};
