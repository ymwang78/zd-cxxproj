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
| **Layer 2** | `mpcctrl`, `softsensorctrl`, `pbcctrl`, … | Service-level: operate one specific VM instance of that service type |

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

# Permanently delete samples from the testing history to free capacity.
# Without ranges: drop every slice that is currently deselected.
mpcctrl <instance> ident compact --yes

# With explicit zero-based half-open ranges (<start>:<length>), repeatable
mpcctrl <instance> ident compact 0:500 12000:3000 --yes
```

Compaction is irreversible, so `--yes` is mandatory; it is rejected while a test is
running. Deleting a range in the middle leaves the data on either side no longer
time-adjacent, so the server records a **segment boundary** there — it does not insert
NaN or placeholder samples to represent the gap. The gap length is whatever the real
timestamps on both sides say it is. Identification then treats the two sides as separate
batches: the first sample of each later segment is excluded from the fit so that no
regression row spans the seam. That sample is still stored and still displayed; it is a
real sample, just not fed to the fit.

The response carries `historyRevision`, which increments whenever existing sample indices
shift (compaction, or replacing the round). Clients must discard any index-based cache and
re-pull the whole round when it changes — incremental top-ups no longer line up.

---

### 5. `softsensorctrl` — SoftSensor Instance Control

`softsensorctrl` is the Layer 2 CTL tool for the `libsoftsensor` / `SoftSensor` service
(module `modules/softsensor`). It drives the online end of a FrontSensor soft sensor:
configuration, tag binding, the estimation loop, closed-loop lab calibration, prediction
history export, and the three Python script hooks.

> **Naming.** The §7 rule would spell it `softsensorctl`. This tool follows the migration
> plan and the existing `mpcctrl` instead — `softsensorctrl`. It is the one documented
> exception; new tools should follow §7.

#### 5.1 Synopsis

```
[HOSTVM=ip:port] softsensorctrl <instance> <noun> <verb> [args...] [flags...]
```

`<instance>` is the `vmname` from `hostvm.xml`. Instance ports live in **22610–22699**
(clear of the MPC range 22520–22599) and are discovered automatically via `listVM`.

#### 5.2 Noun Catalog

| Noun | Scope |
|------|-------|
| `project` | Project-level configuration: download/upload/reload/status, single-field edits |
| `sensor` | Soft sensors: list / add / del |
| `model` | `.ssmdl` model package import |
| `tag` | DCS tag bindings: set / verify / list |
| `run` | The periodic estimation loop: start / stop / status |
| `lab` | Closed-loop lab calibration: submit / history |
| `his` | Prediction history export |
| `script` | The three Python hooks: get / set / exec |
| `opc` | OPC server enumeration |

#### 5.3 Project Commands

```bash
# Summary, or the full config file with --file
softsensorctrl ss1 project download [--file p.zssm]
softsensorctrl ss1 project upload --file p.zssm     # whole-config replace
softsensorctrl ss1 project reload                   # re-read from disk
softsensorctrl ss1 project status                   # instance / runtime state

# Single-field edits. The trailing index is the sensor index for sensor-scoped fields.
softsensorctrl ss1 project set sample-period 60
softsensorctrl ss1 project set correction-coef 0.5 0
softsensorctrl ss1 project str datasource-uri "opc.da://10.0.0.5/OPC.Server.1"
```

Field names are listed by `softsensorctrl --help`.

#### 5.4 Sensor, Model and Tag Commands

```bash
softsensorctrl ss1 sensor list
softsensorctrl ss1 sensor add "TC-101" .        # "." = append
softsensorctrl ss1 sensor del 2
softsensorctrl ss1 model import 0 --file TC101.ssmdl

# tag set <scope> <sensor-idx> <var-idx> <tag> [rw] [type]
#   scope: input | output | lab-value | lab-time | lab-ack | custom
#   rw:    local | read | write | rdwrt     (default read)
#   type:  long | double | bool             (default double)
softsensorctrl ss1 tag set input  0 0 TI101 read
softsensorctrl ss1 tag set output 0 0 AI_TC101 write
softsensorctrl ss1 tag set lab-ack 0 0 LAB_ACK rdwrt    # must be writable

