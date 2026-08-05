# CTL CLI Framework Guide — Two-Layer CLI Design

> **Language / 语言**: This document is bilingual. English section first, Chinese section follows.
> See CLAUDE.md index for the entry point.

---

## English

### Overview

All `*ctl` command-line tools in this project follow a **two-layer CLI design**:

| Layer | Tool | Scope |
|-------|------|-------|
| **Layer 1** | `hostvmctl` | Host-level: enumerate and manage all VM instances running on a remote host |
| **Layer 2** | `mpcctrl`, `pbcctrl`, … | Service-level: operate one specific VM instance of that service type |

Both layers communicate via the **ZCE ZVM RPC** framework (`zce::zvm::VirtualMachineStub`). The host is addressed through the `HOSTVM` environment variable or the `--host` flag.

---

### 1. Global Conventions

#### 1.1 Host Address

```
HOSTVM=<ip>:<port>          # Environment variable (preferred)
--host <ip>:<port>           # CLI flag (overrides env)
```

- Default port when omitted: **22500**
- IPv6: `HOSTVM=[::1]:22500`
- Local pipe (same machine, no network): `HOSTVM=pipe://zvm-main`

#### 1.2 Exit Codes

| Code | Meaning |
|------|---------|
| 0 | Success |
| 1 | General / usage error |
| 2 | Connection failed |
| 3 | RPC call returned a non-zero error code |
| 4 | Timeout |

#### 1.3 Output Modes

| Flag | Meaning |
|------|---------|
| *(default)* | Human-readable table / text |
| `--json` | JSON to stdout, errors to stderr |
| `--quiet` / `-q` | Suppress all output except errors |
| `-v` / `--verbose` | Show raw RPC method name and arguments |

#### 1.4 Shared Flags (every tool, every command)

```
--host <ip:port>      Host address (overrides HOSTVM env)
--timeout <ms>        RPC timeout in milliseconds (default: 5000)
--json                Machine-readable JSON output
-q, --quiet           Suppress normal output
-v, --verbose         Show RPC-level detail
--help                Show help
--version             Print tool version
```

---

### 2. Layer 1 — `hostvmctl`

`hostvmctl` connects to the ZVM host service and manages VM instances at the process level.

#### 2.1 Synopsis

```
hostvmctl [--host ip:port] <command> [args...]
```

#### 2.2 Commands

| Command | Arguments | Description |
|---------|-----------|-------------|
| `list` | `[--type <vmtype>]` | List all running VM instances |
| `status <name>` | | Show status of a specific VM |
| `boot <vmtype> <name>` | `[--path <dir>] [-- arg1 arg2...]` | Boot a new VM instance |
| `stop <name>` | `[--force]` | Gracefully stop (or force-kill) a VM |
| `restart <name>` | | Stop then boot with the same parameters |
| `logs <name>` | `[-n <lines>] [--follow]` | Tail the VM's log output |

#### 2.3 Examples

```bash
# List all VMs on default host
HOSTVM=192.168.1.100:22500 hostvmctl list

# List only MPC-type VMs
HOSTVM=192.168.1.100:22500 hostvmctl list --type TaijiMPC

# Boot an MPC instance named "project1"
HOSTVM=192.168.1.100:22500 hostvmctl boot TaijiMPC project1 --path /data/mpc/project1

# Stop instance "project1" gracefully
HOSTVM=192.168.1.100:22500 hostvmctl stop project1

# Follow logs in real time
HOSTVM=192.168.1.100:22500 hostvmctl logs project1 --follow
```

#### 2.4 `list` Output Format

```
NAME          TYPE        STATUS      ADDR              UPTIME
project1      TaijiMPC    running     192.168.1.100:22501   3h 42m
project2      TaijiMPC    stopped     -                 -
ident_svc     ZIdent      running     192.168.1.100:22502   10m 5s
```

---

### 3. Layer 2 — Service-Specific CTL Tools

#### 3.1 General Synopsis

```
[HOSTVM=ip:port] <toolname> <instance> <noun> <verb> [positional-args...] [flags...]
```

