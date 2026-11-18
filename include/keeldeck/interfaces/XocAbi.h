#pragma once

/**
 * @brief 插件二进制接口（ABI）版本
 *
 * 插件 DLL 在编译期把这个常量烧进 IXocPlugin::abiVersion() 的返回值，核心加载时拿
 * 自己的常量去比。不相等就拒载——插件是拿旧头文件编的，虚表布局、结构体大小、
 * 回调签名都可能已经不一样，硬装进来是随机踩内存，不是"少一个功能"。
 *
 * 什么时候必须 +1（不完全枚举）：
 * - framework/interfaces/ 里任一纯虚接口增删/改虚函数、或改虚函数顺序；
 * - XocInstanceInfo 等跨界结构体增删字段、改字段类型或顺序；
 * - IXocPlugin 自身的签名变化。
 *
 * 纯粹给插件用的 header-only 辅助（XocRpcCallback.h 之类）改动不影响 ABI——那些代码
 * 编进插件自己的二进制里，不跨界。
 *
 * 注意：Qt 版本、debug/release 这两类不兼容由 QPluginLoader 自己拦（插件元数据里
 * 有 QT_VERSION 与 debug 标志），这里不重复检查。
 */
// 变更史：
// - 1：#223 落地时的初版。
// - 2：#246 增加 XocTrendChart.h（IXocTrendChart + xocCreateTrendChart）。纯新增，
//      老插件的虚表没动；但拿新头文件编出来的插件要求核心导出 xocCreateTrendChart，
//      装进老核心会在 LoadLibrary 阶段就解不出符号。版本号跟着 interfaces 的面走，
//      不去区分"加"和"改"。
// - 3：#249 增加 IRpcChannelFactory.h，ProjectHost 多继承它。虚基类一加，ProjectHost
//      的对象布局与 createRpcChannel 的调用方式都变了；插件的 Runtime 层也改成持有
//      IRpcChannelFactory*。老插件拿旧头文件编出来的 ProjectHost* 偏移对不上。
// - 4：#258 IProject 增加 onVerifyRequested()（"校验当前工程"改由核心直接调当前工程，
//      插件不再 connect MainWindow 的信号），虚表尾部多一项——老插件的 IProject 虚表
//      短一格，核心按新表下标调用就是越界。同期 XocHost.h 增加 xocColorizedIcon /
//      xocRefreshHostUi / xocMarkSolutionModified，XocDiagnostics.h 增加带描述的
//      xocFormatRpcError 重载：拿新头文件编的插件要求核心导出这些符号。
// - 5：#294 IXocTrendChart 末尾追加区间着色与拖选/双击回调共 5 个纯虚函数（辨识工程
//      的切片交互）。这一次必须 +1 的理由与 #246 同源但更隐蔽：新增没有带来新的核心
//      导出符号，所以拿新头文件编的插件装进旧核心时 LoadLibrary **不会**失败，ABI 号
//      又一样、PluginLoader 照样放行，等到调 setShadedRanges 这些新槽才按新下标去取
//      旧虚表——越界调用，崩在哪儿都跟真因无关。"核心实现、插件调用的接口末尾追加是
//      安全的"只对"旧插件配新核心"那一侧成立，反过来不成立。
// - 6：#298 新增 XocDirectRpcChannel.h（xocCreateDirectRpcChannelFactory）。与 #246
//      同一条理由：拿新头文件编出来的插件要求核心导出这个函数，装进老核心会在
//      LoadLibrary 阶段就解不出符号。既有虚表一格没动，老插件配新核心照常。
// 7: IRemoteProject adds requiresInstanceEndpoint for host-managed projects.
// 8: IProject adds canRenameProject() and renameProject() virtual slots (issue #462).
// 9: XocHost.h adds xocMarkWorkspaceModified() and xocPluginDisplayName(); ProjectFactory adds
//    translateTypeName() and translateTypeDescription().
#define XOC_PLUGIN_ABI_VERSION 9

/**
 * @brief IXocPlugin 的 Qt 接口标识
 *
 * Q_DECLARE_INTERFACE / Q_PLUGIN_METADATA 用它做类型标识。IID 里的版本号跟着大版本
 * 走即可，细粒度兼容性由 XOC_PLUGIN_ABI_VERSION 负责。
 */
#define XOC_PLUGIN_IID "com.KeelDeck.IXocPlugin/1.0"
