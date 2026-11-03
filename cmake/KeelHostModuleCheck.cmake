# ***************************************************************
#  KeelHostModuleCheck.cmake  -  a HostVM module plugin carries no second copy of a global-state
#  library (ymwang78/HostVM#43)
# ***************************************************************
# cmake -DMODULE=<plugin.so> -P KeelHostModuleCheck.cmake
#
# The process has one libzce, one libuv, one OpenSSL, one libpq, one hiredis and one Lua, all
# reached through the shared libzce. A plugin that links one of them statically defines its
# symbols itself and gets state of its own (a second Scheduler, a second Lua registry...). This
# looks for a few telltale symbols among the plugin's own definitions, and checks that the plugin
# exports khost_module_entry. Linux only.
# ***************************************************************
if(NOT MODULE OR NOT EXISTS "${MODULE}")
    message(FATAL_ERROR "MODULE=${MODULE} does not exist")
endif()
find_program(_NM nm REQUIRED)

execute_process(COMMAND "${_NM}" --defined-only "${MODULE}" OUTPUT_VARIABLE _all RESULT_VARIABLE _rc
                ERROR_QUIET)
if(NOT _rc EQUAL 0)
    # a stripped plugin has no symbol table: fall back to the dynamic one
    execute_process(COMMAND "${_NM}" -D --defined-only "${MODULE}" OUTPUT_VARIABLE _all)
endif()
execute_process(COMMAND "${_NM}" -D --defined-only "${MODULE}" OUTPUT_VARIABLE _exported)

set(_failed "")
foreach(_entry
        "zce_singleton_slot:libzce"
        "uv_run:libuv"
        "SSL_CTX_new:OpenSSL"
        "OPENSSL_init_crypto:OpenSSL"
        "PQconnectdb:libpq"
        "redisConnect:hiredis"
        "lua_newstate:Lua")
    string(REPLACE ":" ";" _pair "${_entry}")
    list(GET _pair 0 _symbol)
    list(GET _pair 1 _library)
    if(_all MATCHES "[ \t][A-Za-z][ \t]${_symbol}\n")
        string(APPEND _failed "  defines ${_symbol}: a static copy of ${_library}\n")
    endif()
endforeach()
if(NOT _exported MATCHES "[ \t]T[ \t]khost_module_entry\n")
    string(APPEND _failed "  does not export khost_module_entry\n")
endif()

if(_failed)
    message(FATAL_ERROR "${MODULE}:\n${_failed}")
endif()
message(STATUS "${MODULE}: no private copy of libzce, libuv, OpenSSL, libpq, hiredis or Lua")