- **`<instance>`**: VM instance name or ID (as shown in `hostvmctl list`)
- **`<noun>`**: The resource category (variable group, model, project, …)
- **`<verb>`**: The action (get, set, add, del, list, start, stop, …)

#### 3.2 Standard Noun-Verb Matrix

All service-specific CTL tools MUST support these verbs for applicable nouns:

| Verb | Meaning | HTTP analogy |
|------|---------|--------------|
| `list` | Enumerate items | GET (collection) |
| `get <id>` | Fetch one item | GET (single) |
| `add <id> [args]` | Create / append item | POST |
| `set <id> <field> <value>` | Update field(s) | PATCH |
| `del <id>` | Remove item | DELETE |
| `swap <id1> <id2>` | Swap two items | PATCH |
| `clear` | Remove all items of this noun | DELETE (all) |
| `start` | Begin an operation (testing, controlling, …) | POST /start |
| `stop` | End an operation | POST /stop |
| `status` | Query current state | GET /status |
| `exec <name>` | Execute a named script or task | POST /exec |

---

### 4. `mpcctrl` — MPC Instance Control

`mpcctrl` is the Layer 2 CTL tool for the `libmpc` / `TaijiMPC` service.

#### 4.1 Synopsis

```
[HOSTVM=ip:port] mpcctrl <instance> <noun> <verb> [args...] [flags...]
```

#### 4.2 Noun Catalog

| Noun | Full Name | Tag Type byte |
|------|-----------|---------------|
| `mv` | Manipulated Variable (操纵变量) | 0 |
| `cv` | Controlled Variable (被控变量) | 1 |
| `dv` | Disturbance Variable (扰动变量) | 2 |
| `tv` | Tracking Variable (跟踪变量) | 3 |
| `sv` | Status Variable (状态变量) | 4 |
| `model` | Control model matrix | — |
| `project` | Project-level configuration | — |
| `test` | Testing session | — |
| `control` | Control loop | — |
| `script` | Embedded scripts | — |
| `ident` | System identification | — |

#### 4.3 Variable Commands (`mv`, `cv`, `dv`, `tv`, `sv`)

```bash
# List all MVs
mpcctrl <instance> mv list

# Add a new MV at position 0
mpcctrl <instance> mv add MV1 0
# Add at end (append): use "." as position
mpcctrl <instance> mv add MV1 .

# Remove MV at index 2
mpcctrl <instance> mv del 2

# Set numeric value of MV index 1 (during testing)
mpcctrl <instance> mv set 1 value 3.14

# Set tag name of MV index 1
mpcctrl <instance> mv set 1 name "PV101.SP"

# Swap MV index 0 and index 2
mpcctrl <instance> mv swap 0 2

# Clear all MVs
mpcctrl <instance> mv clear
```

**`add` positional-args:**
```
mpcctrl <instance> <noun> add <tag-name> <position>
    <tag-name>   string   tag identifier
    <position>   int      0-based insertion index; "." means append
```

**`set` positional-args:**
```
mpcctrl <instance> <noun> set <index> <field> <value>
    <index>   int     0-based tag index
    <field>   string  "value" | "name" | "sp" | "hi" | "lo" | …
    <value>   varies  numeric or string
```

#### 4.4 Model Commands

```bash
# Show all models (as matrix summary)
mpcctrl <instance> model list

# Get single model at row 0, col 1
mpcctrl <instance> model get 0 1

# Set model value at (row=0, col=1, step=5) to 0.42
mpcctrl <instance> model set 0 1 5 0.42

# Push new model matrix (from JSON file)
mpcctrl <instance> model upload --file model.json

# Start auto-tuning
mpcctrl <instance> model autotune start

# Apply auto-tuned values
mpcctrl <instance> model autotune apply
```

#### 4.5 Project Commands

