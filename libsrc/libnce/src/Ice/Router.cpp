// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Router.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/Router.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>
#include <IceUtil/DisableWarnings.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

static const ::std::string __Ice__Router__getClientProxy_name = "getClientProxy";

static const ::std::string __Ice__Router__getServerProxy_name = "getServerProxy";

static const ::std::string __Ice__Router__addProxy_name = "addProxy";

static const ::std::string __Ice__Router__addProxies_name = "addProxies";

ICE_DECLSPEC_EXPORT ::Ice::Object* IceInternal::upCast(::Ice::Router* p) { return p; }
ICE_DECLSPEC_EXPORT ::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::Ice::Router* p) { return p; }

void
Ice::__read(::IceInternal::BasicStream* __is, ::Ice::RouterPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::Ice::Router;
        v->__copyFrom(proxy);
    }
}

bool
Ice::AMI_Router_getClientProxy::__invoke(const ::Ice::RouterPrx& __prx, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prx->__checkTwowayOnly("getClientProxy");
        __prepare(__prx, __Ice__Router__getClientProxy_name, ::Ice::Nonmutating, __ctx);
        __os->endWriteEncaps();
        return __send();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __releaseCallback(__ex);
        return false;
    }
}

void
Ice::AMI_Router_getClientProxy::__response(bool __ok)
{
    ::Ice::ObjectPrx __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
            }
            return;
        }
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __finished(__ex);
        return;
    }
    ice_response(__ret);
    __releaseCallback();
}

bool
Ice::AMI_Router_addProxies::__invoke(const ::Ice::RouterPrx& __prx, const ::Ice::ObjectProxySeq& proxies, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prx->__checkTwowayOnly("addProxies");
        __prepare(__prx, __Ice__Router__addProxies_name, ::Ice::Idempotent, __ctx);
        if(proxies.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            ::Ice::__writeObjectProxySeq(__os, &proxies[0], &proxies[0] + proxies.size());
        }
        __os->endWriteEncaps();
        return __send();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __releaseCallback(__ex);
        return false;
    }
}

void
Ice::AMI_Router_addProxies::__response(bool __ok)
{
    ::Ice::ObjectProxySeq __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
            }
            return;
        }
        __is->startReadEncaps();
        ::Ice::__readObjectProxySeq(__is, __ret);
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __finished(__ex);
        return;
    }
    ice_response(__ret);
    __releaseCallback();
}
#ifdef __SUNPRO_CC
class ICE_DECLSPEC_EXPORT IceProxy::Ice::Router;
#endif

