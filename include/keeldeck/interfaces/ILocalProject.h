#pragma once

#include "IProject.h"

/**
 * @brief 本地工程（host-less）通用接口
 *
 * 与 IRemoteProject 是一对：远程工程绑 ProjectHost 与远端实例，本地工程的数据只在
 * 本地文件里，getProjectHost() 恒为 nullptr。框架按 dynamic_cast 分流，两边都不必
 * 知道对方存在。
 *
 * **为什么单开一个接口，而不是往 IProject 上加虚函数。** IProject 是跨 DLL 边界的
 * 纯虚接口，往里加（哪怕只是追加到末尾）都会改虚表布局，按 XocAbi.h 的约定必须
 * 同步 +1 XOC_PLUGIN_ABI_VERSION，于是所有既有插件二进制一律被拒载、必须重编。
 * 而这里要加的只是一个「有没有未保存改动」的问答：老插件没有实现它，
 * dynamic_cast 返回 nullptr，框架按"没有未保存改动"处理——正是加这个接口之前的
 * 行为。既然不可能弄坏任何老插件，就不值得拿 ABI 版本去换。
 * （设计文档 identification-offline-signed-dataset.md §7.2 也写明本期不动 ABI 版本。）
 *
 * 本头文件属于插件公开 ABI：签名里只许出现 Qt 与标准库类型。
 *
 * 约定与 IRemoteProject 相同：实现类必须同时继承 QObject 并提供
 * `statusChanged(QString)` 信号。
 */
class ILocalProject : public IProject {
  public:
    ~ILocalProject() override = default;

    /**
     * @brief 工程内容改过但还没落盘
     *
     * 框架在任何会丢掉内存中 solution 的时机之前据此询问用户
     * （MainWindow::flushUnsavedLocalProjects）。答不准会直接表现为数据丢失：
     * 少答 true 就是关闭时不提示、改动随退出消失。
     */
    virtual bool isProjectModified() const = 0;
};
