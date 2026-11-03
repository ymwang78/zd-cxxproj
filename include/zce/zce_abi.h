#pragma once
// ***************************************************************
//  zce_abi  -  what a module built against libzce must share with the libzce it runs with
// ***************************************************************
// A plugin (DLL / shared object) compiled against libzce's headers runs with whatever libzce is
// already in the process. That only works if both were compiled from compatible headers
// (ZCE_ABI_VERSION) and with the same compiler, standard library and configuration
// (ZCE_BUILD_ID): std::string, std::function and zce::SmartPtr cross the boundary as they are.
//
// A host compares the two values a module was compiled with, which the module reports through
// its own descriptor, against zce_abi_version() and zce_build_id(), the values libzce itself was
// compiled with. zce_abi_check() does that comparison and says what differs.
// ***************************************************************
#include <zce/zce_inc.h>
#include <stddef.h>
#include <string>  // for the standard library's configuration macros used by ZCE_BUILD_ID

/**
 * @brief Version of libzce's binary interface. +1 on any incompatible change to the public headers.
 *
 * Bump it, in the same change, when a module compiled against the old headers would misbehave
 * with the new libzce or the other way round. For example:
 * - a class in include/zce/ that crosses the module boundary gains, loses or reorders a data
 *   member, a base class or a virtual function (zvm::Machine, VirtualMachineStub, Reactor,
 *   Object, Service, ...);
 * - an exported function changes signature, or modules start needing a function an older
 *   libzce lacks;
 * - a type that zdp / zds pack and unpack across modules changes layout;
 * - an inline function or template changes how it uses state that libzce keeps, as
 *   zce::Singleton reads its slot.
 * Header-only helpers that only ever run inside the module, and changes inside libzce's own
 * .cpp files, do not count.
 *
 * History:
 * - 1: first version (ymwang78/libzce#214), with zce::Singleton keeping its instances in
 *      libzce (ymwang78/libzce#212).
 */
#define ZCE_ABI_VERSION 1

#define ZCE_ABI_STR_(x) #x
#define ZCE_ABI_STR(x) ZCE_ABI_STR_(x)

/**
 * @brief The compiler, standard library and configuration this translation unit is compiled with.
 *
 * Two binaries may share C++ objects only if these are equal, so the comparison is strict on
 * purpose: modules are built in the same pipeline as the host, and a refused module says why
 * where a mismatched one would crash somewhere unrelated.
 * - MSVC: "msvc-<_MSC_VER>-idl<_ITERATOR_DEBUG_LEVEL>-<md|mt>-<dbg|rel>". md / mt is the dynamic
 *   or static CRT, dbg the debug CRT (_DEBUG), which also has a heap of its own.
 * - libstdc++: "gnu-<__GLIBCXX__>-cxx11abi<_GLIBCXX_USE_CXX11_ABI>-<dbg|rel>". dbg is the debug
 *   mode of the library (_GLIBCXX_DEBUG), which changes the layout of its containers; NDEBUG
 *   changes nothing that crosses the boundary.
 * - libc++: "libcxx-<_LIBCPP_VERSION>-abi<_LIBCPP_ABI_VERSION>".
 */
#if defined(_MSC_VER)
#    if defined(_DLL)
#        define ZCE_BUILD_ID_CRT_ "md"
#    else
#        define ZCE_BUILD_ID_CRT_ "mt"
#    endif
#    if defined(_DEBUG)
#        define ZCE_BUILD_ID_CFG_ "dbg"
#    else
#        define ZCE_BUILD_ID_CFG_ "rel"
#    endif
#    define ZCE_BUILD_ID                                  \
        "msvc-" ZCE_ABI_STR(_MSC_VER) "-idl" ZCE_ABI_STR( \
            _ITERATOR_DEBUG_LEVEL) "-" ZCE_BUILD_ID_CRT_ "-" ZCE_BUILD_ID_CFG_
#elif defined(_LIBCPP_VERSION)
#    define ZCE_BUILD_ID \
        "libcxx-" ZCE_ABI_STR(_LIBCPP_VERSION) "-abi" ZCE_ABI_STR(_LIBCPP_ABI_VERSION)
#elif defined(__GLIBCXX__)
#    if defined(_GLIBCXX_DEBUG)
#        define ZCE_BUILD_ID_CFG_ "dbg"
#    else
#        define ZCE_BUILD_ID_CFG_ "rel"
#    endif
#    define ZCE_BUILD_ID                                         \
        "gnu-" ZCE_ABI_STR(__GLIBCXX__) "-cxx11abi" ZCE_ABI_STR( \
            _GLIBCXX_USE_CXX11_ABI) "-" ZCE_BUILD_ID_CFG_
#else
#    error "zce_abi.h: unknown C++ standard library, give it a ZCE_BUILD_ID"
#endif

extern "C" {
/** @brief ZCE_ABI_VERSION as libzce itself was compiled with it. */
ZCE_API unsigned zce_abi_version(void);

/** @brief ZCE_BUILD_ID as libzce itself was compiled with it. */
ZCE_API const char* zce_build_id(void);

/**
 * @brief Whether a module compiled with @p abi_version and @p build_id may run with this libzce.
 *
 * @param reason Where to write what differs and both values, for the host to log; may be null.
 * @param reason_size Size of @p reason in bytes; the text is cut to fit and always terminated.
 * @return 0 when both match, with @p reason empty; ZCE_ERROR_VERSION otherwise, for instance
 *         "build id: module msvc-1951-idl2-md-dbg, libzce msvc-1951-idl0-md-rel".
 */
ZCE_API int zce_abi_check(unsigned abi_version, const char* build_id, char* reason,
                          size_t reason_size);
}
