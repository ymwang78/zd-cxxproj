#ifndef NOT_KEPLER_PROJECT_COMPAT54_H_
#define NOT_KEPLER_PROJECT_COMPAT54_H_

#if defined(__cplusplus) && !defined(COMPAT53_LUA_CPP)
extern "C" {
#endif
#if __has_include(<lua/lua.h>)
  #include <lua/lua.h>
  #include <lua/lauxlib.h>
  #include <lua/lualib.h>
#else
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
#endif
#if defined(__cplusplus) && !defined(COMPAT53_LUA_CPP)
}
#endif

#if defined(LUA_VERSION_NUM) && (LUA_VERSION_NUM == 504 || LUA_VERSION_NUM == 505)

#if !defined(LUA_ERRGCMM)
/* So Lua 5.4+ actually removes this, which breaks sol2...
 man, this API is quite unstable...!
*/
#  define LUA_ERRGCMM (LUA_ERRERR + 2)
#endif /* LUA_ERRGCMM define */

#if !defined(LUA_ERRFILE)
/* Lua 5.5 also removes LUA_ERRFILE */
#  define LUA_ERRFILE (LUA_ERRERR + 3)
#endif /* LUA_ERRFILE define */

#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 505
/* Lua 5.5 changes lua_newstate to take 3 parameters (adds seed parameter)
   We need to provide a macro that wraps the old 2-parameter version */
#  undef lua_newstate
#  define lua_newstate(f, ud) (lua_newstate)((f), (ud), 0)
#endif /* Lua 5.5 lua_newstate compatibility */

#endif // Lua 5.4 and 5.5 only

#endif // NOT_KEPLER_PROJECT_COMPAT54_H_
