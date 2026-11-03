# ***************************************************************
#  KeelHostModule.cmake  -  building a HostVM (KeelHost) module plugin (ymwang78/HostVM#43)
# ***************************************************************
# keelhost_add_module(<target>
#     NAME <Name>                  the module's name, and its directory under modules/
#     VERSION <version>
#     [VMTYPES <vmtype>...]        the vmtypes it provides
#     [REQUIRES <module>...]       modules loaded and initialised before it
#     [EXTENDS <vmtype>...]        vmtypes whose workers also load it
#     [PATH_APPEND <dir>...]       directories, relative to its own, a worker appends to PATH
#     [MIN_HOST <a.b.c.d>]         the oldest host version it runs on
#     SOURCES <source>...          its *_module.cpp and whatever else goes into the plugin
#     [LINK <library>...]          the module's static libraries and other dependencies
#     [OUTPUT_DIR <dir>]           where modules/<Name>/ is made; default CMAKE_BINARY_DIR
#     [NO_INSTALL])
#
# It builds a MODULE library, libkh_<name>.so / kh_<name>.dll (<name> in lower case), into
# <OUTPUT_DIR>/modules/<Name>/, writes module.xml next to it from the same arguments, so that the
# two cannot disagree, and installs both into ${PLATFORM_BIN_DIR}/modules/<Name>/.
#
# The plugin links the host's shared libzce: libzce.so from lib/<arch>/, or zce.lib from
# lib/win_amd64/ with ZCE_DLL / ZCEPY_DLL defined. It must not carry a copy of a library that has
# global state (libzce itself, libuv, OpenSSL, libpq, hiredis, Lua): the process has one of each,
# in libzce. A test, <target>_symbols, checks that on Linux (KeelHostModuleCheck.cmake).
# Symbols are hidden by default, those of the static libraries it links included; the plugin
# exports khost_module_entry only (KHOST_MODULE_DEFINE).
# On Linux it finds the shared libraries it needs (libidh, libzcepy ...) by its own RUNPATH, since
# hostvm.bin's does not apply to what a worker loads with dlopen: its own directory, the host's
# directory two levels up (bin/<arch>/, where Docker and the Debian package put them) and
# lib/<arch>/ of the workspace, relative to the installed plugin and absolute in the build tree.
# ***************************************************************
include_guard(GLOBAL)

set(_KEELHOST_MODULE_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}")

