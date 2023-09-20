#ifndef __ulog_h__
#define __ulog_h__

#include <stdarg.h>

#ifndef _WIN32
#define __cdecl 
#endif

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl ulog_init(
#ifdef _WIN32
                           HMODULE hModule,
#endif
                           const char* name);

#ifndef _WIN32
void __cdecl ulog_console(bool bconsole);
#endif

void __cdecl ulog_initfile(const char* filedir, const char* filename, long maxline, long maxfile);
void __cdecl ulog_uninit();
void __cdecl ulog_setlevel(unsigned level);
unsigned __cdecl ulog_getlevel();

void __cdecl ulog_sprintf(char* buffer, unsigned level, const char* filename, int linenumber, const char* fmt, ...);
void __cdecl ulog_vsprintf(char* buffer, unsigned level, const char* filename, int linenumber, const char* fmt, va_list argptr);

void __cdecl ulog_log(unsigned level, int alarm, const char* filename, int linenumber, const char* fmt, ...);
void __cdecl ulog_vlog(unsigned level, int alarm, const char* filename, int linenumber, const char* fmt, va_list argptr);

enum
{
    ULOG_TRACE =	0,
    ULOG_DEBUG =	1,			/* debug */
    ULOG_INFOR =	2,			/* info */
    ULOG_WARNI =	3,			/* warn */
    ULOG_ERROR =	4,			/* error */
    ULOG_FATAL =	5,			/* fatal */
    ULOG_NONEL =	255,		/* none */
};

#ifdef _WIN32
#define ULOG(lv, fn, ...)		ulog_log((lv), 1, __FILE__, __LINE__, (fn), __VA_ARGS__)
#else
#define ULOG(lv, fn, ...)		ulog_log((lv), 1, __FILE__, __LINE__, (fn), ##__VA_ARGS__)
#endif

#define ULOG_ICEEXCEPTION(lv, ex, s)	ULOG((lv), "%s: ice exception: name = %s, file = %s, line = %d", (s), ex.ice_name().c_str(), ex.ice_file(), ex.ice_line())
#define ULOG_CPPEXCEPTION(lv, s)		ULOG((lv), "%s: unknown exception", (s))
#ifdef _WIN32
#define ULOG_CEXCEPTION(lv, s, c)		ULOG((lv), "%s: seh, code = 0x%08x", (s), (c))
#define ULOG_SYSCALL(lv, s)			ULOG((lv), "%s: os error: %d", (s), GetLastError())
#else
#define ULOG_SYSCALL(lv, s)			ULOG((lv), "%s: errno: %d", (s), errno)
#endif

#ifdef __cplusplus
}
#endif

#endif