::Ice::ObjectPrx
IceProxy::Ice::Router::getClientProxy(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__Router__getClientProxy_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Router* __del = dynamic_cast< ::IceDelegate::Ice::Router*>(__delBase.get());
            return __del->getClientProxy(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::Ice::Router::getClientProxy_async(const ::Ice::AMI_Router_getClientProxyPtr& __cb)
{
    return __cb->__invoke(this, 0);
}

bool
IceProxy::Ice::Router::getClientProxy_async(const ::Ice::AMI_Router_getClientProxyPtr& __cb, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, &__ctx);
}

::Ice::ObjectPrx
IceProxy::Ice::Router::getServerProxy(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__Router__getServerProxy_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Router* __del = dynamic_cast< ::IceDelegate::Ice::Router*>(__delBase.get());
            return __del->getServerProxy(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::Ice::Router::addProxy(const ::Ice::ObjectPrx& proxy, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Router* __del = dynamic_cast< ::IceDelegate::Ice::Router*>(__delBase.get());
            __del->addProxy(proxy, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::ObjectProxySeq
IceProxy::Ice::Router::addProxies(const ::Ice::ObjectProxySeq& proxies, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__Router__addProxies_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Router* __del = dynamic_cast< ::IceDelegate::Ice::Router*>(__delBase.get());
            return __del->addProxies(proxies, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::Ice::Router::addProxies_async(const ::Ice::AMI_Router_addProxiesPtr& __cb, const ::Ice::ObjectProxySeq& proxies)
{
    return __cb->__invoke(this, proxies, 0);
}

bool
IceProxy::Ice::Router::addProxies_async(const ::Ice::AMI_Router_addProxiesPtr& __cb, const ::Ice::ObjectProxySeq& proxies, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, proxies, &__ctx);
}

const ::std::string&
IceProxy::Ice::Router::ice_staticId()
{
    return ::Ice::Router::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::Ice::Router::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::Ice::Router);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::Ice::Router::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::Ice::Router);
}

::IceProxy::Ice::Object*
IceProxy::Ice::Router::__newInstance() const
{
    return new Router;
}

::Ice::ObjectPrx
IceDelegateM::Ice::Router::getClientProxy(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Router__getClientProxy_name, ::Ice::Nonmutating, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::ObjectPrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::ObjectPrx
IceDelegateM::Ice::Router::getServerProxy(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Router__getServerProxy_name, ::Ice::Nonmutating, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::ObjectPrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::Ice::Router::addProxy(const ::Ice::ObjectPrx& proxy, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Router__addProxy_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(proxy);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::Ice::ObjectProxySeq
IceDelegateM::Ice::Router::addProxies(const ::Ice::ObjectProxySeq& proxies, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Router__addProxies_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(proxies.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            ::Ice::__writeObjectProxySeq(__os, &proxies[0], &proxies[0] + proxies.size());
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::ObjectProxySeq __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::Ice::__readObjectProxySeq(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::ObjectPrx
IceDelegateD::Ice::Router::getClientProxy(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::ObjectPrx& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::Router* servant = dynamic_cast< ::Ice::Router*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getClientProxy(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::ObjectPrx& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__Router__getClientProxy_name, ::Ice::Nonmutating, __context);
    ::Ice::ObjectPrx __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPrx
IceDelegateD::Ice::Router::getServerProxy(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::ObjectPrx& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::Router* servant = dynamic_cast< ::Ice::Router*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getServerProxy(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::ObjectPrx& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__Router__getServerProxy_name, ::Ice::Nonmutating, __context);
    ::Ice::ObjectPrx __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::Ice::Router::addProxy(const ::Ice::ObjectPrx& proxy, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::ObjectPrx& proxy, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_proxy(proxy)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::Router* servant = dynamic_cast< ::Ice::Router*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->addProxy(_m_proxy, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::Ice::ObjectPrx& _m_proxy;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__Router__addProxy_name, ::Ice::Idempotent, __context);
    try
    {
        _DirectI __direct(proxy, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::ObjectProxySeq
IceDelegateD::Ice::Router::addProxies(const ::Ice::ObjectProxySeq& proxies, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::ObjectProxySeq& __result, const ::Ice::ObjectProxySeq& proxies, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_proxies(proxies)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::Router* servant = dynamic_cast< ::Ice::Router*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->addProxies(_m_proxies, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::ObjectProxySeq& _result;
        const ::Ice::ObjectProxySeq& _m_proxies;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__Router__addProxies_name, ::Ice::Idempotent, __context);
    ::Ice::ObjectProxySeq __result;
    try
    {
        _DirectI __direct(__result, proxies, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPtr
Ice::Router::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __Ice__Router_ids[2] =
{
    "::Ice::Object",
    "::Ice::Router"
};

bool
Ice::Router::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__Ice__Router_ids, __Ice__Router_ids + 2, _s);
}

::std::vector< ::std::string>
Ice::Router::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__Ice__Router_ids[0], &__Ice__Router_ids[2]);
}

const ::std::string&
Ice::Router::ice_id(const ::Ice::Current&) const
{
    return __Ice__Router_ids[1];
}

const ::std::string&
Ice::Router::ice_staticId()
{
    return __Ice__Router_ids[1];
}

::Ice::DispatchStatus
Ice::Router::___getClientProxy(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::ObjectPrx __ret = getClientProxy(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
Ice::Router::___getServerProxy(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::ObjectPrx __ret = getServerProxy(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
Ice::Router::___addProxy(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    __is->endReadEncaps();
    addProxy(proxy, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
Ice::Router::___addProxies(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::ObjectProxySeq proxies;
    ::Ice::__readObjectProxySeq(__is, proxies);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::ObjectProxySeq __ret = addProxies(proxies, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::Ice::__writeObjectProxySeq(__os, &__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

static ::std::string __Ice__Router_all[] =
{
    "addProxies",
    "addProxy",
    "getClientProxy",
    "getServerProxy",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
Ice::Router::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__Ice__Router_all, __Ice__Router_all + 8, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __Ice__Router_all)
    {
        case 0:
        {
            return ___addProxies(in, current);
        }
        case 1:
        {
            return ___addProxy(in, current);
        }
        case 2:
        {
            return ___getClientProxy(in, current);
        }
        case 3:
        {
            return ___getServerProxy(in, current);
        }
        case 4:
        {
            return ___ice_id(in, current);
        }
        case 5:
        {
            return ___ice_ids(in, current);
        }
        case 6:
        {
            return ___ice_isA(in, current);
        }
        case 7:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
Ice::Router::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
Ice::Router::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
Ice::Router::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::Router was not generated with stream support";
    throw ex;
}

void
Ice::Router::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::Router was not generated with stream support";
    throw ex;
}

void ICE_DECLSPEC_EXPORT 
Ice::__patch__RouterPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::Ice::RouterPtr* p = static_cast< ::Ice::RouterPtr*>(__addr);
    assert(p);
    *p = ::Ice::RouterPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::Ice::Router::ice_staticId(), v->ice_id());
    }
}

bool
Ice::operator==(const ::Ice::Router& l, const ::Ice::Router& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
Ice::operator<(const ::Ice::Router& l, const ::Ice::Router& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
