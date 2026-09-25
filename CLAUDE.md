# C++ Projects Structure

This directory contains all C++ projects with a shared structure for common dependencies and individual project directories. The canonical workspace policy entry is [docs/.ai/cxx-docs-index.md](docs/.ai/cxx-docs-index.md); more specific project `AGENTS.md` guidance controls project conventions.

## Project Structure

### Files
- `.clang-format`: C++ code formatting standards
- `.gitignore`: Git ignore rules
- `.gitmodules`: Git submodule configuration
- `build.sh`: Linux build script for all projects
- `README.md`: Directory documentation
- `vcpkg_requirements.bat`: Windows vcpkg dependency packages

### Directories
- `include/`: Common header files shared across projects
- `lib/`: Common library files
- `libsrc/`: Common library source code
- Individual project directories (each with their own git repository)

## File Encoding

**Mandatory**: All C/C++ header files (`.h`, `.hpp`) and source files (`.c`, `.cpp`) MUST be saved in **UTF-8 with BOM** encoding.

## Coding Standards

Based on Google C++ Style Guide with modifications:
- **Class names**: PascalCase (大驼峰) - `MyClass`
- **Function names**: camelCase (小驼峰) - `myFunction()`
- **Variable names**: snake_case - `my_variable`
- **Class member variables**: snake_case with trailing underscore - `member_variable_`

## Testing Standards

The canonical rules live in [docs/.ai/cxx-testing-policy.md](docs/.ai/cxx-testing-policy.md); a project `AGENTS.md` may override them for that project. The summary below is what day-to-day work needs. If it ever disagrees with the policy, the policy wins: update the policy first, then this summary.

- **Framework**: C++ tests use Google Test by default. Where a project `AGENTS.md` names another framework (e.g. xOptCon uses Qt Test), use that one; the GTest rules below then do not apply.
- **Location**: `tests/` or `gtest/` (libzce uses `gtest/`). Put new tests in the directory the project already uses.
- **Files**: `test_*.cpp`, one component or feature per file.
- **Cases**: `TEST_F()` for fixture-based tests, `TEST()` for simple ones. Name the suite after the component and the case after the behavior and expected outcome, PascalCase without underscores: `TEST_F(HttpRequestLimitsFixture, ContentLengthOverCeilingIsRefusedWith413)`.
- **Custom `main()`**: a test file that defines `main()` must guard it, so the same file also builds against `gtest_main`:
```cpp
#ifndef USE_GTEST_MAIN
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
```
- **CMake**: link `GTest::gtest` / `GTest::gtest_main` when available and register each executable with `add_test()`.
- Add or update tests whenever behavior changes.

## Common Commands

### Build
```bash
# Build all projects on Linux
./build.sh
```

### Development
```bash
# Format code according to project standards
clang-format -i <file>

# Check git status
git status

# Update submodules
git submodule update --init --recursive
```

### Project Management
- Each project directory typically has its own git repository
- Common dependencies are shared through the `include/`, `lib/`, and `libsrc/` directories
- Use the centralized build script for consistent compilation across projects
## CTL CLI Framework (Two-Layer CLI Design)

**IMPORTANT**: All `*ctl` command-line tools follow a unified two-layer CLI design.

### Key Rule
When building a CTL tool for any library (`libxxx` → `xxxctrl`), follow the conventions in `CTL_CLI_GUIDE.md` exactly. Do **not** invent ad-hoc argument structures.

### Documentation
See `CTL_CLI_GUIDE.md` for the complete guide (bilingual EN/ZH), including:
- Layer 1 (`hostvmctl`): host-level VM enumeration and lifecycle management
- Layer 2 (`xxxctrl`): instance-level service control, noun/verb command matrix
- `mpcctrl` full command reference and RPC method mapping
- Implementation skeleton for new CTL tools

### Quick Reference
- Host address: `HOSTVM=ip:port` env var (default port 22500) or `--host ip:port`
- Command structure: `<toolname> <instance> <noun> <verb> [args...] [--flags]`
- Common verbs: `list`, `get`, `add`, `set`, `del`, `swap`, `clear`, `start`, `stop`, `status`, `exec`
- Tool binary naming: `<libname>ctl` (drop "lib" prefix) — e.g. `mpcctrl`, `pbcctrl`
- Exit codes: 0=ok, 1=usage, 2=conn-failed, 3=rpc-error, 4=timeout
- Output flags: `--json` for machine-readable, `-q` quiet, `-v` verbose

## RPC Protocol Definition (ZDL / zGen)

**IMPORTANT**: When defining remote communication protocols (messages, requests, responses, data structures for network transmission), use the **ZDL (Zce Definition Language)** with the **zGen** code generator.

