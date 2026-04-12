# C++ Projects Structure

This directory contains all C++ projects with a shared structure for common dependencies and individual project directories.

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

All test code must follow these rules:

### GTest Framework
- **Mandatory**: All tests MUST use Google Test (GTest) framework
- **Test file naming**: `test_*.cpp` (e.g., `test_per_tag_tracking.cpp`)
- **Test structure**: Use `TEST_F()` for fixture-based tests or `TEST()` for simple tests

### Main Function Rule
- **Critical**: ALL test files MUST wrap the main function with `#ifndef USE_GTEST_MAIN` macro
- **Purpose**: Allows flexible test compilation with custom or default GTest main
- **Standard template**:
```cpp
#ifndef USE_GTEST_MAIN
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
```

### Test Organization
- Place all tests in the `tests/` directory
- Each test file should focus on a single component or feature
- Use descriptive test names: `ComponentName_BehaviorUnderTest_ExpectedOutcome`
- Example: `PerTagTrackingTest_BasicWriteSmallBuffer`

### CMake Integration
- Add test executables to CMakeLists.txt with conditional compilation
- Link against GTest libraries: `GTest::gtest` and `GTest::gtest_main`
- Register tests with `add_test(NAME TestName COMMAND test_executable)`

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