```bash
# Show project configuration summary
mpcctrl <instance> project status

# Download full project config (to file or stdout)
mpcctrl <instance> project download [--file config.json]

# Upload (replace) full project config
mpcctrl <instance> project upload --file config.json

# Reload project from disk (hot reload)
mpcctrl <instance> project reload

# Set a numeric project parameter (by enum index or name)
mpcctrl <instance> project set SampleTime 2.0

# Set a string project parameter
mpcctrl <instance> project set Description "Reactor loop"
```

#### 4.6 Test Session Commands

```bash
# Start testing
mpcctrl <instance> test start [--duration <seconds>]

# Stop testing
mpcctrl <instance> test stop

# Query testing status
mpcctrl <instance> test status
```

#### 4.7 Control Loop Commands

```bash
# Start MPC control
mpcctrl <instance> control start

# Start simulation mode
mpcctrl <instance> control start --simulate

# Stop control
mpcctrl <instance> control stop

# Query control status
mpcctrl <instance> control status
```

#### 4.8 Script Commands

```bash
# List scripts
mpcctrl <instance> script list

# Show script content
mpcctrl <instance> script get <script-name>

# Upload / update script from file
mpcctrl <instance> script set <script-name> --file my_script.lua

# Execute a script
mpcctrl <instance> script exec <script-name>

# Validate without executing
mpcctrl <instance> script exec <script-name> --dry-run
```

#### 4.9 Identification Commands

```bash
# Run online identification (timeout 60 s)
mpcctrl <instance> ident online --timeout 60000

# Ask the Python identification algorithm to export files (ignored by older HostVM)
mpcctrl <instance> ident online --export-file --timeout 60000

# Estimate delays
mpcctrl <instance> ident delay --timeout 30000

# Design test
mpcctrl <instance> ident design --timeout 120000

# Cancel the running identification task (optionally only the given requestId)
mpcctrl <instance> ident cancel
mpcctrl <instance> ident cancel 42

# Import identification data from CSV (header: DateTime,TAG1,TAG2,...).
# Replaces the current testing-data round; rejected while a test is running.
# --period is required when the CSV has no DateTime column.
mpcctrl <instance> ident import --file data.csv --period 60

# Append a further chunk to the round started by the import above
mpcctrl <instance> ident import --file more.csv --append

# Map CSV column names onto project tag names
mpcctrl <instance> ident import --file data.csv --map "FI-101=FIC101" --map "TI-102=TI102"

# Tune the per-request chunk size (default 20000 points); the first chunk overwrites,
# the rest are appended
mpcctrl <instance> ident import --file big.csv --chunk 50000

# Export testing/identification data as CSV (optionally only the last N points)
mpcctrl <instance> ident export --file dump.csv --max-points 10000
```

---

### 5. Implementing a New CTL Tool

When adding a CTL tool for a new library (`libxxx` → `xxxctrl`):

#### 5.1 File Layout

```
xxxctrl/
├── CMakeLists.txt
├── src/
│   ├── main.cpp              # main(): arg parsing, reactor startup, dispatch
│   ├── xxxctrl_client.h      # XxxCtlClient: wraps VirtualMachineStub
│   ├── xxxctrl_client.cpp
│   ├── cmd_<noun1>.h/cpp     # One file pair per noun
│   ├── cmd_<noun2>.h/cpp
│   └── ctl_output.h          # Shared output helpers (table, JSON)
```

#### 5.2 Implementation Skeleton

```cpp
// main.cpp
int main(int argc, char** argv) {
    // 1. Parse global flags (--host, --timeout, --json, -v)
    //    Then dispatch: argv[1] = instance, argv[2] = noun, argv[3] = verb
    std::string host = resolveHost(argc, argv);   // HOSTVM env or --host
    std::string instance = argv[1];
    std::string noun     = argv[2];
    std::string verb     = argv[3];

    // 2. Set up ZCE reactor (single-threaded, one-shot)
    auto reactor = zce::makeReactor();
    auto stub    = zce::SmartPtr<zce::zvm::VirtualMachineStub>(new zce::zvm::VirtualMachineStub());
    stub->initStub(scheduler, reactor, "");

    // 3. Boot a proxy VM targeting the remote instance
    auto vm_proxy = stub->boot(instance, host_ip, host_port, false, timeout_ms, {}, {});

    // 4. Dispatch to command handler
    int exit_code = dispatchCommand(stub, vm_proxy, noun, verb, remaining_args);

    // 5. Run reactor until the RPC response arrives
    reactor->run();
    return exit_code;
}
```

