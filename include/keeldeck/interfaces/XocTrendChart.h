#pragma once

// ***************************************************************
//  XocTrendChart —— 插件侧趋势图外观（核心 DLL 里那份 QWT 的唯一出口）
//  -------------------------------------------------------------
//  This file is a part of project KeelDeck.
// ***************************************************************
//
// 为什么需要这一层：QWT 与 `libQTExt/xQwtChartWidget` 都编在
// keeldeck_core.dll 里，且都没有导出宏；而 DLL 插件按 #223 的 B 模式只许链 Qt 与
// 核心（tools/check_plugin_symbols.ps1 卡着）。插件要画趋势只剩两条路：
//
//   (a) 把 libQWT 的九十来个 .cpp 再编进插件——两份 QWT、两倍构建时间；
//   (b) 核心把画图能力以纯虚接口下发——与 IRpcChannel / IStormRouter 同一手法。
//
// 取 (b)。实现（framework/core/XocTrendChart.cpp）就是 xQwtChartWidget 的薄包装，
// 曲线/坐标轴/时间轴刻度这些 QWT 类型一律不出现在本头文件里：接口只用 Qt 基础类型，
// 枚举在这边自己定一份，核心侧再映射回 QwtPlotCurve::CurveStyle。
//
// 用法：
//   m_chart = xocCreateTrendChart(this);          // unique_ptr，析构即销毁部件
//   layout->addWidget(m_chart->widget());
//   const int id = m_chart->addCurve(tr("Predicted"), QColor(0x1f77b4));
//   m_chart->setCurveData(id, xs, ys);
//
// 线程：与普通 QWidget 一样，只能在 GUI 线程上用。

#include "XocExport.h"

#include <QColor>
#include <QPair>
#include <QString>
#include <QVector>
#include <Qt>
#include <functional>
#include <memory>

class QWidget;

/// 曲线画法。QWT 支持的样式远不止这三种，接口只暴露趋势页真正用得上的
enum class XocCurveStyle {
    Line = 0,  ///< 折线（默认）
    Dots,      ///< 只画点，用来叠加化验值这类离散样本
    Steps,     ///< 阶梯
};

/**
 * @brief 沿 X 轴着色的区间分组
 *
 * 辨识工程要把两类排除段画得能分开：用户手划的（可撤）与系统自动排除的（数据的
 * 物理事实，只读，例如 resume 补的合成常数点）。分组只决定颜色与图层，语义由调用方定。
 */
enum class XocShadedBand {
    UserExcluded = 0,  ///< 用户手划，可双击撤销
    AutoExcluded,      ///< 系统判定，界面上只展示不编辑
};

/**
 * @brief 趋势图部件的最小外观
 *
 * 曲线用整型 id 标识（addCurve 返回），与 xQwtChartWidget 的 id 语义一致：
 * removeAllCurves() 之后旧 id 全部失效，对失效 id 的调用是空操作而非崩溃。
 */
class IXocTrendChart {
  public:
    virtual ~IXocTrendChart() = default;

    /// 放进调用方布局里的部件；所有权跟着本对象走，不要单独 delete
    virtual QWidget* widget() = 0;

    // ---- 曲线 ----
    virtual int addCurve(const QString& title, const QColor& color) = 0;
    virtual void removeAllCurves() = 0;
    virtual void setCurveVisible(int curveId, bool visible) = 0;
    virtual void setCurveStyle(int curveId, XocCurveStyle style) = 0;
    virtual void setCurveWidth(int curveId, int pixel) = 0;
    /// x/y 等长；非有限值的处理与 xQwtChartWidget 一致（前导 NaN 跳过，中段截断）
    virtual void setCurveData(int curveId, const QVector<double>& x,
                              const QVector<double>& y) = 0;

    // ---- 坐标轴 ----
    virtual void setAxisTitles(const QString& xTitle, const QString& yTitle) = 0;
    /// Y 轴固定量程；min >= max 时按 xQwtChartWidget 的兜底展开，不会喂给 QWT 零宽量程
    virtual void setYAxisScale(double min, double max) = 0;
    virtual void setYAxisAutoScale() = 0;
    virtual void setXAxisScale(double min, double max) = 0;
    /**
     * @brief X 轴按"unix 秒 → 本地时刻"渲染刻度
     *
     * 趋势数据一律以 unix 秒（UTC 纪元）为 x 值存与传，只有刻度文字按本地时区显示。
     * 关掉则回到普通数值刻度。
     */
    virtual void setXAxisLocalTimeLabels(bool enable) = 0;

    // ---- 其它 ----
    virtual void setTitle(const QString& title) = 0;
    virtual void enableGrid(bool enable) = 0;
    virtual void enableLegend(bool enable) = 0;
    virtual void replot() = 0;

    // ---- 区间着色 ----
    //
    // 新增于 #294（辨识工程的切片交互），**只在类的末尾追加**。
    //
    // 即便如此也**必须**同步 +1 XOC_PLUGIN_ABI_VERSION（已改为 5）。末尾追加只保证了
    // "旧插件配新核心"那一侧安全：它们的虚表视图是新表的前缀，永远不会去调新槽。
    // 反方向是坏的——拿这份新头文件编出来的插件装进旧核心时，因为本次新增没有带来
    // 新的核心导出符号，LoadLibrary 不会失败，ABI 号一样也会被 PluginLoader 放行，
    // 等调到下面这些新槽才按新下标去取旧虚表，直接越界。比 #246 那次（缺符号、在
    // LoadLibrary 就响）更隐蔽。

    /**
     * @brief 设置某一组的着色区间（沿 X 轴的竖条），整组替换
     *
     * @param band   分组；两组各自独立，互不影响
     * @param ranges [x0, x1] 的列表，单位与曲线的 x 一致。顺序不限，允许重叠；
     *               x0 > x1 的会被当作 [x1, x0]。传空清掉这一组。
     */
    virtual void setShadedRanges(XocShadedBand band,
                                 const QVector<QPair<double, double>>& ranges) = 0;

    /// 分组的着色颜色（含 alpha）。不调用时用各组的缺省色。
    virtual void setShadedBandColor(XocShadedBand band, const QColor& color) = 0;

    // ---- 交互 ----

    /**
     * @brief 启用"按住修饰键 + 左键拖动"选一段 X 区间
     *
     * @param modifiers 需要按住的修饰键；传 Qt::NoModifier 关闭该功能（缺省即关闭，
     *                  不影响既有调用方）。中键平移与滚轮缩放不受影响。
     */
    virtual void setRangeSelectionModifier(Qt::KeyboardModifiers modifiers) = 0;

    /// 拖选完成的回调；x0 <= x1，单位与曲线的 x 一致。传空 std::function 取消回调。
    virtual void setRangeSelectedCallback(std::function<void(double x0, double x1)> callback) = 0;

    /// 画布上双击的回调（辨识工程用它撤销一段排除）。传空 std::function 取消回调。
    virtual void setDoubleClickedCallback(std::function<void(double x)> callback) = 0;
};

/**
 * @brief 建一个趋势图部件
 *
 * @param parent 部件父窗口。销毁归返回的 unique_ptr 管；若 Qt 的父子关系先把部件
 *               销毁了（父窗口先析构），本对象析构不会二次 delete——内部按弱引用
 *               持有，之后所有调用都是空操作。
 * @return 永不为空
 */
XOC_API std::unique_ptr<IXocTrendChart> xocCreateTrendChart(QWidget* parent);