### Key Rule
**Always use ZDL `.ptl` files** to define protocol data structures instead of writing C++ structs by hand. The `zgen` tool generates the C++ types and ZDS serialization code automatically.

### Documentation
See `ZDL_PROTOCOL.md` for the complete syntax reference, type mappings, code generation commands, and naming conventions.

### Quick Reference
- Protocol files: `*.ptl` (UTF-8 with BOM encoding)
- Code generator: `/zdata/cxxproj/bin/linux_x86_64/zgen`
- Generated outputs: `*_proto.h` (types) + `*_pack.h`/`*_pack.cpp` (serialization)
- Serialization API: `zce::zdp::zds_pack()` / `zce::zdp::zds_unpack()`
- Variable-length arrays: `type field[~];` → `std::vector<T>`
- Optional fields: `[o] type field;`

## libzce Library Usage

**IMPORTANT**: Before implementing networking, logging, threading, or common utilities, check if libzce already provides the functionality.

### Key Rule
**Always prefer libzce over custom implementations** for:
- Network services (Reactor, Acceptor, HttpStream)
- Logging (ZLOG macros)
- Thread management (zce::Thread, zce::Task)
- Timers (zce::Timer, not sleep loops)
- Data structures (zce::Array, zce::Hash, zce::SafeMap)
- Serialization (BSON, ZDS)

### Documentation
See `LIBZCE.md` for complete API reference and usage examples.

### CMake Integration
```cmake
target_include_directories(your_target PUBLIC
    ${CMAKE_SOURCE_DIR}/../include
)
target_link_libraries(your_target PUBLIC
    /zdata/cxxproj/libsrc/libzce/build/libzce.a
)
```

## Database Naming Conventions

### Table Names
- Use **plural + snake_case**: `users`, `order_items`, `sensor_readings`
- Avoid abbreviations unless widely accepted (e.g., `configs`, not `cfg`)
- Junction/association tables: combine both entity names — `user_roles`, `product_categories`

### Column Names
- Use **snake_case** with clear, self-documenting semantics
- Include **units** in the name when applicable:
  - `temperature_celsius`, `pressure_kpa`, `duration_seconds`, `file_size_bytes`
- Booleans: use `is_` or `has_` prefix — `is_active`, `has_children`
- Avoid generic names like `value`, `data`, `info`; prefer `retry_count`, `response_body`

### Foreign Keys
- Format: **`<referenced_table_singular>_id`**
- Examples: `user_id`, `order_id`, `sensor_id`, `parent_node_id`
- Always reference the primary key of the parent table

### Timestamps
- Creation time: `created_at` (preferred) or `create_time`
- Last update time: `updated_at` (preferred) or `update_time`
- Specific event times: `<event>_time` — `deleted_time`, `published_time`, `expired_time`
- All timestamps stored in **UTC**; use `TIMESTAMP` or `DATETIME` type

### Indexes
- Format: **`idx_<table>_<column(s)>`**
- Examples:
  - `idx_users_email`
  - `idx_order_items_order_id`
  - `idx_sensor_readings_sensor_id_created_at` (composite)
- Primary key index name: `pk_<table>` — `pk_users`

### Constraints
- **Unique constraints**: `uk_<table>_<column(s)>` — `uk_users_email`, `uk_products_sku`
- **Foreign key constraints**: `fk_<table>_<referenced_table>` — `fk_orders_users`, `fk_order_items_orders`
- **Check constraints**: `chk_<table>_<column>` — `chk_products_price_positive`
- **Not-null/default**: enforced at the column definition level, no special naming required

### Examples

```sql
CREATE TABLE order_items (
    id              BIGINT       PRIMARY KEY,           -- pk_order_items
    order_id        BIGINT       NOT NULL,              -- fk_order_items_orders
    product_id      BIGINT       NOT NULL,              -- fk_order_items_products
    quantity        INT          NOT NULL,
    unit_price_cny  DECIMAL(12,2) NOT NULL,
    is_gift         BOOLEAN      NOT NULL DEFAULT FALSE,
    created_at      TIMESTAMP    NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at      TIMESTAMP    NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT fk_order_items_orders   FOREIGN KEY (order_id)   REFERENCES orders(id),
    CONSTRAINT fk_order_items_products FOREIGN KEY (product_id) REFERENCES products(id),
    CONSTRAINT chk_order_items_qty     CHECK (quantity > 0)
);

CREATE INDEX idx_order_items_order_id   ON order_items (order_id);
CREATE INDEX idx_order_items_product_id ON order_items (product_id);
```
