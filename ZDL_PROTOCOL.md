# ZCE RPC Protocol Writing Guide (ZDL / zGen)

This guide describes how to define remote communication protocols using the **ZDL (Zce Definition Language)** and the **zGen** code generator.

## Overview

The workflow is:
1. Write a `.ptl` protocol definition file using ZDL syntax
2. Run `zgen` to generate C++ header files (`*_proto.h`, `*_pack.h`) and implementation (`*_pack.cpp`)
3. Include the generated headers and link the ZDS serialization functions in your project

Tool location: `/zdata/cxxproj/bin/linux_x86_64/zgen`

---

## Protocol File Format

- **File extension**: `.ptl`
- **Encoding**: UTF-8 with BOM (mandatory, same as all C/C++ source files in this project)
- **Location convention**: place alongside the module that owns the protocol

---

## ZDL Syntax Reference

### 1. Namespace

Every protocol file should declare a namespace (mirrors C++ namespace).

```
namespace my_module
{
    // enums and structs go here
};
```

### 2. Enumerations

```
enum MyEnum
{
    MyEnum_None   = 0,
    MyEnum_Alpha  = 0x00000001,
    MyEnum_Beta,          // auto-increments from previous value
    MyEnum_Error  = 0x80001000,
};
```

Generated C++:
```cpp
enum MyEnum : int
{
    MyEnum_None  = (int)0x00000000,
    MyEnum_Alpha = (int)0x00000001,
    MyEnum_Beta  = (int)0x00000002,
    MyEnum_Error = (int)0x80001000,
};
```

**Rules:**
- Enumerators may have explicit integer values (decimal or hex)
- Values auto-increment from the last explicit value
- Keywords cannot be used as enumerator names

### 3. Structs (Messages)

Basic struct:
```
struct MyMessage
{
    int32   seq_id;
    string  name;
    double  value;
};
```

With struct-level metadata (enables BSON serialization):
```
[bson] struct MyBsonMessage
{
    int32   id;
    string  label;
};
```

**Rules:**
- Each field declaration ends with `;`
- Struct definition ends with `;` after the closing brace
- Keywords cannot be used as struct or field names

### 4. Primitive Types

| ZDL keyword          | C++ type          | Notes                          |
|----------------------|-------------------|--------------------------------|
| `char`               | `zce_char`        | signed 8-bit                   |
| `uchar` / `byte`     | `zce_byte`        | unsigned 8-bit                 |
| `short` / `int16`    | `zce_int16`       | signed 16-bit                  |
| `ushort` / `uint16`  | `zce_uint16`      | unsigned 16-bit                |
| `int` / `int32`      | `zce_int32`       | signed 32-bit (most common)    |
| `uint32`             | `zce_uint32`      | unsigned 32-bit                |
| `int64`              | `zce_int64`       | signed 64-bit                  |
| `uint64`             | `zce_uint64`      | unsigned 64-bit                |
| `float`              | `zce_float`       | 32-bit float                   |
| `double`             | `zce_double`      | 64-bit float                   |
| `astring`            | `zce_astring`     | ASCII / narrow string          |
| `string` / `tstring` / `ntchars` | `zce_tstring` | General text string   |
| `wstring`            | `zce_wstring`     | Wide (Unicode) string          |
| `bytevec`            | `zce_bytevec`     | Raw byte vector                |
| `datetime`           | datetime          | Date/time value                |
| `any`                | `zce::Any`        | Variant / any type             |
| `dblmat`             | `zce_dblmat`      | Double-precision matrix        |
| `fltmat`             | `zce_fltmat`      | Single-precision matrix        |

### 5. Field Declarations

#### Scalar (single value)
```
int32   field_name;
```
Generated: `zce_int32 field_name;`

#### Fixed-size array
```
int32   values[10];
char    buffer[256];
```
Generated: `zce_int32 values[10];` / `zce_char buffer[256];`

#### Variable-length array (unbounded)
```
int32   items[~];
string  names[~];
```
Generated: `std::vector<zce_int32> items;` / `std::vector<zce_tstring> names;`

Short form using pointer syntax (equivalent, min=0, max=unbounded):
```
int32*  items;
```

#### Variable-length array with bounds
```
int32   values[1~100];   // 1 to 100 elements (closed interval)
string  tags[0~50];      // 0 to 50 strings (min 0 => optional)
```

