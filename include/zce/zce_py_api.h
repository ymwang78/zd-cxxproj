#pragma once
// ***************************************************************
//  zce_py_api  -  the functions of libzcepy, libzce's embedded Python runtime
// ***************************************************************
// When libzce is a shared library, the Python VM (zvm/zpy) and the functions below are a shared
// library of their own, libzcepy, so that libzce does not depend on libpython
// (ymwang78/libzce#215); code that calls them, runs a Python VM or uses zce_pybind11.h links
// libzcepy as well as libzce. A static libzce.a still contains them. zce_api.h includes this
// header, so callers do not need to include it themselves.
// ***************************************************************
#include <zce/zce_inc.h>
#include <string>

/**
 * @brief Marks what libzcepy exports: dllexport while zcepy.dll is built (ZCEPY_EXPORTS),
 *        dllimport in code using zcepy.dll (ZCEPY_DLL), default visibility on ELF.
 */
#ifndef ZCEPY_API
#    if defined(_WIN32) && defined(ZCEPY_EXPORTS)
#        define ZCEPY_API __declspec(dllexport)
#    elif defined(_WIN32) && defined(ZCEPY_DLL)
#        define ZCEPY_API __declspec(dllimport)
#    elif defined(__GNUC__) && !defined(_WIN32)
#        define ZCEPY_API __attribute__((visibility("default")))
#    else
#        define ZCEPY_API
#    endif
#endif

extern "C" {
/** @brief Starts the embedded interpreter, with the home zce_resolve_pyenv_home() finds. */
ZCEPY_API int zce_init_pyenv(const char* python_home);

/** @brief Stops the embedded interpreter started by zce_init_pyenv(). */
ZCEPY_API void zce_fini_pyenv();

/**
 * @brief Wires Python logging (ZceLogHandler) into the ZCE log, and optionally redirects
 *        sys.stdout / sys.stderr for print (stdout to trace, stderr to error).
 * @param redirect_stdio 0 for logging only, nonzero for logging and stdio. Needs zce_init_pyenv().
 */
ZCEPY_API int zce_setup_py_logging(int redirect_stdio);
}

// 解析嵌入式 Python home，返回存在的绝对路径；一个候选都不存在时返回空串。
// 候选优先级：python_home 实参 > ZCE_VIRTUAL_VENV 环境变量 > 平台默认值
// （Windows 为 ../WinPy<主><次>/python，Linux 为 /opt/venv）。
// 即 python_home 传 nullptr 时，ZCE_VIRTUAL_VENV 就是优先级最高的覆盖手段。
// 其中相对路径先按「可执行文件所在目录」解析，再按进程工作目录解析：服务与测试都可能
// 从任意目录启动，只按工作目录解析会在目录不对时静默失败，随后取 GIL 直接访问违例。
// zce_init_pyenv() 内部用的就是这个函数，python_home 传 nullptr 即表示只用默认候选。
ZCEPY_API std::string zce_resolve_pyenv_home(const char* python_home = nullptr);