```cpp
// xxxctrl_client.h  — thin wrapper for type-safe RPC calls
class XxxCtlClient {
    zce::zvm::VirtualMachineStubPtr stub_;
    zce::SmartPtr<zce::Object>      proxy_;
    int                             timeout_ms_;
public:
    // Generic call — returns synchronously (blocks reactor until response)
    template <typename Req, typename Resp>
    int call(const std::string& method, const Req& req, Resp& out);

    // Zero-arg call
    template <typename Resp>
    int call0(const std::string& method, Resp& out);
};
```

#### 5.3 Command Handler Pattern

```cpp
// cmd_mv.cpp
int cmdMv(XxxCtlClient& client, const std::string& verb,
          const std::vector<std::string>& args) {
    if (verb == "list") {
        return cmdMvList(client);
    } else if (verb == "add") {
        return cmdMvAdd(client, args);   // args: [name, position]
    } else if (verb == "set") {
        return cmdMvSet(client, args);   // args: [index, field, value]
    } else if (verb == "del") {
        return cmdMvDel(client, args);   // args: [index]
    } else if (verb == "swap") {
        return cmdMvSwap(client, args);  // args: [index1, index2]
    } else if (verb == "clear") {
        return cmdMvClear(client);
    } else {
        return usageError("mv", verb);
    }
}
```

#### 5.4 Output Helper Conventions

```cpp
// ctl_output.h
namespace ctl_output {
    // Print one row to a growing table; call flush() at the end
    void tableRow(const std::vector<std::string>& cells);
    void tableFlush();

    // Print success message (suppressed with --quiet)
    void ok(const std::string& msg);

    // Print error to stderr; returns exit code 3
    int rpcError(const std::string& method, int error_code);

    // JSON output (when --json flag set)
    void jsonValue(const nlohmann::json& j);
}
```

---

### 6. Naming Conventions for CTL Tools

| Concern | Convention | Example |
|---------|-----------|---------|
| Tool binary name | `<libname>ctl` (drop "lib" prefix) | `mpcctrl`, `pbcctrl` |
| Noun names | lowercase, no hyphens | `mv`, `cv`, `model`, `project` |
| Verb names | lowercase, no hyphens | `list`, `add`, `set`, `del`, `start`, `stop` |
| Flag names | `--kebab-case` | `--dry-run`, `--follow` |
| Env variable | `HOSTVM` (shared across all tools) | `HOSTVM=10.0.0.1:22500` |

---

### 7. Error Handling Rules

1. **Connection errors** → exit code 2, print `"error: cannot connect to <host>"` to stderr.
2. **RPC non-zero** → exit code 3, print `"error: <method> returned <code>: <description>"`.
3. **Timeout** → exit code 4, print `"error: timeout waiting for <method>"`.
4. **Bad arguments** → exit code 1, print usage and `"error: <description>"`.
5. Never print stack traces to stdout; use stderr for all error output.
6. `--json` flag: errors go to stderr as plain text; stdout always contains valid JSON or is empty.

---

---

## 中文

### 概述

本项目中所有 `*ctl` 命令行工具遵循**两层 CLI 设计**：

| 层级 | 工具 | 职责 |
|------|------|------|
| **第一层** | `hostvmctl` | 主机层：枚举和管理远程主机上运行的所有 VM 实例 |
| **第二层** | `mpcctrl`、`pbcctrl` 等 | 服务层：对该服务类型的某个具体 VM 实例进行操作 |

两层均通过 **ZCE ZVM RPC** 框架（`zce::zvm::VirtualMachineStub`）通信。主机地址通过环境变量 `HOSTVM` 或 `--host` 参数指定。

---

### 1. 全局约定

#### 1.1 主机地址

