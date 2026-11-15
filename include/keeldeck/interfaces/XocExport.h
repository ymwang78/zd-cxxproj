#pragma once

/**
 * @brief 框架核心 DLL（keeldeck_core）的符号可见性宏
 *
 * 三种编译身份：
 * - 编核心本身：定义 XOC_CORE_BUILD → 导出；
 * - 编 exe / 插件 DLL：什么都不定义 → 导入；
 * - 编单元测试（把核心的 .cpp 直接拉进测试可执行文件）：定义 XOC_CORE_STATIC → 空宏。
 *   测试里既没有 DLL 也没有导入库，留着 dllimport 会在链接期找 __imp_ 符号。
 *
 * Linux 侧本期只保证编译不破坏：gcc 默认 visibility 是 default，加 visibility 属性
 * 是为了将来上 -fvisibility=hidden 时不用再动这里。
 */
#if defined(_WIN32)
#    define XOC_DECL_EXPORT __declspec(dllexport)
#    define XOC_DECL_IMPORT __declspec(dllimport)
#else
#    define XOC_DECL_EXPORT __attribute__((visibility("default")))
#    define XOC_DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(XOC_CORE_STATIC)
#    define XOC_API
#elif defined(XOC_CORE_BUILD)
#    define XOC_API XOC_DECL_EXPORT
#else
#    define XOC_API XOC_DECL_IMPORT
#endif

/**
 * @brief 插件 DLL 自身的导出宏
 *
 * QPluginLoader 走 Qt 的 qt_plugin_instance 入口，插件通常不需要额外导出任何东西；
 * 保留这个宏是给"插件想再对外暴露 C 接口"的场景，不要用它导出模板实例。
 */
#define XOC_PLUGIN_API XOC_DECL_EXPORT
