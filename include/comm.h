#ifndef __comm_h__
#define __comm_h__

#ifdef __cplusplus

#include <time.h>
#ifndef _WIN32
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>
#include <sys/resource.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#define PATH_PARTITION        '\\'
#else
#define PATH_PARTITION        '/'
#ifndef MAX_PATH
#define MAX_PATH            PATH_MAX
#endif
#endif

class __autolock
{
public:
#ifdef _WIN32
    __autolock(CRITICAL_SECTION* lock)    { _lock = lock; EnterCriticalSection(_lock); }
    ~__autolock() { LeaveCriticalSection(_lock); }
#else
    __autolock(pthread_mutex_t* lock)    { _lock = lock; pthread_mutex_lock(_lock); }
    ~__autolock() { pthread_mutex_unlock(_lock); }
#endif
private:
#ifdef _WIN32
    CRITICAL_SECTION*    _lock;
#else
    pthread_mutex_t*    _lock;
#endif
};
#define autolock(cs)    __autolock ___lock(&(cs))

#ifdef _WIN32
#define initlock(cs)        InitializeCriticalSection(&(cs))
#define uninitlock(cs)        DeleteCriticalSection(&(cs))
#define initlock2(cs)        InitializeCriticalSection(&(cs))
#else
#define initlock(cs)        pthread_mutex_init(&(cs), NULL)
#define uninitlock(cs)        pthread_mutex_destroy(&(cs))
#define initlock2(cs)                    \
do {                                    \
    pthread_mutexattr_t mutexattr;        \
    pthread_mutexattr_init(&mutexattr);    \
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);    \
    pthread_mutex_init(&(cs), &mutexattr);    \
}while(0)
#endif

template<typename T>
class __autodelete
{
public:
    __autodelete(T*& t):m_t(t) {}
    ~__autodelete() { delete m_t; m_t = NULL; }
private:
    T*&    m_t;
};
#define autodelete(t)    __autodelete ___delete(t);

class __autofree
{
public:
    __autofree(void** ptr):m_ptr(ptr) {}
    ~__autofree() { if (*m_ptr) { free(*m_ptr); *m_ptr = NULL; } }
private:
    void** m_ptr;
};
#define autofree(ptr) __autofree ___free((void**)&ptr);

#define SAFEFREE(p)        \
do {                    \
    if (NULL != (p))    \
    {                    \
        free(p);        \
        (p) = NULL;        \
    }                    \
}while(0)

#define SAFEDELETE(p)    \
do{                        \
    delete p;            \
    p = NULL;            \
}while(0)

#define SAFEARRAYDELETE(x)    \
do{                            \
    if (x)                    \
    {                        \
        delete[] x;          \
        x = NULL;            \
    }                        \
}while(0)

#ifdef _WIN32

#define SAFECLOSEHANDLE(h)    \
do{                            \
    if (NULL != (h))        \
    {                        \
        CloseHandle(h);        \
        (h) = NULL;            \
    }                        \
}while(0)

#else

#define SAFECLOSEHANDLE(h)        \
do{                                \
    if ((h) >= 0)                \
    {                                \
        TEMP_FAILURE_RETRY(close(h));    \
        (h) = -1;            \
    }                        \
}while(0)

#endif

class time2string
{
public:
    time2string(time_t t)
    {
        if (0 == t)
        {
            _str[0] = '0';
            _str[1] = '\0';
        }
        else
        {
#ifdef _WIN32
            tm rtm{0};
            localtime_s(&rtm, &t);
            tm* ltm = &rtm;
            if (NULL == ltm)
                sprintf_s(_str, "invalid time: 0x%08x\n", (unsigned)t);
            else
                strftime(_str, 20, "%y-%m-%d %H:%M:%S", ltm);
#else
            tm rtm;
            tm* ltm = localtime_r(&t, &rtm);
            if (NULL == ltm)
                sprintf(_str, "invalid time: 0x%08x\n", (unsigned)t);
            else
                strftime(_str, 20, "%y-%m-%d %H:%M:%S", ltm);
#endif
        }
    }
public:
    const char* c_str()const { return _str;}
private:
    char _str[32];
};