```
HOSTVM=<ip>:<port>          # 环境变量（推荐方式）
--host <ip>:<port>           # CLI 参数（覆盖环境变量）
```

- 省略端口时默认使用 **22500**
- IPv6：`HOSTVM=[::1]:22500`
- 本机命名管道（无网络）：`HOSTVM=pipe://zvm-main`

#### 1.2 退出码

| 码 | 含义 |
|----|------|
| 0 | 成功 |
| 1 | 通用/用法错误 |
| 2 | 连接失败 |
| 3 | RPC 调用返回非零错误码 |
| 4 | 超时 |

#### 1.3 输出模式

| 参数 | 含义 |
|------|------|
| *(默认)* | 人类可读的表格/文本 |
| `--json` | JSON 输出到 stdout，错误输出到 stderr |
| `--quiet` / `-q` | 抑制所有输出（仅报错） |
| `-v` / `--verbose` | 显示 RPC 方法名及原始参数 |

#### 1.4 所有工具通用参数

```
--host <ip:port>      主机地址（覆盖 HOSTVM 环境变量）
--timeout <ms>        RPC 超时时间，单位毫秒（默认 5000）
--json                机器可读的 JSON 输出
-q, --quiet           抑制正常输出
-v, --verbose         显示 RPC 层级详情
--help                显示帮助
--version             打印工具版本
```

---

### 2. 第一层 — `hostvmctl`

`hostvmctl` 连接 ZVM 宿主服务，在进程级别管理 VM 实例。

#### 2.1 语法

```
hostvmctl [--host ip:port] <command> [args...]
```

#### 2.2 命令列表

| 命令 | 参数 | 说明 |
|------|------|------|
| `list` | `[--type <vmtype>]` | 列出所有运行中的 VM 实例 |
| `status <name>` | | 查看指定 VM 的状态 |
| `boot <vmtype> <name>` | `[--path <dir>] [-- arg1 arg2...]` | 启动一个新 VM 实例 |
| `stop <name>` | `[--force]` | 优雅停止（或强制终止）VM |
| `restart <name>` | | 停止后以相同参数重新启动 |
| `logs <name>` | `[-n <lines>] [--follow]` | 追踪 VM 日志输出 |

#### 2.3 示例

```bash
# 列出默认主机上的所有 VM
HOSTVM=192.168.1.100:22500 hostvmctl list

# 只列出 MPC 类型的 VM
HOSTVM=192.168.1.100:22500 hostvmctl list --type TaijiMPC

# 启动名为 "project1" 的 MPC 实例
HOSTVM=192.168.1.100:22500 hostvmctl boot TaijiMPC project1 --path /data/mpc/project1

# 优雅停止 "project1"
HOSTVM=192.168.1.100:22500 hostvmctl stop project1

# 实时跟踪日志
HOSTVM=192.168.1.100:22500 hostvmctl logs project1 --follow
```

---

### 3. 第二层 — 服务专属 CTL 工具

#### 3.1 通用语法

```
[HOSTVM=ip:port] <工具名> <实例> <名词> <动词> [位置参数...] [选项...]
```

- **`<实例>`**：VM 实例名称或 ID（见 `hostvmctl list` 输出）
- **`<名词>`**：资源分类（变量组、模型、项目等）
- **`<动词>`**：操作类型（get、set、add、del、list、start、stop 等）

#### 3.2 标准名词-动词矩阵

所有服务专属 CTL 工具对适用的名词**必须**支持以下动词：

| 动词 | 含义 | HTTP 类比 |
|------|------|-----------|
| `list` | 枚举列表 | GET（集合） |
| `get <id>` | 获取单项 | GET（单项） |
| `add <id> [args]` | 创建/追加 | POST |
| `set <id> <field> <value>` | 更新字段 | PATCH |
| `del <id>` | 删除 | DELETE |
| `swap <id1> <id2>` | 交换两项 | PATCH |
| `clear` | 清除该名词下所有项 | DELETE（全部） |
| `start` | 开始操作（测试、控制等） | POST /start |
| `stop` | 结束操作 | POST /stop |
| `status` | 查询当前状态 | GET /status |
| `exec <name>` | 执行脚本或任务 | POST /exec |

