#pragma once

#include <QString>
#include <functional>

/**
 * @brief 工程手里"还没交出去"的改动（可选接口）
 *
 * 远程工程的草稿（脚本编辑器里改了还没 Save 的内容之类）只活在工程对象里，后端并不知道。
 * 工程对象一析构就没了：退出 KeelDeck、关 / 换 solution、删 ProjectHost 都会走到。
 * 框架在这些时机之前、以及用户关掉工程 Tab 时，按 dynamic_cast 取本接口逐个询问
 * （MainWindow::flushUnsavedProjectChanges），让用户选"保存 / 放弃 / 取消"。
 *
 * 与 ILocalProject 的分工：本地工程"改了没落盘"由 ILocalProject::isProjectModified()
 * 回答，保存就是 saveToFile()，框架同步地弹另存为。本接口管的是保存要跟后端打交道
 * （异步、可能失败）的那一类；本地工程若另有这类状态也可以实现它，框架同样会问。
 *
 * **为什么是独立的混入接口，而不是 IProject / IRemoteProject 上的虚函数。** 那两个是跨
 * DLL 边界的纯虚接口，加虚函数会改虚表布局，按 XocAbi.h 必须 +1 XOC_PLUGIN_ABI_VERSION，
 * 所有既有插件二进制一律被拒载、必须重编。本接口不派生自 IProject，实现类自己多继承它；
 * 老插件没有实现它，dynamic_cast 拿到 nullptr，框架按"没有未保存的改动"处理——正是加它
 * 之前的行为。所以不动 ABI 版本。
 *
 * 本头文件属于插件公开 ABI：签名里只许出现 Qt 与标准库类型。
 *
 * 约定：实现类应同时继承 QObject（IRemoteProject 本来就要求）。保存期间工程对象若被
 * 析构，框架靠 QObject::destroyed 把那一笔记成失败；不是 QObject 的实现得不到这层兜底。
 */
class IUnsavedChanges {
  public:
    virtual ~IUnsavedChanges() = default;

    /**
     * @brief 保存完成回调：ok=false 时 error 是给用户看的失败原因（可以为空）
     */
    using SaveCallback = std::function<void(bool ok, const QString& error)>;

    /**
     * @brief 有没有关掉就会丢的改动
     *
     * 答不准会直接表现为数据丢失：少答 true 就是关闭时不提示、改动随退出消失。
     * 会被频繁调用（每次关闭 / 关 Tab 都问），不要在这里发 RPC。
     */
    virtual bool hasUnsavedChanges() const = 0;

    /**
     * @brief 一句话说明没保存的是什么，例如"入口脚本 main.py"
     *
     * 框架拼进询问框里，工程名已经由框架给出，这里不必重复。可以返回空串。
     */
    virtual QString unsavedChangesSummary() const = 0;

    /**
     * @brief 把改动交给后端（异步）
     *
     * 与 IRemoteProject::connectRemoteInstance 同一套约定：
     * 返回 false 表示当场失败，done 未被调用、也不会再被调用；
     * 返回 true 表示 done 会（且仅会）被回调一次（可能在返回前同步回调）。
     * 能说清失败原因时，宁可返回 true 并同步 done(false, 原因)，框架会把原因转给用户。
     *
     * done(true) 之后 hasUnsavedChanges() 必须为 false（除非用户期间又改了），
     * 框架据此在重做被中止的关闭动作时不再重复询问。
     */
    virtual bool saveUnsavedChanges(SaveCallback done) = 0;

    /**
     * @brief 放弃改动，回到后端那一份
     *
     * 返回后 hasUnsavedChanges() 必须为 false。用户在关 Tab 时选"放弃"也走这里，
     * 所以工程对象之后还会接着用，不能把自己弄成不可用的状态。
     */
    virtual void discardUnsavedChanges() = 0;
};
