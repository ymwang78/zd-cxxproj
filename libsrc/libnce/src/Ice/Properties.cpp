// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Properties.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/Properties.h>
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

static const ::std::string __Ice__Properties__getProperty_name = "getProperty";

static const ::std::string __Ice__Properties__getPropertyWithDefault_name = "getPropertyWithDefault";

static const ::std::string __Ice__Properties__getPropertyAsInt_name = "getPropertyAsInt";

static const ::std::string __Ice__Properties__getPropertyAsIntWithDefault_name = "getPropertyAsIntWithDefault";

static const ::std::string __Ice__Properties__getPropertyAsList_name = "getPropertyAsList";

static const ::std::string __Ice__Properties__getPropertyAsListWithDefault_name = "getPropertyAsListWithDefault";

static const ::std::string __Ice__Properties__getPropertiesForPrefix_name = "getPropertiesForPrefix";

static const ::std::string __Ice__Properties__setProperty_name = "setProperty";

static const ::std::string __Ice__Properties__getCommandLineOptions_name = "getCommandLineOptions";

static const ::std::string __Ice__Properties__parseCommandLineOptions_name = "parseCommandLineOptions";

static const ::std::string __Ice__Properties__parseIceCommandLineOptions_name = "parseIceCommandLineOptions";

static const ::std::string __Ice__Properties__load_name = "load";

static const ::std::string __Ice__Properties__clone_name = "clone";

static const ::std::string __Ice__PropertiesAdmin__getProperty_name = "getProperty";

static const ::std::string __Ice__PropertiesAdmin__getPropertiesForPrefix_name = "getPropertiesForPrefix";

ICE_DECLSPEC_EXPORT ::Ice::LocalObject* IceInternal::upCast(::Ice::Properties* p) { return p; }

ICE_DECLSPEC_EXPORT ::Ice::Object* IceInternal::upCast(::Ice::PropertiesAdmin* p) { return p; }
ICE_DECLSPEC_EXPORT ::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::Ice::PropertiesAdmin* p) { return p; }

void
Ice::__read(::IceInternal::BasicStream* __is, ::Ice::PropertiesAdminPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::Ice::PropertiesAdmin;
        v->__copyFrom(proxy);
    }
}

void
Ice::__writePropertyDict(::IceInternal::BasicStream* __os, const ::Ice::PropertyDict& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::Ice::PropertyDict::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        __os->write(p->second);
    }
}

void
Ice::__readPropertyDict(::IceInternal::BasicStream* __is, ::Ice::PropertyDict& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::std::string> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::Ice::PropertyDict::iterator __i = v.insert(v.end(), pair);
        __is->read(__i->second);
    }
}

