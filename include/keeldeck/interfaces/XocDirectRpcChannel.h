#pragma once

// ***************************************************************
//  xocCreateDirectRpcChannelFactory —— 不经 ProjectHost 的通道来源
//  -------------------------------------------------------------
//  This file is a part of project KeelDeck.
// ***************************************************************
//
// 本地工程（host-less，见 ILocalProject.h）也要能开一条 RPC 通道：离线辨识工程要
// 连 IDENT。但它没有 ProjectHost，而插件自己也造不出通道——生产用的 VmRpcChannel
// 在 framework/core 里且没有导出宏，插件又不链 libzce。
//
// 于是由核心提供这个薄工厂：只认主机名，端口与服务名跟着 XocInstanceInfo 走，
// 与 ProjectHost::createRpcChannel 的分工完全一致。
//
// **不要拿 ProjectHost 当连接工厂**：那是 Solution 里的宿主对象（Storm 路由、实例
// 列表、方案持久化），辨识工程只需要一条 TCP，用它太重，也容易把本地工程误挂进
// 远程树（设计文档 identification-offline-signed-dataset.md §7.2）。

#include "IRpcChannelFactory.h"
#include "XocExport.h"

#include <QString>
#include <memory>

/**
 * @brief 建一个只认主机名的 IRpcChannelFactory
 *
 * @param serviceHost 目标主机名或 IP。**不含端口**——端口与服务名由调用方通过
 *                    XocInstanceInfo{vmName, vmPort} 给出，每次 createRpcChannel
 *                    都可以不同。空串会让 createRpcChannel 恒返回空指针。
 * @return 永不为空；所有权归调用方
 */
XOC_API std::unique_ptr<IRpcChannelFactory> xocCreateDirectRpcChannelFactory(
    const QString& serviceHost);