> **`[ ]` always means element multiplicity** — how *many* values the field holds —
> for every type, including strings. Bounds are a **closed interval** `[min, max]`.
> The string *content length* is a separate concern; see below.

#### String length constraint — `string(min~max)`
A length bound is attached to the **type**, written with parentheses, and is
independent of the `[ ]` multiplicity. It applies to the string family
(`astring`, `tstring`/`string`, `wstring`) and to `bytevec`. Closed interval.

```
string(2~32)  name;        // one string, length 2..32
string(~64)   label;       // one string, length 0..64 (upper bound only)
string(16)    code;        // one string, exact length 16
string        free;        // one string, unbounded length
```

Because length and multiplicity are orthogonal, they compose — this is how you
express an **array of length-bounded strings** (previously a separate `strvec`):

```
string(2~32)  tags[~];     // vector of strings, each 2..32 chars
string(2~32)  fixed[8];    // 8 strings, each 2..32 chars
```

**Enforcement & metadata.** The generated C++ ZDS code enforces the length:
`zds_pack` rejects out-of-range strings before serializing and `zds_unpack`
re-validates after deserializing (returning `ZCE_ERROR_SYNTAX`), per element for
vectors. A `0` bound means "unbounded on that side" (so `string(~64)` emits only
an upper check). The bounds are also exposed as `_str_len_min` / `_str_len_max`
member metadata, so other generators (SQL/ORM column sizing, docs, etc.) can read
them via `fetch_meta`.

#### Nested struct
```
struct OtherStruct  inner;
struct OtherStruct  list[~];    // vector of OtherStruct
```
Generated: `OtherStruct inner;` / `std::vector<OtherStruct> list;`

#### Cross-namespace struct reference
```
struct OtherModule::SomeType  field;
```

### 6. Optional Fields

Fields with min_size = 0 are implicitly optional. Use the `[o]` metadata to explicitly mark optional:
```
struct MyRequest
{
    int32   required_id;
    [o] string  optional_remark;    // optional - may be absent in serialized form
};
```

### 7. Default Values

```
struct Config
{
    int32   timeout  = 30;
    double  scale    = 1.0;
    string  label    = "default";
};
```

### 8. Comments

```
struct Documented
{
    // Single-line comment
    int32   field1;

    /* Multi-line
       comment */
    string  field2;
};
```

### 9. Metadata Annotations

Metadata is placed before the field or struct in `[key]` or `[key, value]` format:

| Metadata                  | Meaning                                           |
|---------------------------|---------------------------------------------------|
| `[o]` or `[O]`            | Field is optional                                 |
| `[bson]`                  | Enable BSON serialization for this struct         |
| `[key, "string value"]`   | Custom annotation with string value               |
| `[key, 42]`               | Custom annotation with integer value              |
| `[key, 3.14]`             | Custom annotation with double value               |

---

## Full Example

```
namespace my_service
{

// -------------------------------------------------------
// Status codes
// -------------------------------------------------------
enum ErrCode
{
    ErrCode_OK       = 0x00000000,
    ErrCode_Base     = 0x80002000,
    ErrCode_Invalid,
    ErrCode_Timeout,
    ErrCode_NotFound,
};

// -------------------------------------------------------
// Shared types
// -------------------------------------------------------
struct Point
{
    double  x;
    double  y;
    double  z;
};

// -------------------------------------------------------
// Request / response messages
// -------------------------------------------------------
struct QueryRequest
{
    int32           seq_id;
    string          topic;
    [o] int64       from_time_us;    // optional time filter
    [o] int64       to_time_us;
    int32           max_results = 100;
};

struct DataRecord
{
    int64           timestamp_us;
    double          value;
    int32           quality;
    [o] string      tag_name;
};

struct QueryResponse
{
    int32                   seq_id;
    enum ErrCode            error_code;
    string                  message;
    struct DataRecord       records[~];    // variable-length list
    struct Point            location;      // nested struct
};

}
```

---

## Code Generation with zGen

### Commands

```bash
# Step 1: Generate C++ type definitions (structs/enums)
zgen -t header_zds -f * -i my_service.ptl -o my_service_proto

# Step 2: Generate ZDS (de)serialization declarations + implementation
zgen -t cpp_zds -f * -I my_service_proto.h -I my_service_pack.h -i my_service.ptl -o my_service

# Step 3 (optional): Generate BSON serialization
zgen -t cpp_bson -f * -I my_service_proto.h -i my_service.ptl -o my_service
```

