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
