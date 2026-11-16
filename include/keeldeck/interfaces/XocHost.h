#pragma once

#include "XocExport.h"
#include <QIcon>
#include <QSize>
#include <QString>

class QWidget;

/**
 * @brief 插件与主程序外壳之间的最小接触面
 *
 * 插件不能 include MainWindow.h（它带 zce 类型，没有 XOC_API 标注，而且主窗口不属于
 * 插件 ABI），但弹个对话框总得有父窗口、干完一件事总得往状态栏写一句、工具栏的图标
 * 总得跟着主题走。这里就是全部。
 */

/// 主窗口部件，用作插件对话框的 parent。主窗口尚未建立时返回 nullptr（照样能弹，只是无父窗口）
XOC_API QWidget* xocMainWindowWidget();

/// 往主窗口状态栏写一行；非 GUI 线程调用会自动投递
XOC_API void xocSetStatusText(const QString& text);

/**
 * @brief 按当前主题的前景色给 SVG 上色，返回可直接用的图标
 *
 * 与框架自己的菜单/工具栏图标同一条路径（MainWindow::createColorizedIcon），
 * 换主题时插件图标跟着一起变。主窗口尚未建立时返回空 QIcon。
 */
XOC_API QIcon xocColorizedIcon(const QString& svgPath, const QSize& size = QSize(24, 24));

/**
 * @brief 刷新主窗口的动作可用性（菜单/工具栏 enable 状态）
 *
 * 插件改了会影响动作状态的东西（例如某个视图的 UI 状态被持久化）之后调用。
 * 主窗口尚未建立时什么都不做。
 */
XOC_API void xocRefreshHostUi();

/**
 * @brief 把当前 Solution 标记为已修改，并刷新动作状态与窗口标题
 *
 * 视图级 UI 设置属于 Solution 级编辑：Host/Solution 的信号通常已经刷过一遍动作状态，
 * 这里补最后一手，覆盖"动作在第一次 dirty 之后才重新绑定"以及"Solution 早就是 dirty"
 * 这两种漏刷的情形。主窗口或当前 Solution 不存在时只做能做的部分。
 */
XOC_API void xocMarkSolutionModified();
