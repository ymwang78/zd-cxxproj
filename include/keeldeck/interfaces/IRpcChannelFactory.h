#pragma once

#include "IRpcChannel.h"
#include "XocInstanceInfo.h"

/**
 * @brief RPC 通道的来源（框架提供，插件只用不实现）
 *
 * 插件的 Runtime 层原来直接拿着 ProjectHost 去调 createRpcChannel()，于是"通道从哪来"
 * 与"核心的半个世界"绑死：没有后端时要么拿不到 host、要么拿到一条连不上的真通道，
 * 两种情况下 call() 都是同步立即失败，"回包晚于下一拍广播"这类时序在单测里根本构造
 * 不出来（见 issue #249 的清单）。
 *
 * 这个接口就是那条缝，与 StormBackend 同构：
 *   - 生产环境里 ProjectHost 实现它，行为和以前一模一样；
 *   - 测试里换成 xoc_test::FakeRpcChannelFactory（framework/sdk/XocFakeRpcChannel.h），
 *     响应可以按方法名编排、可以推迟到用例说了算的那一刻才投递。
 *
 * 顺带收窄了插件对 ProjectHost 的依赖：Runtime 层只需要"给我一条通道"，不需要整个宿主。
 */
class IRpcChannelFactory {
  public:
    virtual ~IRpcChannelFactory() = default;

    /**
     * @brief 为某个远程实例建一条 RPC 通道
     *
     * 通道所有权交给调用方；析构即关闭。需要第二条独立 TCP 时用返回通道的
     * createAuxChannel()。
     *
     * @return 失败（实例缺 vmName/vmPort，或宿主未配置地址）返回空指针
     */
    virtual IRpcChannelPtr createRpcChannel(const XocInstanceInfo& instance) = 0;
};