function(keelhost_add_module target)
    cmake_parse_arguments(KM "NO_INSTALL" "NAME;VERSION;MIN_HOST;OUTPUT_DIR"
                          "VMTYPES;REQUIRES;EXTENDS;PATH_APPEND;SOURCES;LINK" ${ARGN})
    foreach(_required NAME VERSION SOURCES)
        if(NOT KM_${_required})
            message(FATAL_ERROR "keelhost_add_module(${target}): ${_required} is required")
        endif()
    endforeach()
    if(NOT PLATFORM_SOLUTION_DIR OR NOT PLATFORM_LIB_DIR OR NOT PLATFORM_BIN_DIR)
        message(FATAL_ERROR "keelhost_add_module(${target}): include cmake/PlatformArch.cmake first")
    endif()
    if(NOT KM_OUTPUT_DIR)
        set(KM_OUTPUT_DIR "${CMAKE_BINARY_DIR}")
    endif()

    string(TOLOWER "${KM_NAME}" _lower)
    set(_output_name "kh_${_lower}")
    set(_dir "${KM_OUTPUT_DIR}/modules/${KM_NAME}")

    add_library(${target} MODULE ${KM_SOURCES})
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME "${_output_name}"
        LIBRARY_OUTPUT_DIRECTORY "${_dir}"
        RUNTIME_OUTPUT_DIRECTORY "${_dir}"
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON)
    foreach(_config ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER "${_config}" _config)
        set_target_properties(${target} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY_${_config} "${_dir}"
            RUNTIME_OUTPUT_DIRECTORY_${_config} "${_dir}")
    endforeach()
    # Lua's headers are libzce's (include/zce/lua), ahead of any system Lua
    target_include_directories(${target} BEFORE PRIVATE "${PLATFORM_SOLUTION_DIR}/include/zce/lua")
    target_include_directories(${target} PRIVATE "${PLATFORM_SOLUTION_DIR}/include")

    if(WIN32)
        set(_zce "${PLATFORM_SOLUTION_DIR}/${PLATFORM_LIB_DIR}/zce.lib")
        set(_zcepy "${PLATFORM_SOLUTION_DIR}/${PLATFORM_LIB_DIR}/zcepy.lib")
        target_compile_definitions(${target} PRIVATE ZCE_DLL ZCEPY_DLL)
        if(MSVC)
            target_compile_options(${target} PRIVATE /wd4251 /utf-8)
        endif()
    else()
        set(_zce "${PLATFORM_SOLUTION_DIR}/${PLATFORM_LIB_DIR}/libzce.so")
        set(_zcepy "${PLATFORM_SOLUTION_DIR}/${PLATFORM_LIB_DIR}/libzcepy.so")
        set_target_properties(${target} PROPERTIES
            BUILD_RPATH "$ORIGIN;${PLATFORM_SOLUTION_DIR}/${PLATFORM_LIB_DIR}"
            INSTALL_RPATH "$ORIGIN;$ORIGIN/../..;$ORIGIN/../../../../${PLATFORM_LIB_DIR}")
        if(NOT APPLE)
            # a dependency the plugin forgot to link fails here, not when a worker loads it
            target_link_options(${target} PRIVATE "LINKER:--no-undefined")
            # the module's own static libraries are compiled with default visibility: keep their
            # symbols out of the plugin's dynamic symbol table too
            target_link_options(${target} PRIVATE "LINKER:--exclude-libs,ALL")
        endif()
    endif()
    if(NOT EXISTS "${_zce}")
        message(FATAL_ERROR "keelhost_add_module(${target}): ${_zce} not found; build and install "
                            "the shared libzce first (build.sh / build_zce_dll.bat)")
    endif()
    target_link_libraries(${target} PRIVATE ${KM_LINK} "${_zce}")
    # a module that uses the embedded Python (requires py) links libzcepy too
    if("py" IN_LIST KM_REQUIRES AND EXISTS "${_zcepy}")
        target_link_libraries(${target} PRIVATE "${_zcepy}")
    endif()

    # module.xml, from the same arguments
    set(_xml "<module name=\"${KM_NAME}\" version=\"${KM_VERSION}\"")
    if(KM_MIN_HOST)
        string(APPEND _xml " min_host=\"${KM_MIN_HOST}\"")
    endif()
    string(APPEND _xml ">\n    <library linux=\"lib${_output_name}.so\" windows=\"${_output_name}.dll\" />\n")
    foreach(_v ${KM_VMTYPES})
        string(APPEND _xml "    <vmtype name=\"${_v}\" />\n")
    endforeach()
    foreach(_v ${KM_REQUIRES})
        string(APPEND _xml "    <requires name=\"${_v}\" />\n")
    endforeach()
    foreach(_v ${KM_EXTENDS})
        string(APPEND _xml "    <extends vmtype=\"${_v}\" />\n")
    endforeach()
    foreach(_v ${KM_PATH_APPEND})
        string(APPEND _xml "    <path append=\"${_v}\" />\n")
    endforeach()
    string(APPEND _xml "</module>\n")
    file(GENERATE OUTPUT "${_dir}/module.xml" CONTENT "${_xml}")

    if(NOT KM_NO_INSTALL)
        install(TARGETS ${target}
            LIBRARY DESTINATION "${PLATFORM_BIN_DIR}/modules/${KM_NAME}"
            RUNTIME DESTINATION "${PLATFORM_BIN_DIR}/modules/${KM_NAME}")
        install(FILES "${_dir}/module.xml" DESTINATION "${PLATFORM_BIN_DIR}/modules/${KM_NAME}")
    endif()

    if(UNIX AND NOT APPLE)
        add_test(NAME ${target}_symbols
                 COMMAND "${CMAKE_COMMAND}" "-DMODULE=$<TARGET_FILE:${target}>"
                         -P "${_KEELHOST_MODULE_CMAKE_DIR}/KeelHostModuleCheck.cmake")
    endif()
endfunction()
