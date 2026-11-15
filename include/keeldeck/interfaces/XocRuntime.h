#pragma once

#include "XocExport.h"

namespace xoc {

/**
 * @brief 整个应用的启动入口，实现在核心 DLL 里
 *
 * 做完这些事才返回：装消息处理器 → 建 QApplication → 读 branding.ini → 应用字体与
 * 语言 → 起 zce 运行时（Reactor / Scheduler / VirtualMachineStub）→ 扫 plugins 目录
 * 装 DLL 插件 → 开主窗口 → 跑事件循环 → 反序停掉 zce。
 *
 * 为什么整段都在 DLL 里、exe 只剩一行 main：
 *   libzce 被静态链进核心 DLL 且不再对外暴露。exe 若自己调 zce_init / ReactorSigt，
 *   就得再静态链一份 libzce，于是 ReactorSigt / BlockPoolSigt 这些 zce::Singleton
 *   在进程里出现两份副本——RPC 走没初始化的那份 stub，内存块跨模块 acquire/release
 *   落在不同的池上直接损坏堆。让 exe 完全不碰 zce 是唯一稳的边界。
 *
 * @return 进程退出码
 */
XOC_API int runApplication(int argc, char** argv);

}  // namespace xoc
