#pragma once

#include "IProject.h"
#include "XocExport.h"
#include "XocInstanceInfo.h"
#include <QByteArray>
#include <QObject>
#include <QStringList>
#include <QJsonObject>
#include <functional>
#include <memory>

class ProjectHost;

/**
 * @brief Factory for creating different project types
 *
 * This factory implements the plugin registration system. Project types are
 * registered with their creation functions, enabling dynamic project creation
 * without hardcoded dependencies.
 *
 * 工厂是核心 DLL 唯一持有的单例：内置项目类型（BuiltinProjectTypes，#265 起为
 * 0 个）和 DLL 插件（PluginLoader 扫出来的）注册到的是同一份注册表。本头文件属于
 * 插件公开面，签名里不得出现 zce 类型。
 */
class XOC_API ProjectFactory : public QObject {
    Q_OBJECT

  public:
    /**
     * @brief Project type information
     */
    struct ProjectTypeInfo {
        QString typeName;       // Display name (e.g., "Taiji MPC")
        QString typeId;         // Internal ID, must equal remote vmtype (e.g., "TaijiMPC")
        QString description;    // Description text
        QString fileExtension;  // File extension (e.g., "tmpc")
        QIcon typeIcon;         // Type icon
        QString translationContext;  // Optional QObject/QT_TRANSLATE_NOOP context for UI text

        // === 远程实例通用化扩展（未加载实例走这些 provider，已加载走 IRemoteProject） ===

        // 上传实例（uploadVM）时使用的远端配置文件名（XocInstanceInfo::vmPath，如 "_config.zmpc"）
        QString remoteConfigFileName;

        // 未加载实例的树节点状态文本/图标（入参为服务器下发的实例元数据）
        std::function<QString(const XocInstanceInfo&)> instanceStatusTextProvider;
        std::function<QIcon(const XocInstanceInfo&)> instanceStatusIconProvider;

        // 未加载实例是否处于"已启动"状态（右键菜单 Start 使能）
        std::function<bool(const XocInstanceInfo&)> instanceStartedProvider;

        // 未加载实例停止前需确认的进行中操作名（本地化文本；空 = 无需确认）
        std::function<QStringList(const XocInstanceInfo&)> instanceActiveOperationsProvider;

        // host 级"Load Project"：解析本地工程文件，输出实例名与要上传的配置数据块。
        // configData 用 QByteArray 而非 zce::RefBlock——RefBlock 背后是 zce 的内存池，
        // 跨 DLL acquire/release 会落在不同池上；核心内部再转回 RefBlock 去发 RPC。
        std::function<bool(const QString& filePath, QString& projectName, QByteArray& configData,
                           QString& error)>
            projectFileLoader;

        // === 本地工程（host-less）===

        // 该类型能否脱离 ProjectHost 存在：工程数据只在本地文件里，不绑任何远端实例。
        // 置 true 的类型，其 ProjectCreator 必须接受 host == nullptr，产出的 IProject
        // 的 getProjectHost() 返回 nullptr，且不应实现 IRemoteProject——框架按
        // dynamic_cast<IRemoteProject*> 分流 storm 订阅与实例状态展示，本地工程走不到
        // 那些路径。
        bool supportsLocalProject = false;

        // Must match IRemoteProject::requiresInstanceEndpoint(), also before lazy loading.
        bool requiresInstanceEndpoint = true;

        ProjectTypeInfo() = default;
        ProjectTypeInfo(const QString& name, const QString& id, const QString& desc,
                        const QString& ext, const QIcon& ico = QIcon(),
                        const QString& context = QString())
            : typeName(name),
              typeId(id),
              description(desc),
              fileExtension(ext),
              typeIcon(ico),
              translationContext(context) {}
    };

    /**
     * @brief Project creation function type
     */
    using ProjectCreator = std::function<IProjectPtr(ProjectHost*)>;

    /**
     * @brief Get the singleton factory instance
     */
    static ProjectFactory& instance();

    // === Project Type Registration ===
    /**
     * @brief Register a new project type
     * @param typeInfo Project type information
     * @param creator Function to create new instances
     * @return true if registered successfully
     */
    bool registerProjectType(const ProjectTypeInfo& typeInfo, ProjectCreator creator);

    /**
     * @brief Unregister a project type
     * @param typeId Project type ID
     */
    void unregisterProjectType(const QString& typeId);

    /**
     * @brief Check if a project type is registered
     * @param typeId Project type ID
     * @return true if registered
     */
    bool isTypeRegistered(const QString& typeId) const;

    // === Project Creation ===
    /**
     * @brief Create a new project of specified type
     * @param typeId Project type ID
     * @return New project instance (null if type not found)
     */
    IProjectPtr loadProject(ProjectHost* host, const QString& typeId) const;

    /**
     * @brief Create project from file (auto-detects type)
     * @param filePath Path to project file
     * @return Loaded project instance (null if failed)
     */
    IProjectPtr loadProjectFromFile(ProjectHost* host, const QString& filePath) const;