### Generated File Structure

| File                  | Content                                                             |
|-----------------------|---------------------------------------------------------------------|
| `my_service_proto.h`  | C++ `namespace my_service { enum ...; struct ...; }` definitions   |
| `my_service_pack.h`   | `namespace zce::zdp { zds_pack/zds_unpack declarations }` header   |
| `my_service_pack.cpp` | ZDS pack/unpack implementations                                     |

**Important**: Generated files contain the header comment `// This file was generated by zGen, don't modify manually.` — never hand-edit them; regenerate instead.

### Generated C++ Pattern

From `my_service_proto.h`:
```cpp
namespace my_service {
    enum ErrCode : int {
        ErrCode_OK    = (int)0x00000000,
        ErrCode_Base  = (int)0x80002000,
        ...
    };
    struct DataRecord {
        static DataRecord _empty;
        bool operator==(const DataRecord& _t) const noexcept { ... }
        zce_int64   timestamp_us;
        zce_double  value;
        zce_int32   quality;
        zce_tstring tag_name;     // optional - present in struct, absent in wire format when empty
    };
    struct QueryResponse {
        ...
        std::vector<my_service::DataRecord> records;  // [~] → std::vector
        my_service::Point                   location;
    };
}
```

From `my_service_pack.h`:
```cpp
namespace zce { namespace zdp {
    int zds_pack(zce_byte* buf, int size, const my_service::DataRecord& _t,
                 zds_context_t* ctx, bool has_prefix);
    int zds_unpack(my_service::DataRecord& _t, const zce_byte* buf, int size,
                   zds_context_t* ctx, bool has_prefix);
    // ... one pair per struct
}}
```

---

## Using Serialized Protocol in C++

### Include generated headers
```cpp
#include "my_service_proto.h"
#include "my_service_pack.h"
#include <zce/zce_dblock.h>
```

### Serialize (pack)
```cpp
my_service::QueryRequest req;
req.seq_id = 1;
req.topic  = "sensor/temp";
req.max_results = 50;

zce::DBlock buf(4096);
int len = zce::zdp::zds_pack(buf.wr_ptr(), buf.space(),
                              req, nullptr, true);
buf.wr_ptr(len);
```

### Deserialize (unpack)
```cpp
my_service::QueryResponse resp;
int consumed = zce::zdp::zds_unpack(resp, buf.rd_ptr(), buf.length(),
                                    nullptr, true);
for (auto& rec : resp.records) {
    // process each DataRecord
}
```

---

## Naming Conventions

Follow the project coding standards within `.ptl` files:

| Element     | Convention         | Example                          |
|-------------|--------------------|----------------------------------|
| Namespace   | snake_case         | `my_module`, `sensor_data`       |
| Struct name | PascalCase         | `QueryRequest`, `DataRecord`     |
| Enum name   | PascalCase         | `ErrCode`, `TagType`             |
| Enumerator  | PascalCase prefix  | `ErrCode_OK`, `TagType_MV`       |
| Field name  | snake_case         | `seq_id`, `timestamp_us`         |

**Field naming tips:**
- Include units in numeric field names: `timeout_ms`, `temperature_celsius`, `timestamp_us`
- Use `is_` prefix for boolean-like integer fields: `is_active`, `is_online`
- Suffix list/collection fields with `_list` or use plural: `records`, `tag_list`

---

## CMake Integration

```cmake
# Add generated sources
target_sources(my_target PRIVATE
    my_service_pack.cpp
)

# Include path for generated headers
target_include_directories(my_target PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../include
)

# Link ZCE
target_link_libraries(my_target PUBLIC
    /zdata/cxxproj/libsrc/libzce/build/libzce.a
)
```

---

## Key Rules Summary

1. **File encoding**: UTF-8 with BOM
2. **File extension**: `.ptl` for protocol definitions
3. **Every definition ends with `;`** (structs, enums, and each field)
4. **Variable-length fields** use `[~]` suffix or `*` pointer notation → `std::vector<T>`
5. **`[ ]` = multiplicity, `( )` = string length** — both closed intervals, independent and composable (`string(2~32) tags[~]`)
6. **Optional fields** use `[o]` metadata or min_size=0 bounds
7. **Never edit generated files** — always edit the `.ptl` and regenerate
8. **Cross-namespace struct references** use `struct Ns::TypeName field;` syntax
9. **Enum auto-increment**: values increment from the last explicit assignment