---

### 4. `mpcctrl` — MPC 实例控制

`mpcctrl` 是 `libmpc` / `TaijiMPC` 服务的第二层 CTL 工具。

#### 4.1 语法

```
[HOSTVM=ip:port] mpcctrl <实例> <名词> <动词> [参数...] [选项...]
```

#### 4.2 名词目录

| 名词 | 全称 | Tag Type 字节值 |
|------|------|----------------|
| `mv` | 操纵变量（Manipulated Variable） | 0 |
| `cv` | 被控变量（Controlled Variable） | 1 |
| `dv` | 扰动变量（Disturbance Variable） | 2 |
| `tv` | 跟踪变量（Tracking Variable） | 3 |
| `sv` | 状态变量（Status Variable） | 4 |
| `model` | 控制模型矩阵 | — |
| `project` | 项目级配置 | — |
| `test` | 测试会话 | — |
| `control` | 控制回路 | — |
| `script` | 内嵌脚本 | — |
| `ident` | 系统辨识 | — |

#### 4.3 变量命令（`mv`、`cv`、`dv`、`tv`、`sv`）

```bash
# 列出所有操纵变量
mpcctrl <实例> mv list

# 在位置 0 新增一个 MV
mpcctrl <实例> mv add MV1 0
# 追加到末尾：位置用 "." 表示
mpcctrl <实例> mv add MV1 .

# 删除索引为 2 的 MV
mpcctrl <实例> mv del 2

# 设置 MV 索引 1 的数值（测试期间）
mpcctrl <实例> mv set 1 value 3.14

# 设置 MV 索引 1 的标签名
mpcctrl <实例> mv set 1 name "PV101.SP"

# 交换 MV 索引 0 和索引 2
mpcctrl <实例> mv swap 0 2

# 清除所有 MV
mpcctrl <实例> mv clear
```

**`add` 位置参数说明：**
```
mpcctrl <实例> <名词> add <标签名> <位置>
    <标签名>  string  标签标识符
    <位置>    int     从 0 开始的插入索引；"." 表示追加
```

**`set` 位置参数说明：**
```
mpcctrl <实例> <名词> set <索引> <字段> <值>
    <索引>  int     从 0 开始的标签索引
    <字段>  string  "value" | "name" | "sp" | "hi" | "lo" | …
    <值>    变量    数值或字符串
```

#### 4.4 模型命令

```bash
# 显示所有模型（矩阵摘要）
mpcctrl <实例> model list

# 获取第 0 行第 1 列的单个模型
mpcctrl <实例> model get 0 1

# 设置 (行=0, 列=1, 步=5) 处的模型值为 0.42
mpcctrl <实例> model set 0 1 5 0.42

# 从 JSON 文件上传新模型矩阵
mpcctrl <实例> model upload --file model.json

# 启动自动整定
mpcctrl <实例> model autotune start

# 应用自动整定结果
mpcctrl <实例> model autotune apply
```

#### 4.5 项目命令

```bash
# 显示项目配置摘要
mpcctrl <实例> project status

# 下载完整项目配置
mpcctrl <实例> project download [--file config.json]

# 上传（替换）完整项目配置
mpcctrl <实例> project upload --file config.json

# 从磁盘热重载项目
mpcctrl <实例> project reload

# 设置数值型项目参数
mpcctrl <实例> project set SampleTime 2.0

# 设置字符串型项目参数
mpcctrl <实例> project set Description "Reactor loop"
```

#### 4.6 测试会话命令

```bash
# 开始测试
mpcctrl <实例> test start [--duration <秒>]

# 停止测试
mpcctrl <实例> test stop

# 查询测试状态
mpcctrl <实例> test status
```

#### 4.7 控制回路命令

```bash
# 启动 MPC 控制
mpcctrl <实例> control start

# 启动仿真模式
mpcctrl <实例> control start --simulate

# 停止控制
mpcctrl <实例> control stop

# 查询控制状态
mpcctrl <实例> control status
```

