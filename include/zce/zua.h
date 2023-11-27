#pragma once

#include <zce/zce_smartptr.h>
#include <zce/zce_task.h>
#include <zce/zce_dblock.h>
#include <zce/zua_serialize.h>
#if ZCE_ISSERVICE_MODE
#   include <Ice/Ice.h>
#endif

class zce_schedule;
class zce_reactor;
class zua_vm;
class zua_pimpl;
struct lua_State;

class ZCE_API zua : public zce_smartptr_mtbase
{
    zce_smartptr<zua_pimpl> pimpl_ptr_;

public:

    zua();

    ~zua();

    const zce_smartptr<zua_pimpl>& pimpl_ptr() const {
        return pimpl_ptr_;
    }

    void boot(const zce_smartptr<zce_schedule>&,
        const zce_smartptr<zce_reactor>&,
#if ZCE_ISSERVICE_MODE
        const Ice::CommunicatorPtr&,
#endif
        const std::string& path);

    int lpc_call_dblock(const std::string& svrname,
        const std::string& method,
        zce_dblock& dblock);

    //add your own lib ref here
    //luaL_requiref(L, "xxx", luaopen_xxx, 1);
    virtual void on_load_lib(lua_State* L) {};

    void luas_push(zce_dblock& dblock,
        const char* v) {
        luas_push_string(dblock, v);
    }

    void luas_push(zce_dblock& dblock,
        const std::string& v) {
        luas_push_lstring(dblock, v.c_str(), (zce_int32)v.length());
    }

#ifndef WIN32
    void luas_push(zce_dblock& dblock,
        long long v) {
        luas_push_integer(dblock, v);
    }
#endif

    void luas_push(zce_dblock& dblock,
        zce_int64 v) {
        luas_push_integer(dblock, v);
    }

    void luas_push(zce_dblock& dblock,
        zce_int32 v) {
        luas_push_integer(dblock, v);
    }

    void luas_push(zce_dblock& dblock,
        bool v) {
        luas_push_bool(dblock, v);
    }

    void luas_push(zce_dblock& dblock,
        void* ctx) {
        luas_push_lightuserdata(dblock, ctx);
    }

    template <typename T, typename... Args>
    void luas_push(zce_dblock& dblock,
        const T& v,
        Args... args) {
        luas_push(dblock, v);
        luas_push(dblock, args...);
    }

    template<typename... Args>
    int lpc_call(const std::string& svrname, 
        const std::string& method,
        Args... args) {
        zce_dblock dblock;
        luas_push(dblock, args...);
        return lpc_call_dblock(svrname, method, dblock);
    }
};


class ZCE_API zua_cotask_caller : public zce_task
{
    ZCE_OBJECT_DECLARE;
protected:

    //zce_int64 sid_;
    zce_smartptr<zua_vm> vmptr_;
    lua_State* L_;

    bool ok_;
    zce_dblock dblock_;

public:

    static int caller_continue_func(lua_State *L, int status, ptrdiff_t ctx);

    zua_cotask_caller(const char* name,
        zua_vm* vm,
        lua_State* L);

    zua_cotask_caller(const char* name,
        zua_vm* vm,
        lua_State* L,
        zce_dblock&& dblock);

    ~zua_cotask_caller();

    int rpc_return(bool ok, const zce_dblock& dblock);

    virtual void call();

    virtual int resume_async();

    virtual int start_async() = 0;
};