softsensorctrl ss1 tag verify                   # read every bound tag once
softsensorctrl ss1 tag list
```

`lab-time` takes `<var-idx>` 0..5 for the year/month/day/hour/minute/second tags.

#### 5.5 Run and Lab Commands

```bash
softsensorctrl ss1 run start
softsensorctrl ss1 run status
softsensorctrl ss1 run stop                     # runtime state is flushed to disk

# <time>: omitted or '.' = now, digits = UTC unix seconds,
#         "YYYY-MM-DD HH:MM:SS" = LOCAL wall clock (same as the DCS lab-time tags)
softsensorctrl ss1 lab submit 0 128.5 "2026-08-27 13:45:30"
softsensorctrl ss1 lab history 0
```

#### 5.6 History Export

```bash
# By global index: start='.' means oldest, count=0 means all
softsensorctrl ss1 his export 0 [<start>] [<count>]

# By TIME RANGE (either form, not both); '.' on one side leaves it open
softsensorctrl ss1 his export 0 --from "2026-08-27 08:00:00" --to .                                 --local-time --file new.csv
```

CSV columns: `index,timestamp_utc[,timestamp_local],predicted,y_model,bias,calc_status`.
Timestamps are **UTC unix seconds**; `--local-time` only *adds* a wall-clock column, so
a mis-converted timezone is visible on the spot.

#### 5.7 Script and OPC Commands

```bash
softsensorctrl ss1 script get [<kind>] [--file p.py]   # kind: init|input|output
softsensorctrl ss1 script set <kind> --file p.py       # rejected while estimating
softsensorctrl ss1 script exec <kind>                  # dry-run on a snapshot
softsensorctrl ss1 opc servers 10.0.0.5
```

`script exec` runs on a snapshot: no DCS write, no runtime change. It prints stdout and,
on failure, the traceback, exiting `3`.

#### 5.8 RPC Method Mapping

| Command | RPC |
|---------|-----|
| `project download` | `downloadProject(Config\|Script)` |
| `project upload` | `uploadProjectConfig` |
| `project reload` | `reloadProjectConfig` |
| `project status` / `run status` / `lab history` | `runtimeStatus` |
| `project set` / `project str` | `setProjectReal` / `setProjectString` |
| `sensor list` / `tag list` | `downloadProject(Config)` |
| `sensor add` / `sensor del` | `addDelSensor` |
| `model import` | `importModelPackage` |
| `tag set` | `setTagLink` |
| `tag verify` | `verifyTags` |
| `run start` / `run stop` | `startEstimating` / `stopEstimating` |
| `his export` | `downloadHisDataRange` (chunked; `--from/--to` filters client-side) |
| `lab submit` | `submitLabSample` |
| `script get` | `downloadProject(Script)` |
| `script set` / `script exec` | `setScript` / `executeScript` |
| `opc servers` | `enumOpcServers` |

#### 5.9 Companion Tool: `softsensor_paracmp`

Shipped alongside `softsensorctrl`, but **not** a CTL tool — it never connects to a
HostVM. It compares two prediction series (VB6 legacy vs. the new system) offline:

```bash
softsensor_paracmp vb.csv new.csv --ref-time "时间" --ref-value "预估值"                                   --tol 0.05 --align-tol 30 --warmup 25
```

Its exit codes are therefore **different** from the CTL set (§8): `0` within tolerance,
`1` usage/file error, `2` points outside tolerance, `3` nothing comparable lined up.
Full usage in `modules/softsensor/manual/PARALLEL_RUN.md`.

#### 5.10 Config Edits Are Rejected While Estimating

`project upload/set/str`, `sensor add/del`, `model import`, `tag set/verify` and
`script set` all return `BUSY` while the estimation loop is running — stop it first.
The only exceptions are `project set plot-upper|plot-lower`, which the loop never reads.
`script get` / `script exec` stay available (one is read-only, the other runs on a snapshot).

---

### 6. Implementing a New CTL Tool

When adding a CTL tool for a new library (`libxxx` → `xxxctrl`):

#### 6.1 File Layout

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

#### 6.2 Implementation Skeleton

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

#### 6.3 Command Handler Pattern

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

#### 6.4 Output Helper Conventions

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

### 7. Naming Conventions for CTL Tools

| Concern | Convention | Example |
|---------|-----------|---------|
| Tool binary name | `<libname>ctl` (drop "lib" prefix) | `mpcctrl`, `pbcctrl` |
| Noun names | lowercase, no hyphens | `mv`, `cv`, `model`, `project` |
| Verb names | lowercase, no hyphens | `list`, `add`, `set`, `del`, `start`, `stop` |
| Flag names | `--kebab-case` | `--dry-run`, `--follow` |
| Env variable | `HOSTVM` (shared across all tools) | `HOSTVM=10.0.0.1:22500` |

---

### 8. Error Handling Rules

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
| **第二层** | `mpcctrl`、`softsensorctrl`、`pbcctrl` 等 | 服务层：对该服务类型的某个具体 VM 实例进行操作 |

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

# 物理删除测试历史里的采样点，释放点数配额。
# 不带区间：删掉当前生效的全部"取消选择"切片。
mpcctrl <实例> ident compact --yes

# 显式给区间（零基半开 <起点>:<长度>），可重复
mpcctrl <实例> ident compact 0:500 12000:3000 --yes
```