bool
Ice::AMI_PropertiesAdmin_getProperty::__invoke(const ::Ice::PropertiesAdminPrx& __prx, const ::std::string& key, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prx->__checkTwowayOnly("getProperty");
        __prepare(__prx, __Ice__PropertiesAdmin__getProperty_name, ::Ice::Normal, __ctx);
        __os->write(key);
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
Ice::AMI_PropertiesAdmin_getProperty::__response(bool __ok)
{
    ::std::string __ret;
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
Ice::AMI_PropertiesAdmin_getPropertiesForPrefix::__invoke(const ::Ice::PropertiesAdminPrx& __prx, const ::std::string& prefix, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prx->__checkTwowayOnly("getPropertiesForPrefix");
        __prepare(__prx, __Ice__PropertiesAdmin__getPropertiesForPrefix_name, ::Ice::Normal, __ctx);
        __os->write(prefix);
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
Ice::AMI_PropertiesAdmin_getPropertiesForPrefix::__response(bool __ok)
{
    ::Ice::PropertyDict __ret;
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
        ::Ice::__readPropertyDict(__is, __ret);
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
class ICE_DECLSPEC_EXPORT IceProxy::Ice::PropertiesAdmin;
#endif

::std::string
IceProxy::Ice::PropertiesAdmin::getProperty(const ::std::string& key, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__PropertiesAdmin__getProperty_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::PropertiesAdmin* __del = dynamic_cast< ::IceDelegate::Ice::PropertiesAdmin*>(__delBase.get());
            return __del->getProperty(key, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::Ice::PropertiesAdmin::getProperty_async(const ::Ice::AMI_PropertiesAdmin_getPropertyPtr& __cb, const ::std::string& key)
{
    return __cb->__invoke(this, key, 0);
}

bool
IceProxy::Ice::PropertiesAdmin::getProperty_async(const ::Ice::AMI_PropertiesAdmin_getPropertyPtr& __cb, const ::std::string& key, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, key, &__ctx);
}

::Ice::PropertyDict
IceProxy::Ice::PropertiesAdmin::getPropertiesForPrefix(const ::std::string& prefix, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__PropertiesAdmin__getPropertiesForPrefix_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::PropertiesAdmin* __del = dynamic_cast< ::IceDelegate::Ice::PropertiesAdmin*>(__delBase.get());
            return __del->getPropertiesForPrefix(prefix, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::Ice::PropertiesAdmin::getPropertiesForPrefix_async(const ::Ice::AMI_PropertiesAdmin_getPropertiesForPrefixPtr& __cb, const ::std::string& prefix)
{
    return __cb->__invoke(this, prefix, 0);
}

bool
IceProxy::Ice::PropertiesAdmin::getPropertiesForPrefix_async(const ::Ice::AMI_PropertiesAdmin_getPropertiesForPrefixPtr& __cb, const ::std::string& prefix, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, prefix, &__ctx);
}

const ::std::string&
IceProxy::Ice::PropertiesAdmin::ice_staticId()
{
    return ::Ice::PropertiesAdmin::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::Ice::PropertiesAdmin::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::Ice::PropertiesAdmin);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::Ice::PropertiesAdmin::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::Ice::PropertiesAdmin);
}

::IceProxy::Ice::Object*
IceProxy::Ice::PropertiesAdmin::__newInstance() const
{
    return new PropertiesAdmin;
}

::std::string
IceDelegateM::Ice::PropertiesAdmin::getProperty(const ::std::string& key, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__PropertiesAdmin__getProperty_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(key);
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
        ::std::string __ret;
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

::Ice::PropertyDict
IceDelegateM::Ice::PropertiesAdmin::getPropertiesForPrefix(const ::std::string& prefix, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__PropertiesAdmin__getPropertiesForPrefix_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(prefix);
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
        ::Ice::PropertyDict __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::Ice::__readPropertyDict(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateD::Ice::PropertiesAdmin::getProperty(const ::std::string& key, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& key, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_key(key)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::PropertiesAdmin* servant = dynamic_cast< ::Ice::PropertiesAdmin*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getProperty(_m_key, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_key;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__PropertiesAdmin__getProperty_name, ::Ice::Normal, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, key, __current);
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

::Ice::PropertyDict
IceDelegateD::Ice::PropertiesAdmin::getPropertiesForPrefix(const ::std::string& prefix, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::PropertyDict& __result, const ::std::string& prefix, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_prefix(prefix)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::PropertiesAdmin* servant = dynamic_cast< ::Ice::PropertiesAdmin*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPropertiesForPrefix(_m_prefix, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::PropertyDict& _result;
        const ::std::string& _m_prefix;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__PropertiesAdmin__getPropertiesForPrefix_name, ::Ice::Normal, __context);
    ::Ice::PropertyDict __result;
    try
    {
        _DirectI __direct(__result, prefix, __current);
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

bool
Ice::operator==(const ::Ice::Properties& l, const ::Ice::Properties& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) == static_cast<const ::Ice::LocalObject&>(r);
}


bool
Ice::operator<(const ::Ice::Properties& l, const ::Ice::Properties& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) < static_cast<const ::Ice::LocalObject&>(r);
}

::Ice::ObjectPtr
Ice::PropertiesAdmin::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __Ice__PropertiesAdmin_ids[2] =
{
    "::Ice::Object",
    "::Ice::PropertiesAdmin"
};

bool
Ice::PropertiesAdmin::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__Ice__PropertiesAdmin_ids, __Ice__PropertiesAdmin_ids + 2, _s);
}

::std::vector< ::std::string>
Ice::PropertiesAdmin::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__Ice__PropertiesAdmin_ids[0], &__Ice__PropertiesAdmin_ids[2]);
}

const ::std::string&
Ice::PropertiesAdmin::ice_id(const ::Ice::Current&) const
{
    return __Ice__PropertiesAdmin_ids[1];
}

const ::std::string&
Ice::PropertiesAdmin::ice_staticId()
{
    return __Ice__PropertiesAdmin_ids[1];
}

::Ice::DispatchStatus
Ice::PropertiesAdmin::___getProperty(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string key;
    __is->read(key);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = getProperty(key, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
Ice::PropertiesAdmin::___getPropertiesForPrefix(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string prefix;
    __is->read(prefix);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::PropertyDict __ret = getPropertiesForPrefix(prefix, __current);
    ::Ice::__writePropertyDict(__os, __ret);
    return ::Ice::DispatchOK;
}

static ::std::string __Ice__PropertiesAdmin_all[] =
{
    "getPropertiesForPrefix",
    "getProperty",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
Ice::PropertiesAdmin::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__Ice__PropertiesAdmin_all, __Ice__PropertiesAdmin_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __Ice__PropertiesAdmin_all)
    {
        case 0:
        {
            return ___getPropertiesForPrefix(in, current);
        }
        case 1:
        {
            return ___getProperty(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
Ice::PropertiesAdmin::__write(::IceInternal::BasicStream* __os) const
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
Ice::PropertiesAdmin::__read(::IceInternal::BasicStream* __is, bool __rid)
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
Ice::PropertiesAdmin::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::PropertiesAdmin was not generated with stream support";
    throw ex;
}

void
Ice::PropertiesAdmin::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::PropertiesAdmin was not generated with stream support";
    throw ex;
}

void ICE_DECLSPEC_EXPORT 
Ice::__patch__PropertiesAdminPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::Ice::PropertiesAdminPtr* p = static_cast< ::Ice::PropertiesAdminPtr*>(__addr);
    assert(p);
    *p = ::Ice::PropertiesAdminPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::Ice::PropertiesAdmin::ice_staticId(), v->ice_id());
    }
}

bool
Ice::operator==(const ::Ice::PropertiesAdmin& l, const ::Ice::PropertiesAdmin& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
Ice::operator<(const ::Ice::PropertiesAdmin& l, const ::Ice::PropertiesAdmin& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
