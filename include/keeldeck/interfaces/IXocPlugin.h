#pragma once

#include "XocAbi.h"
#include <QString>
#include <QtPlugin>

class ProjectFactory;

/**
 * @brief 插件 DLL 的入口接口
 *
 * 一个插件 DLL 导出一个实现了本接口的 QObject 派生类，用 Q_PLUGIN_METADATA 标记：
 *
 * ```cpp
 * class SampleRemotePlugin : public QObject, public IXocPlugin {
 *     Q_OBJECT
 *     Q_PLUGIN_METADATA(IID XOC_PLUGIN_IID FILE "SampleRemotePlugin.json")
 *     Q_INTERFACES(IXocPlugin)
 *   public:
 *     int abiVersion() const override { return XOC_PLUGIN_ABI_VERSION; }
 *     ...
 * };
 * ```
 *
 * 加载流程见 PluginLoader：扫描 bin/plugins/*.dll → QPluginLoader::instance() →
 * qobject_cast<IXocPlugin*> → 校验 abiVersion() → registerProjectTypes()。
 * 任一步失败只记日志并跳过该 DLL，主程序照常启动。
 */
class IXocPlugin {
  public:
    virtual ~IXocPlugin() = default;

    /**
     * @brief 插件编译时所用的 ABI 版本，实现一律 `return XOC_PLUGIN_ABI_VERSION;`
     *
     * 和核心自己的常量不相等就拒载。不要手写数字——那样改了头文件也发现不了。
     */
    virtual int abiVersion() const = 0;

    /// 插件唯一标识（用于日志、翻译文件名、重复加载检测），如 "SampleRemote"
    virtual QString pluginId() const = 0;

    /// 人类可读的插件名，用于设置界面展示；缺省回退 pluginId()
    virtual QString displayName() const { return pluginId(); }

    /**
     * @brief 向工厂注册本插件提供的项目类型
     *
     * 在这里填 ProjectTypeInfo 并给出 creator，与内置插件的注册代码完全同形。
     * 允许注册多个类型。
     */
    virtual void registerProjectTypes(ProjectFactory& factory) = 0;

    /**
     * @brief 指定语言的翻译文件路径（.qm）；返回空表示该插件无翻译
     *
     * @param locale 语言代码，与 LanguageManager::currentLanguageCode() 一致（"zh_CN" / "en_US"）
     *
     * 惯例是返回插件 DLL 同级 translations 目录下的 `<pluginId>_<locale>.qm`，
     * 也可以返回 qrc 路径（":/i18n/xxx.qm"）。核心与插件的 lupdate/lrelease 各自独立。
     */
    virtual QString translationFile(const QString& locale) const {
        Q_UNUSED(locale);
        return QString();
    }

    /// 主程序退出前调用，用于释放插件自己的全局资源；卸载 DLL 之前一定会调到
    virtual void shutdown() {}
};

Q_DECLARE_INTERFACE(IXocPlugin, XOC_PLUGIN_IID)