压缩不可逆，所以 `--yes` 是必填；辨识测试运行中会被拒绝。删掉中段之后，两侧数据不再时间
相邻，服务端在接缝处记一条**段边界**（见 `TestingHisData.segmentStarts`），而**不是**插
NaN 或占位点来表示间隔——隔了多久由两侧真实时间戳说了算。辨识随后把两侧当作互不相干的两批
数据：每个后续段的首点被排除出拟合，保证没有任何一行回归跨过接缝。这个点照常存储、照常显示，
它是真实采样点，只是不进拟合。

应答里的 `historyRevision` 在既有采样点下标发生平移时（压缩、整轮换数据）递增。客户端一旦
发现它变了，必须丢弃所有按下标缓存的历史并重拉整轮——增量补拉的下标已经对不上账。

---

### 5. `softsensorctrl` — SoftSensor 实例控制

`softsensorctrl` 是 `libsoftsensor` / `SoftSensor` 服务（模块 `modules/softsensor`）的
第二层 CTL 工具。它管的是 FrontSensor 软测量的**在线端**：组态、位号绑定、周期估计、
化验闭环校正、预测历史导出，以及三个 Python 脚本挂点。

> **命名。** 按 §7 的通则应该叫 `softsensorctl`。这个工具跟随迁移计划与既有的
> `mpcctrl`，用 **`softsensorctrl`**。这是唯一一个成文的例外，新工具请按 §7 命名。

#### 5.1 语法

```
[HOSTVM=ip:port] softsensorctrl <实例名> <名词> <动词> [参数...] [选项...]
```

`<实例名>` 就是 `hostvm.xml` 里的 `vmname`。实例端口在 **22610–22699**（避开 MPC 的
22520–22599），由 `listVM` 自动发现，不用手填。

#### 5.2 名词目录

| 名词 | 职责 |
|------|------|
| `project` | 工程级组态：下载/上传/重载/状态、单字段编辑 |
| `sensor` | 软测量：列表 / 新增 / 删除 |
| `model` | `.ssmdl` 模型包导入 |
| `tag` | DCS 位号绑定：设置 / 校验 / 列表 |
| `run` | 周期估计：启动 / 停止 / 状态 |
| `lab` | 化验闭环校正：人工录入 / 校正历史 |
| `his` | 预测历史导出 |
| `script` | 三个 Python 挂点：读 / 写 / 试运行 |
| `opc` | OPC 服务器枚举 |

#### 5.3 工程命令

```bash
# 不带 --file 打摘要，带 --file 写出与服务端落盘同格式的完整组态
softsensorctrl ss1 project download [--file p.zssm]
softsensorctrl ss1 project upload --file p.zssm     # 整份替换
softsensorctrl ss1 project reload                   # 从磁盘重新载入
softsensorctrl ss1 project status                   # 实例 / 运行态状态

# 单字段编辑。末尾那个下标是软测量下标（仅软测量级字段需要）
softsensorctrl ss1 project set sample-period 60
softsensorctrl ss1 project set correction-coef 0.5 0
softsensorctrl ss1 project str datasource-uri "opc.da://10.0.0.5/OPC.Server.1"
```

