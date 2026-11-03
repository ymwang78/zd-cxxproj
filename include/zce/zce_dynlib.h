#pragma once
// ***************************************************************
//  zce_dynlib  -  loading a shared library into the process
// ***************************************************************
// The one way a libzce program loads a plugin or another shared library at run time (first for
// HostVM's modules, ymwang78/HostVM#43). Both platforms look up what the library itself needs
// next to it before the usual places:
// - POSIX: dlopen(RTLD_NOW | RTLD_LOCAL). RTLD_NOW reports a missing symbol at load time rather
//   than at the first call, and RTLD_LOCAL keeps the library's own symbols out of the way of the
//   libraries loaded after it.
// - Windows: LoadLibraryExW(LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS),
//   so that a DLL finds the DLLs in its own directory; the path must be absolute for that.
// A loaded library is never unloaded: a program that loads plugins ends with _exit, and an
// unloaded plugin may still have code on a thread or in a registered callback.
// ***************************************************************
#include <zce/zce_config.h>
#include <string>

namespace zce {

class ZCE_API DynLib {
  public:
    DynLib() = default;

    /**
     * @brief Loads the shared library at @p path, an absolute path in UTF-8.
     * @param error Where to write why it failed, the loader's own message included; may be null.
     * @return false when @p path is not absolute, is no file, or the loader refuses it.
     */
    bool open(const std::string& path, std::string* error = nullptr);

    /** @brief The address of the exported symbol @p name, or null when there is none. */
    void* symbol(const char* name) const;

    bool isOpen() const noexcept { return handle_ != nullptr; }

    /** @brief The path open() was given. */
    const std::string& path() const noexcept { return path_; }

  private:
    void* handle_ = nullptr;
    std::string path_;
};

}  // namespace zce
