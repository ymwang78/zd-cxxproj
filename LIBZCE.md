# libzce 库功能说明

libzce 是一个基于 libuv 的 C++ 网络和系统工具库，提供异步 I/O、日志、线程管理等功能。

## 安装位置

- **源码**: `/zdata/cxxproj/libsrc/libzce/`
- **静态库**: `/zdata/cxxproj/libsrc/libzce/build/libzce.a`
- **头文件**: `/zdata/cxxproj/include/zce/`
- **依赖**: libuv1-dev, libssl-dev, libbz2-dev, uuid-dev, zlib1g-dev

## 1. 网络与 I/O

### 核心类

**zce::Reactor** - 事件循环（基于 libuv）
- `onReactorStart()` / `onReactorStop()` - 生命周期钩子
- `scheduleTimer()` - 定时器调度
- `delegate()` - 提交任务到 reactor 线程
- `delegate_delay()` - 延迟任务执行

**zce::IStream** - 双向流基类
- `on_open()` / `on_read()` / `on_close()` - 生命周期回调
- `write()` - 发送数据（支持优先级）
- `link()` - 链接流（如 TCP → HttpStream）

**zce::HttpStream** - HTTP 协议处理器
- 解析 HTTP 请求/响应
- 头部规范化 `to_formula()` (Title-Case)
- URL 编码/解码工具

**zce::Tcp** - TCP 套接字封装
**zce::Connector** - 出站连接处理器
**zce::Acceptor** - 入站连接监听器（工厂模式）

### 架构模式

流链式架构：
1. `Acceptor` 创建 `Tcp` 套接字
2. `Tcp` 链接到协议处理器（如 `HttpStream`）
3. 通过 `link()` 链接 - 双向 `prev_`/`next_` SmartPtr
4. 所有 I/O 在 `Reactor` 线程中运行
5. 通过 `delegate()` 提交任务实现线程安全

## 2. 日志系统

### C API (zce_log.h)

```c
zlog_init()     // 初始化
zlog_setlevel() // 设置日志级别
ZLOG(level, fmt, ...) // 记录日志
```

**日志级别**: `ZLOG_TRACE`, `ZLOG_DEBUG`, `ZLOG_INFOR`, `ZLOG_WARNI`, `ZLOG_ERROR`, `ZLOG_FATAL`

### C++ API

```cpp
zce::Logger::write(level, args...)  // 流式日志
ZTRACE(), ZDEBUG(), ZINFOR(), ZWARNI(), ZERROR(), ZFATAL()
```

### 断言宏

```cpp
ZCE_ASSERT(condition)
ZCE_ASSERT_TEXT(condition, message)
ZCE_ASSERT_RETURN(condition, return_value)
```

## 3. 线程与并发

### 线程管理

**zce::Thread** - 工作线程基类
- `onThreadStart()` / `onThreadTerminate()` - 生命周期
- `startThread()` / `joinThread()` - 控制
- 优先级: `PRIORITY_HIGHEST` 到 `PRIORITY_LOWEST`

### 同步原语

- `zce::Mutex` - 互斥锁
- `zce::Semaphore` - 信号量
- `acquire()`, `release()`, `try_acquire()`

### 任务系统

- `zce::Task` - 异步工作单元
- `zce::TaskQueue` - 线程安全任务队列
- `zce::TaskResult<T>` - 结果包装器

## 4. 定时器

**zce::Timer** - 定时回调执行
- 单次或重复定时器
- 毫秒精度
- `start(callback)` - 基于 lambda 的执行
- `cancel()` - 停止定时器

## 5. 数据结构

### 容器类

- `zce::Array<T>` - 动态数组
- `zce::List<T>` - 双向链表
- `zce::Ring<T>` - 环形缓冲区
- `zce::Hash<K, V>` - 哈希表
- `zce::SafeMap<K, V>` - 线程安全映射

### 工具结构

- `zce::Any` - 类型擦除值持有者
- `zce::Optional<T>` - 可选值包装器
- `zce::SmartPtr<T>` - 引用计数智能指针

## 6. 序列化

### BSON (zce_bson.h)

```cpp
zce::BsonRef doc;
doc.append("key", value);  // 支持 int32, int64, double, bool, string
```

### ZDS (zds_schema_inc.h)

- 基于模式的二进制序列化
- 稀疏编码（零/默认字段省略）
- xTdb 使用 ZDS 进行高效数据打包

## 7. 字符串处理

**zce::string_view** - 非拥有字符串引用（C++17 兼容）
- `replace()` - 字符串替换
- 大小写转换、修剪、分割函数

## 8. 系统工具

### 对象管理

- `zce::Object` - 引用计数基类
- `zce::Singleton<T, Mutex>` - 线程安全单例模式

### 系统功能

- `zce::Process` - 进程管理
- `zce::Filesystem` - 文件 I/O 辅助
- `zce::CtrlCHandler` - 信号处理
- `zce::Random` - 随机数生成
- `zce::Guid` - UUID 生成

### 加密

- `zce::RSA` - RSA 加密/解密
- `zce::SHA1` - SHA-1 哈希
- `zce::Base64` - base64 编码/解码

## 9. 协议支持

- **HTTP** - `http_stream.h`
- **WebSocket** - `webs.h`, `whp_stream.h`
- **PTP** (专有传输协议) - `ptp_node.h`, `ptp_stream.h`
- **RTP** (实时传输协议) - `rtp.h`, `rtp_stream.h`
- **RTMP** (实时消息协议) - `rtmp_stream.h`

## 关键头文件参考

| 头文件 | 用途 |
|--------|------|
| `zce_reactor.h` | 事件循环与异步 I/O |
| `http_stream.h` | HTTP 协议处理 |
| `zce_log.h` | 日志系统 |
| `zce_thread.h` | 线程管理 |
| `zce_sync.h` | 同步原语 |
| `zce_timer.h` | 定时器调度 |
| `zce_task.h` | 异步任务执行 |
| `zce_handler.h` | 流基类 |
| `zce_string.h` | 字符串工具 |
| `zce_bson.h` | BSON 序列化 |
| `zce_array.h` | 动态数组 |
| `zce_hash.h` | 哈希表 |
| `zce_safemap.h` | 线程安全映射 |

## 使用建议

**优先使用 libzce 提供的功能，避免重复实现：**

1. **网络服务** - 使用 Reactor + Acceptor + HttpStream，不要自己实现事件循环
2. **日志** - 使用 ZLOG 宏，不要自己实现日志系统
3. **线程池** - 使用 zce::Task + TaskQueue，不要自己实现线程池
4. **定时器** - 使用 zce::Timer，不要用 sleep 循环
5. **数据结构** - 优先使用 zce::Array/Hash/SafeMap
6. **序列化** - 使用 BSON 或 ZDS，不要自己实现二进制协议

## CMake 集成示例

```cmake
target_include_directories(your_target PUBLIC
    ${CMAKE_SOURCE_DIR}/../include  # ZCE 头文件
)

target_link_libraries(your_target PUBLIC
    /zdata/cxxproj/libsrc/libzce/build/libzce.a
    uv      # libuv
    ssl     # OpenSSL
    crypto
    pthread
)
```