字段名清单见 `softsensorctrl --help`。

#### 5.4 软测量、模型与位号命令

```bash
softsensorctrl ss1 sensor list
softsensorctrl ss1 sensor add "TC-101" .        # "." = 追加到末尾
softsensorctrl ss1 sensor del 2
softsensorctrl ss1 model import 0 --file TC101.ssmdl

# tag set <作用域> <软测量下标> <变量下标> <位号> [读写] [类型]
#   作用域: input | output | lab-value | lab-time | lab-ack | custom
#   读写:   local | read | write | rdwrt     (默认 read)
#   类型:   long | double | bool             (默认 double)
softsensorctrl ss1 tag set input  0 0 TI101 read
softsensorctrl ss1 tag set output 0 0 AI_TC101 write
softsensorctrl ss1 tag set lab-ack 0 0 LAB_ACK rdwrt    # 必须可写: 结算完要写回 0

softsensorctrl ss1 tag verify                   # 每个绑定的位号读一次，报坏的
softsensorctrl ss1 tag list
```

`lab-time` 的 `<变量下标>` 是 0..5，对应 年/月/日/时/分/秒 六个位号。

#### 5.5 运行与化验命令

```bash
softsensorctrl ss1 run start
softsensorctrl ss1 run status
softsensorctrl ss1 run stop                     # 运行态落盘

# <时刻>: 省略或 '.' = 现在；纯数字 = UTC unix 秒；
#         "YYYY-MM-DD HH:MM:SS" = **本地墙钟**（与 DCS 那 6 个化验时刻位号同一口径）
softsensorctrl ss1 lab submit 0 128.5 "2026-08-27 13:45:30"
softsensorctrl ss1 lab history 0
```

#### 5.6 历史导出

```bash
# 按全局序号: start 写 '.' 表示从最早那一点开始，count 写 0 表示到最新
softsensorctrl ss1 his export 0 [<start>] [<count>]

# 按**时间段**（与上面二选一，不能同时给）；一侧写 '.' 表示这一头不限
softsensorctrl ss1 his export 0 --from "2026-08-27 08:00:00" --to .                                 --local-time --file new.csv
```

CSV 列：`index,timestamp_utc[,timestamp_local],predicted,y_model,bias,calc_status`。
时间戳一律是 **UTC unix 秒**；`--local-time` 只是**追加**一列本地墙钟，于是时区换算
错了当场看得出来。

#### 5.7 脚本与 OPC 命令

```bash
softsensorctrl ss1 script get [<段>] [--file p.py]   # 段: init|input|output
softsensorctrl ss1 script set <段> --file p.py       # 估计运行中会被拒
softsensorctrl ss1 script exec <段>                  # 在快照上试运行
softsensorctrl ss1 opc servers 10.0.0.5
```

`script exec` 跑在**快照**上：不写 DCS、不改运行态。它把 stdout 打出来，失败时打完整
traceback 并以退出码 `3` 结束。

#### 5.8 RPC 方法映射

| 命令 | RPC |
|------|-----|
| `project download` | `downloadProject(Config\|Script)` |
| `project upload` | `uploadProjectConfig` |
| `project reload` | `reloadProjectConfig` |
| `project status` / `run status` / `lab history` | `runtimeStatus` |
| `project set` / `project str` | `setProjectReal` / `setProjectString` |
| `sensor list` / `tag list` | `downloadProject(Config)` |
| `sensor add` / `sensor del` | `addDelSensor` |
| `model import` | `importModelPackage` |
| `tag set` | `setTagLink` |
| `tag verify` | `verifyTags` |
| `run start` / `run stop` | `startEstimating` / `stopEstimating` |
| `his export` | `downloadHisDataRange`（分块；`--from/--to` 在**客户端**筛） |
| `lab submit` | `submitLabSample` |
| `script get` | `downloadProject(Script)` |
| `script set` / `script exec` | `setScript` / `executeScript` |
| `opc servers` | `enumOpcServers` |