    // === 本地工程（host-less）===
    /**
     * @brief 新建一个不挂 ProjectHost 的本地工程
     *
     * 与 loadProject(nullptr, typeId) 的区别是这里**校验类型确实声明了
     * supportsLocalProject**：没声明的类型（如所有远程实例类型）拿到 host == nullptr
     * 多半会在构造里解引用，宁可在工厂这层直接拒绝。
     *
     * @param typeId 项目类型 ID
     * @return 新建的本地工程；类型未注册或不支持本地工程时返回 null
     */
    IProjectPtr createLocalProject(const QString& typeId) const;

    /**
     * @brief 按扩展名识别类型并从本地文件打开一个本地工程
     * @param filePath 工程文件路径
     * @param error 可选：失败原因（已本地化），用于 UI 提示
     * @return 打开的本地工程；识别失败/类型不支持本地工程/解析失败时返回 null
     */
    IProjectPtr loadLocalProjectFromFile(const QString& filePath,
                                         QString* error = nullptr) const;

    /**
     * @brief 取所有声明了 supportsLocalProject 的类型
     */
    QList<ProjectTypeInfo> getLocalProjectTypes() const;

    /**
     * @brief 本地工程的文件对话框过滤器（只含支持本地工程的类型）
     * @return 过滤器字符串；一个支持本地工程的类型都没有时返回空
     */
    QString getLocalFileFilter() const;

    // === Project Type Information ===
    /**
     * @brief Get list of all registered project types
     * @return List of project type information
     */
    QList<ProjectTypeInfo> getRegisteredTypes() const;

    /**
     * @brief Get project type information by ID
     * @param typeId Project type ID
     * @return Project type info (empty if not found)
     */
    ProjectTypeInfo getTypeInfo(const QString& typeId) const;

    /**
     * @brief 用类型注册的翻译上下文翻译元数据文本（typeName/description）
     *
     * 所有展示类型名/描述的 UI（新建向导、设置页等）统一走这里，避免各处直接用
     * 未翻译的 typeName 造成中英不一致。
     */
    static QString translateTypeText(const ProjectTypeInfo& typeInfo, const QString& source);

    /**
     * @brief 取类型的本地化/OEM 显示名（优先读 branding.ini 的 [ProjectTypeNames]，再走翻译上下文）
     */
    static QString translateTypeName(const ProjectTypeInfo& typeInfo);

    /**
     * @brief 取类型的本地化描述文本
     */
    static QString translateTypeDescription(const ProjectTypeInfo& typeInfo);

    /**
     * @brief Get project type names for UI display
     * @return List of display names
     */
    QStringList getTypeNames() const;

    /**
     * @brief Get project type IDs
     * @return List of type IDs
     */
    QStringList getTypeIds() const;

    /**
     * @brief Get default project type ID based on branding and registered types
     * @return Default project type ID (empty if no types registered)
     */
    QString defaultProjectType() const;

    // === File Handling ===
    /**
     * @brief Get file filter string for Open/Save dialogs
     * @param typeId Specific type ID (empty = all types)
     * @return File filter string
     */
    QString getFileFilter(const QString& typeId = QString()) const;

    /**
     * @brief Detect project type from file
     * @param filePath Path to project file
     * @return Project type ID (empty if not detected)
     */
    QString detectProjectType(const QString& filePath) const;

    /**
     * @brief Get default file extension for project type
     * @param typeId Project type ID
     * @return File extension (empty if not found)
     */
    QString getDefaultExtension(const QString& typeId) const;

    // === Validation ===
    /**
     * @brief Validate JSON data for project type
     * @param jsonData JSON data to validate
     * @param typeId Expected project type ID
     * @return true if valid
     */
    bool validateProjectJson(const QJsonObject& jsonData, const QString& typeId) const;

  signals:
    /**
     * @brief Emitted when a new project type is registered
     * @param typeId Project type ID
     */
    void projectTypeRegistered(const QString& typeId);

    /**
     * @brief Emitted when a project type is unregistered
     * @param typeId Project type ID
     */
    void projectTypeUnregistered(const QString& typeId);

  private:
    ProjectFactory() = default;
    ~ProjectFactory() = default;
    ProjectFactory(const ProjectFactory&) = delete;
    ProjectFactory& operator=(const ProjectFactory&) = delete;

    // Registry of project types
    QMap<QString, ProjectTypeInfo> m_typeInfoMap;  // typeId -> info
    QMap<QString, ProjectCreator> m_creatorMap;    // typeId -> creator function
};

/**
 * @brief Helper macro for registering project types
 *
 * Usage:
 * REGISTER_PROJECT_TYPE("TaijiMPC", "Taiji MPC", "Model Predictive Control", "tmpc",
 *                       QIcon(":/icons/taiji.png"), TaijiMPCProject)
 */
#define REGISTER_PROJECT_TYPE(typeId, displayName, description, extension, icon, className)      \
    do {                                                                                         \
        ProjectFactory::ProjectTypeInfo info(displayName, typeId, description, extension, icon); \
        ProjectFactory::instance().registerProjectType(                                          \
            info, [](ProjectHost* host) -> IProjectPtr {                                         \
                return std::make_unique<className>(host);                                        \
            });                                                                                  \
    } while (0)