#### 4.8 脚本命令

```bash
# 列出脚本
mpcctrl <实例> script list

# 显示脚本内容
mpcctrl <实例> script get <脚本名>

# 从文件上传/更新脚本
mpcctrl <实例> script set <脚本名> --file my_script.lua

# 执行脚本
mpcctrl <实例> script exec <脚本名>

# 仅验证不执行
mpcctrl <实例> script exec <脚本名> --dry-run
```

#### 4.9 辨识命令

```bash
# 在线辨识（超时 60 秒）
mpcctrl <实例> ident online --timeout 60000

# 让 Python 辨识算法导出文件（旧 HostVM 会忽略该尾参）
mpcctrl <实例> ident online --export-file --timeout 60000

# 估算延迟
mpcctrl <实例> ident delay --timeout 30000

# 设计测试
mpcctrl <实例> ident design --timeout 120000

# 取消在途的辨识任务（可只取消指定的 requestId）
mpcctrl <实例> ident cancel
mpcctrl <实例> ident cancel 42

# 从 CSV 导入辨识数据（表头 DateTime,位号1,位号2,...）。
# 覆盖当前的试验数据轮次；辨识测试运行中会被拒绝。
# CSV 没有 DateTime 列时必须给 --period。
mpcctrl <实例> ident import --file data.csv --period 60

# 向上面开始的那一轮追加后续数据块
mpcctrl <实例> ident import --file more.csv --append

# CSV 列名与工程位号不一致时手工映射
mpcctrl <实例> ident import --file data.csv --map "FI-101=FIC101" --map "TI-102=TI102"

# 调整单块上传的点数(默认 20000); 首块 overwrite, 其余转 append
mpcctrl <实例> ident import --file big.csv --chunk 50000

# 导出试验/辨识数据为 CSV（可只导出最后 N 个点）
mpcctrl <实例> ident export --file dump.csv --max-points 10000
```

---

### 5. 新增 CTL 工具的实现规范

为新库（`libxxx` → `xxxctrl`）新增 CTL 工具时遵循以下规范：

#### 5.1 目录结构

```
xxxctrl/
├── CMakeLists.txt
├── src/
│   ├── main.cpp              # main()：参数解析、Reactor 启动、命令分发
│   ├── xxxctrl_client.h      # XxxCtlClient：封装 VirtualMachineStub
│   ├── xxxctrl_client.cpp
│   ├── cmd_<名词1>.h/cpp     # 每个名词一对文件
│   ├── cmd_<名词2>.h/cpp
│   └── ctl_output.h          # 共享输出工具（表格、JSON）
```

#### 5.2 命令分发框架

```cpp
// main.cpp 结构
// 1. 解析全局参数（--host、--timeout、--json、-v）
//    argv[1]=实例名, argv[2]=名词, argv[3]=动词
// 2. 初始化 ZCE Reactor（单线程，一次性执行）
// 3. 用 stub->boot() 创建指向远程实例的代理 VM
// 4. 分发至对应命令处理器
// 5. 运行 Reactor 直到 RPC 响应返回
```

#### 5.3 命令处理器模式

```cpp
// cmd_mv.cpp — 按名词分文件，内部按动词 if-else 分发
int cmdMv(XxxCtlClient& client, const std::string& verb,
          const std::vector<std::string>& args) {
    if      (verb == "list")  return cmdMvList(client);
    else if (verb == "add")   return cmdMvAdd(client, args);
    else if (verb == "set")   return cmdMvSet(client, args);
    else if (verb == "del")   return cmdMvDel(client, args);
    else if (verb == "swap")  return cmdMvSwap(client, args);
    else if (verb == "clear") return cmdMvClear(client);
    else                      return usageError("mv", verb);
}
```

---

### 6. 命名规范

