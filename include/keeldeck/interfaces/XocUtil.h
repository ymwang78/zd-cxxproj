#pragma once

#include "XocExport.h"

#include <cstddef>
#include <string>
#include <vector>

/**
 * @brief 核心替插件挡住 libzce 的那几个工具函数（issue #264 / M3a）
 *
 * B 模式（issue #223）的硬约束是：插件 DLL 不链 libzce。整个进程只有
 * keeldeck_core.dll 里有一份 zce 状态，插件哪怕只想要个 bzip2 也不能自己链一份——
 * 链上去就会连带拉进 zce_any / zce_api / zce_log / zce_sync 那几个带全局状态的
 * 目标文件（实测），于是 Singleton / BlockPool / Reactor 在进程里出现第二份副本。
 *
 * TaijiMPC 按 M3b 变成插件之后，会有五处功能撞上这条线（都是 #262 边界闸门里
 * 标着"M3 的欠账"的那批链接期符号）：
 *
 *   bzip2 压缩/解压   —— .tmpc 容器里模型段的编码
 *   base64 编解码     —— 同一条链路，"taijimpc://model/..." 串
 *   ZOH 阶跃响应      —— 模型辨识对话框的曲线
 *   Guid15            —— 新建工程的 guid
 *
 * 三条路里选了"核心开缝"这一条，而不是"核心把 zce 符号再导出 + 放宽插件符号白名单"，
 * 也不是"插件自己重写一份"：
 *
 *   - 再导出的话，白名单要放行 zce_compress / zce_base64_* / Guid15 /
 *     zoh_step_response 等一串名字，每一条都要单独论证"无进程级状态"，而且
 *     zce/zce_api.h 是个大杂烩头（zce_init、reactor 全在里面），留在 TaijiMPC 的
 *     include 白名单上等于留一扇很宽的门；
 *   - 重写的话，bzip2 那段直接改 .tmpc 的**已有工程文件**的兼容性——那是真会丢数据的。
 *
 * 开缝两头都占：调用的还是 libzce 里那一份实现（字节级兼容，.tmpc 老文件照读照写），
 * 插件侧看到的却只是 xoc_ 前缀的核心导出符号，check_plugin_symbols.ps1 的 zce 过滤器
 * （zce|zdp|zds|zvm|zlog）根本扫不到它们，白名单一条都不用加。
 *
 * 判据仍是老那条：**只搬无进程级状态的东西**。这里四组全是纯函数或值语义——压缩/编码是
 * 缓冲区到缓冲区，阶跃响应是纯数值，Guid15 只吐一个新串。带 Reactor / Storm /
 * VirtualMachine / BlockPool 的东西一概不会出现在本文件里。
 */

/// 压缩算法。与 zce 的 ERV_ZCE_COMPRESS 一一对应，这里只暴露实际用到的两种。
enum class XocCompression {
    None = 0,
    Bzip2 = 1,
};

/**
 * @brief 压缩一段缓冲区
 *
 * @param algo      算法
 * @param out_buf   输出缓冲区，调用方自备且必须足够大
 * @param out_size  入参为 out_buf 的容量，出参为实际写入的字节数
 * @param in_buf    输入缓冲区
 * @param in_size   输入字节数
 * @return          >= 0 成功，< 0 失败（与 zce_compress 同语义，返回码原样透传）
 *
 * 编码结果与 zce_compress(ZCE_COMPRESS_BZIP2, ...) **逐字节一致**——本函数就是它的
 * 转发。.tmpc 是已有工程文件的容器格式，换实现就是换格式，所以这里刻意不重写。
 */
XOC_API int xocCompress(XocCompression algo, char* out_buf, size_t& out_size, const char* in_buf,
                        size_t in_size);

/// 解压，参数与返回值语义同 xocCompress（转发 zce_decompress）
XOC_API int xocDecompress(XocCompression algo, char* out_buf, size_t& out_size, const char* in_buf,
                          size_t in_size);

/**
 * @brief base64 编码；out_buf 由调用方自备，返回写入的字节数（< 0 失败）
 *
 * 与 zce_base64_encode 同一份实现。out_buf 的容量按 in_size * 4 / 3 + 4 估。
 */
XOC_API int xocBase64Encode(unsigned char* out_buf, const unsigned char* in_buf, size_t in_size);

/// base64 解码；返回写入的字节数（< 0 失败）。与 zce_base64_decode 同一份实现。
XOC_API int xocBase64Decode(unsigned char* out_buf, const unsigned char* in_buf, size_t in_size);

/**
 * @brief 连续传递函数的 ZOH 离散单位阶跃响应（含纯滞后，按最接近的整采样数处理）
 *
 * @param num        分子系数，降幂
 * @param den        分母系数，降幂
 * @param ts         采样周期（秒）
 * @param delay_sec  纯滞后（秒）
 * @param n          输出点数
 * @param t_out      各点时刻
 * @param y_out      各点响应
 *
 * 纯数值计算，无状态。转发 zce::zoh_step_response，结果与改动前逐点一致。
 * 参数非法（如分母首系数为 0）时抛 std::exception，与 zce 侧行为一致——
 * AddResetModelDialog 靠 catch 弹提示。
 */
XOC_API void xocZohStepResponse(const std::vector<double>& num, const std::vector<double>& den,
                                double ts, double delay_sec, int n, std::vector<double>& t_out,
                                std::vector<double>& y_out);

/**
 * @brief 生成一个新的 Guid15（15 字节、时间有序的短 guid 串）
 *
 * 转发 zce::Guid15(0).toString()。格式与改动前一致——工程文件里存量的 projectGuid
 * 是同一种串，不能换成别的生成器。
 *
 * 放在核心而不是插件各生成一份，还有个额外的好处：Guid15 的时间序列号是**进程级**的，
 * 只有一份实现才谈得上"同一进程内生成的 guid 不撞"。
 */
XOC_API std::string xocNewGuid15();
