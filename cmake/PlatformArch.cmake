# PlatformArch.cmake
# 公共的平台和架构检测模块
# 使用方法: include(cmake/PlatformArch.cmake)

# 检测平台和架构

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(ARCH_NAME "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(ARCH_NAME "aarch64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armv7")
    set(ARCH_NAME "armv7")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686")
    set(ARCH_NAME "i386")
else()
    set(ARCH_NAME ${CMAKE_SYSTEM_PROCESSOR})
endif()

if(WIN32)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        set(PLATFORM_ARCH "win_amd64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
        set(PLATFORM_ARCH "win_arm64")
    else()
        set(PLATFORM_ARCH "win_${CMAKE_SYSTEM_PROCESSOR}")
    endif()
elseif(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        set(PLATFORM_ARCH "linux_x86_64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
        set(PLATFORM_ARCH "linux_aarch64")
    else()
        set(PLATFORM_ARCH "linux_${CMAKE_SYSTEM_PROCESSOR}")
    endif()
else()
    set(PLATFORM_ARCH "${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}")
endif()

# 获取解决方案根目录（当前文件的父目录）
get_filename_component(PLATFORM_SOLUTION_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

# 输出检测结果
message(STATUS "Platform-Architecture detected: ${PLATFORM_ARCH}")
message(STATUS "Solution directory: ${PLATFORM_SOLUTION_DIR}")

# 设置一些常用的路径变量
set(PLATFORM_LIB_DIR "lib/${PLATFORM_ARCH}")
set(PLATFORM_BIN_DIR "bin/${PLATFORM_ARCH}")

if (CMAKE_CURRENT_FUNCTION)
    # 导出变量到父作用域，使其可以被调用者使用
    set(ARCH_NAME ${ARCH_NAME} PARENT_SCOPE)
    set(PLATFORM_ARCH ${PLATFORM_ARCH} PARENT_SCOPE)
    set(PLATFORM_LIB_DIR ${PLATFORM_LIB_DIR} PARENT_SCOPE)
    set(PLATFORM_BIN_DIR ${PLATFORM_BIN_DIR} PARENT_SCOPE)
    set(PLATFORM_SOLUTION_DIR ${PLATFORM_SOLUTION_DIR} PARENT_SCOPE)
endif()

# 消除常见警告（仅针对 GCC/Clang）
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-unused-function)
    # for C++
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-reorder>)
endif()