| 关注点 | 规范 | 示例 |
|--------|------|------|
| 工具二进制名 | `<库名>ctl`（去掉 "lib" 前缀） | `mpcctrl`、`pbcctrl` |
| 名词 | 全小写，无连字符 | `mv`、`cv`、`model`、`project` |
| 动词 | 全小写，无连字符 | `list`、`add`、`set`、`del`、`start`、`stop` |
| 选项 | `--kebab-case` | `--dry-run`、`--follow` |
| 环境变量 | `HOSTVM`（所有工具共用） | `HOSTVM=10.0.0.1:22500` |

---

### 7. 错误处理规则

1. **连接错误** → 退出码 2，stderr 输出 `"error: cannot connect to <host>"`
2. **RPC 非零返回** → 退出码 3，stderr 输出 `"error: <method> returned <code>: <description>"`
3. **超时** → 退出码 4，stderr 输出 `"error: timeout waiting for <method>"`
4. **参数错误** → 退出码 1，打印用法说明及 `"error: <description>"`
5. 不在 stdout 打印堆栈信息；所有错误输出均使用 stderr
6. `--json` 模式：错误信息以纯文本输出到 stderr；stdout 始终输出合法 JSON 或保持为空

---

### 8. RPC 方法映射参考（mpcctrl）

CTL 命令到 ZVM RPC 方法的完整映射：

| CTL 命令 | RPC 方法 | 参数类型 |
|---------|---------|---------|
| `mv add` | `addDelTag` | `(byte=0, int pos, bool=true, string name)` |
| `mv del` | `addDelTag` | `(byte=0, int pos, bool=false, string="")` |
| `mv set value` | `setValue` | `(bool=false, byte=0, int idx, int field, double val)` |
| `mv set name` | `setString` | `(bool=false, byte=0, int idx, int field, string val)` |
| `mv swap` | `swapTag` | `(byte=0, int idx1, int idx2)` |
| `mv clear` | `clearTag` | `(byte=0)` |
| `model set` | `setModelValue` | `(int row, int col, int step, int type, double val)` |
| `model upload` | `updateModels` | `(int flags, IDModelMatrix matrix)` |
| `model autotune start` | `startAutoTuning` | `(bool=true)` |
| `model autotune apply` | `useAutoTuningValue` | `(bool=true)` |
| `project download` | `downloadProject` | `(unsigned mask)` — CLI passes Config\|Model\|SimulationModel\|Script\|Runtime |
| `project upload` | `uploadProjectConfig` | `(ProjectFull cfg)` |
| `project reload` | `reloadProjectConfig` | *(无参数)* |
| `project set <real>` | `setProjectReal` | `(int enum_idx, double val)` |
| `project set <str>` | `setProjectString` | `(int enum_idx, string val)` |
| `test start` | `startTesting` | `(StartTestingRequest req)` |
| `test stop` | `stopTesting` | `(StopTestingRequest req)` |
| `control start` | `startControlling` | `(bool=false, StartControllingRequest req)` |
| `control start --simulate` | `startControlling` | `(bool=true, StartControllingRequest req)` |
| `control stop` | `stopControlling` | `(bool=false, StopControllingRequest req)` |
| `script set` | `setScript` | `(string name, string content)` |
| `script exec` | `executeScript` | `(bool=false, string name)` |
| `script exec --dry-run` | `executeScript` | `(bool=true, string name)` |
| `ident online` | `onlineIdent` | `(bool estimate_delay=false, bool use_expectation=true, int timeout_ms, bool export_file)` — the two leading bools exist only for wire compatibility with older HostVM; current HostVM reads `delayCalculationMethod` / `idTestingFlag` from the project. `--estimate-delay` / `--no-expectation` therefore only affect legacy servers |
| `ident delay` | `estDelayon` | `(int timeout_ms)` |
| `ident design` | `testDesign` | `(int timeout_ms)` |
| `ident cancel` | `identCancel` | `(int64 request_id=0)` — omit the argument to cancel whatever task is in flight (`request_id <= 0`) |
| `ident import` | `importTestingHisData` | `(ImportTestingHisDataRequest req)` — CSV parsed client-side, uploaded in chunks |
| `ident export` | `downloadProject` | `(unsigned mask)` — mask = Config\|TestingHisData; CSV written client-side |
