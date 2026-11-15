#pragma once

#include <QList>
#include <QString>
#include <QStringList>

/**
 * @brief 远程实例元数据（框架侧等价物，对应服务端的 zdp_base::zvm_t）
 *
 * 插件不允许看见 zdp_base::zvm_t：那个结构体来自 libzce 的 ptl 生成代码，字段是
 * zce_astring/zce_uint16 等 zce 类型，跟着 libzce 一起变。跨 DLL 传它等于把插件的
 * 二进制兼容性绑死在 libzce 的版本上。
 *
 * 框架核心在 InstanceInfoConvert.h（仅核心内部可见）里做 zvm_t ↔ XocInstanceInfo
 * 的双向转换，边界之外只流通本结构体。
 *
 * 字段与 zvm_t 一一对应，只做类型归一（zce_astring → QString，zce_uintN → quintN），
 * 不做语义加工——vmStatus 的位含义仍由各 vmtype 自己解释。
 */
struct XocNamedValue {
    QString name;
    QString value;

    bool operator==(const XocNamedValue& other) const noexcept {
        return name == other.name && value == other.value;
    }
};

struct XocInstanceInfo {
    QString vmType;      // vm 类型，等于 ProjectTypeInfo::typeId
    QString vmName;      // vm 可读名称，进程内唯一
    QString vmPath;      // vm 启动文件路径（上传组态时的远端文件名）
    QString workDir;     // vm 工作路径
    QString vmGuid;      // vm guid
    QString vmAddr;      // vm RPC 监听地址
    quint32 vmStatus{0}; // vm 标志位，含义由 vmtype 自定义
    quint16 vmPort{0};   // vm RPC 监听端口
    quint16 stormPort{0};// storm 端口
    QString stormTopic;  // storm 主题
    QString stormAddr;   // storm 地址
    QStringList args;
    QList<XocNamedValue> env;

    /// 能否据此发起 RPC：名字和端口缺一不可
    bool isConnectable() const noexcept { return !vmName.isEmpty() && vmPort != 0; }

    bool operator==(const XocInstanceInfo& other) const noexcept {
        return vmType == other.vmType && vmName == other.vmName && vmPath == other.vmPath &&
               workDir == other.workDir && vmGuid == other.vmGuid && vmAddr == other.vmAddr &&
               vmStatus == other.vmStatus && vmPort == other.vmPort &&
               stormPort == other.stormPort && stormTopic == other.stormTopic &&
               stormAddr == other.stormAddr && args == other.args && env == other.env;
    }

    bool operator!=(const XocInstanceInfo& other) const noexcept { return !(*this == other); }
};