#### 5.9 配套工具：`softsensor_paracmp`

与 `softsensorctrl` 一起随包，但**不是** CTL 工具——它一次都不连 HostVM。它离线比对
两条预测序列（VB6 存量系统 vs 新系统）：

```bash
softsensor_paracmp vb.csv new.csv --ref-time "时间" --ref-value "预估值"                                   --tol 0.05 --align-tol 30 --warmup 25
```

所以它的退出码与 CTL 那一套（§8）**刻意不同**：`0` 在容差内，`1` 用法/文件错误，
`2` 有超容差点位，`3` 对齐不上（没有可比的点）。完整用法见
`modules/softsensor/manual/PARALLEL_RUN.md`。

#### 5.10 估计运行中，组态类命令一律被拒

`project upload/set/str`、`sensor add/del`、`model import`、`tag set/verify` 与
`script set` 在周期估计运行时全部回 `BUSY`——先 `run stop`。唯一的例外是
`project set plot-upper|plot-lower`（周期核从不读它们）。`script get` / `script exec`
运行中照常可用（一个只读，一个在快照上跑）。

---

### 6. 新增 CTL 工具的实现规范

为新库（`libxxx` → `xxxctrl`）新增 CTL 工具时遵循以下规范：

#### 6.1 目录结构

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

#### 6.2 命令分发框架

```cpp
// main.cpp 结构
// 1. 解析全局参数（--host、--timeout、--json、-v）
//    argv[1]=实例名, argv[2]=名词, argv[3]=动词
// 2. 初始化 ZCE Reactor（单线程，一次性执行）
// 3. 用 stub->boot() 创建指向远程实例的代理 VM
// 4. 分发至对应命令处理器
// 5. 运行 Reactor 直到 RPC 响应返回
```

#### 6.3 命令处理器模式

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

### 7. 命名规范

| 关注点 | 规范 | 示例 |
|--------|------|------|
| 工具二进制名 | `<库名>ctl`（去掉 "lib" 前缀） | `mpcctrl`、`pbcctrl` |
| 名词 | 全小写，无连字符 | `mv`、`cv`、`model`、`project` |
| 动词 | 全小写，无连字符 | `list`、`add`、`set`、`del`、`start`、`stop` |
| 选项 | `--kebab-case` | `--dry-run`、`--follow` |
| 环境变量 | `HOSTVM`（所有工具共用） | `HOSTVM=10.0.0.1:22500` |

---

### 8. 错误处理规则

1. **连接错误** → 退出码 2，stderr 输出 `"error: cannot connect to <host>"`
2. **RPC 非零返回** → 退出码 3，stderr 输出 `"error: <method> returned <code>: <description>"`
3. **超时** → 退出码 4，stderr 输出 `"error: timeout waiting for <method>"`
4. **参数错误** → 退出码 1，打印用法说明及 `"error: <description>"`
5. 不在 stdout 打印堆栈信息；所有错误输出均使用 stderr
6. `--json` 模式：错误信息以纯文本输出到 stderr；stdout 始终输出合法 JSON 或保持为空

---

### 9. RPC 方法映射参考（mpcctrl）

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
| `ident online` | `onlineIdent` | `(bool estimate_delay=false, bool use_expectation=true, int timeout_ms, bool export_file=false)` — the two leading bools exist only for wire compatibility with older HostVM; current HostVM reads `delayCalculationMethod` / `idTestingFlag` from the project. `--estimate-delay` / `--no-expectation` therefore only affect legacy servers |
| `ident delay` | `estDelayon` | `(int timeout_ms)` |
| `ident design` | `testDesign` | `(int timeout_ms)` |
| `ident cancel` | `identCancel` | `(int64 request_id=0)` — omit the argument to cancel whatever task is in flight (`request_id <= 0`) |
| `ident import` | `importTestingHisData` | `(ImportTestingHisDataRequest req)` — CSV parsed client-side, uploaded in chunks |
| `ident export` | `downloadProject` | `(unsigned mask)` — mask = Config\|TestingHisData; CSV written client-side |