class bin2string
{
public:
    bin2string(const void* bin, size_t len, bool upper = false)
    {
        static const char hexa[] = "0123456789abcdef";
        static const char hexA[] = "0123456789ABCDEF";
        if (2 * len >= 256)
        {
            snprintf(_str, 255, "length = %u, too long", (unsigned)len);
            return;
        }
        const char* hex = upper ? hexA : hexa;
        unsigned char *p = (unsigned char*)bin;
        for (size_t i = 0; i < len; ++i, ++p)
        {
            _str[i * 2] = hex[(*p) >> 4];
            _str[i * 2 + 1] = hex[(*p) & 0x0f];
        }
        _str[len * 2] = '\0';
    }
public:
    const char* c_str()const { return _str; }
private:
    char    _str[256];
};

#ifdef _WIN32
class bin2stringW
{
public:
    bin2stringW(const void* bin, size_t len, bool upper = false)
    {
        static const wchar_t hexa[] = L"0123456789abcdef";
        static const wchar_t hexA[] = L"0123456789ABCDEF";
        if (2 * len >= 256) {
            swprintf(_str, _countof(_str), L"length = %u, too long", (unsigned)len);
            return;
        }
        const wchar_t* hex = upper ? hexA : hexa;
        unsigned char *p = (unsigned char*)bin;
        for (size_t i = 0; i < len; ++i, ++p)
        {
            _str[i * 2] = hex[(*p) >> 4];
            _str[i * 2 + 1] = hex[(*p) & 0x0f];
        }
        _str[len * 2] = '\0';
    }
public:
    const wchar_t* c_str()const { return _str; }
private:
    wchar_t    _str[256];
};
#endif

#define USE_TIME2STRING(n, t)            time2string __##n(t)
#define USE_BIN2STRING(n, b, len)        bin2string __##n((b), (len), false)
#define USE_BIN2STRINGAF(n, b, len)        bin2string __##n((b), (len), true)
#define USE_STD2STRING(n, s)            bin2string __##n(s.data(), s.size())

#define TIME2STRING(n)        __##n.c_str()
#define BIN2STRING(n)        __##n.c_str()
#define BIN2STRING(n)        __##n.c_str()
#define STD2STRING(n)        __##n.c_str()

inline void strreplace(char* str, char s, char d)
{
    while (*str != '\0')
    {
        if (*str == s)
            *str = d;
        ++str;
    }
}

inline bool getexefile(char* exefile)
{
#ifdef _WIN32
    if (0 == GetModuleFileNameA(NULL, exefile, MAX_PATH))
        return false;
#else
    int c = readlink("/proc/self/exe", exefile, PATH_MAX);
    if (c <= 0)
        return false;
    exefile[c] = '\0';
#endif

    return true;
}

inline bool getexepath(char* exepath)
{
    if (!getexefile(exepath))
        return false;

    char* pc = strrchr(exepath, PATH_PARTITION);
    if (NULL == pc)
        return false;
    *pc = '\0';
    return true;
}

inline bool getexepath(char* filepath, const char* filename)
{
    if (!getexefile(filepath))
        return false;

    char* pc = strrchr(filepath, PATH_PARTITION);
    if (NULL == pc)
        return false;
    if (NULL == filename)
        *pc = '\0';
    else {
#ifndef _WIN32
        strcpy(pc + 1, filename);
#else
        strcpy_s(pc + 1, 128, filename);
#endif
    }
    return true;
}

#ifndef _WIN32
inline void enable_coredump(bool enable)
{
    rlimit rl = {0, 0};
    if (enable)
        rl.rlim_cur = rl.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &rl);
}
#endif

#endif
#endif
