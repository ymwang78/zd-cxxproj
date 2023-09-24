// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Locator.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/Locator.h>
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

static const ::std::string __Ice__Locator__findObjectById_name = "findObjectById";

static const ::std::string __Ice__Locator__findAdapterById_name = "findAdapterById";

static const ::std::string __Ice__Locator__getRegistry_name = "getRegistry";

static const ::std::string __Ice__LocatorRegistry__setAdapterDirectProxy_name = "setAdapterDirectProxy";

static const ::std::string __Ice__LocatorRegistry__setReplicatedAdapterDirectProxy_name = "setReplicatedAdapterDirectProxy";

static const ::std::string __Ice__LocatorRegistry__setServerProcessProxy_name = "setServerProcessProxy";

ICE_DECLSPEC_EXPORT ::Ice::Object* IceInternal::upCast(::Ice::Locator* p) { return p; }
ICE_DECLSPEC_EXPORT ::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::Ice::Locator* p) { return p; }

ICE_DECLSPEC_EXPORT ::Ice::Object* IceInternal::upCast(::Ice::LocatorRegistry* p) { return p; }
ICE_DECLSPEC_EXPORT ::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::Ice::LocatorRegistry* p) { return p; }

void
Ice::__read(::IceInternal::BasicStream* __is, ::Ice::LocatorPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::Ice::Locator;
        v->__copyFrom(proxy);
    }
}

void
Ice::__read(::IceInternal::BasicStream* __is, ::Ice::LocatorRegistryPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::Ice::LocatorRegistry;
        v->__copyFrom(proxy);
    }
}

Ice::AdapterNotFoundException::~AdapterNotFoundException() throw()
{
}

static const char* __Ice__AdapterNotFoundException_name = "Ice::AdapterNotFoundException";

::std::string
Ice::AdapterNotFoundException::ice_name() const
{
    return __Ice__AdapterNotFoundException_name;
}

::Ice::Exception*
Ice::AdapterNotFoundException::ice_clone() const
{
    return new AdapterNotFoundException(*this);
}

void
Ice::AdapterNotFoundException::ice_throw() const
{
    throw *this;
}

void
Ice::AdapterNotFoundException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::Ice::AdapterNotFoundException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
}

void
Ice::AdapterNotFoundException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
}

void
Ice::AdapterNotFoundException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::AdapterNotFoundException was not generated with stream support";
    throw ex;
}

void
Ice::AdapterNotFoundException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::AdapterNotFoundException was not generated with stream support";
    throw ex;
}

struct __F__Ice__AdapterNotFoundException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::Ice::AdapterNotFoundException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__Ice__AdapterNotFoundException__Ptr = new __F__Ice__AdapterNotFoundException;

const ::IceInternal::UserExceptionFactoryPtr&
Ice::AdapterNotFoundException::ice_factory()
{
    return __F__Ice__AdapterNotFoundException__Ptr;
}

class __F__Ice__AdapterNotFoundException__Init
{
public:

    __F__Ice__AdapterNotFoundException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::Ice::AdapterNotFoundException", ::Ice::AdapterNotFoundException::ice_factory());
    }

    ~__F__Ice__AdapterNotFoundException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::Ice::AdapterNotFoundException");
    }
};

static __F__Ice__AdapterNotFoundException__Init __F__Ice__AdapterNotFoundException__i;

#ifdef __APPLE__
extern "C" { void __F__Ice__AdapterNotFoundException__initializer() {} }
#endif

Ice::InvalidReplicaGroupIdException::~InvalidReplicaGroupIdException() throw()
{
}

static const char* __Ice__InvalidReplicaGroupIdException_name = "Ice::InvalidReplicaGroupIdException";

::std::string
Ice::InvalidReplicaGroupIdException::ice_name() const
{
    return __Ice__InvalidReplicaGroupIdException_name;
}

::Ice::Exception*
Ice::InvalidReplicaGroupIdException::ice_clone() const
{
    return new InvalidReplicaGroupIdException(*this);
}

void
Ice::InvalidReplicaGroupIdException::ice_throw() const
{
    throw *this;
}

void
Ice::InvalidReplicaGroupIdException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::Ice::InvalidReplicaGroupIdException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
}

void
Ice::InvalidReplicaGroupIdException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
}

void
Ice::InvalidReplicaGroupIdException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::InvalidReplicaGroupIdException was not generated with stream support";
    throw ex;
}

void
Ice::InvalidReplicaGroupIdException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::InvalidReplicaGroupIdException was not generated with stream support";
    throw ex;
}

struct __F__Ice__InvalidReplicaGroupIdException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::Ice::InvalidReplicaGroupIdException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__Ice__InvalidReplicaGroupIdException__Ptr = new __F__Ice__InvalidReplicaGroupIdException;

const ::IceInternal::UserExceptionFactoryPtr&
Ice::InvalidReplicaGroupIdException::ice_factory()
{
    return __F__Ice__InvalidReplicaGroupIdException__Ptr;
}

class __F__Ice__InvalidReplicaGroupIdException__Init
{
public:

    __F__Ice__InvalidReplicaGroupIdException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::Ice::InvalidReplicaGroupIdException", ::Ice::InvalidReplicaGroupIdException::ice_factory());
    }

    ~__F__Ice__InvalidReplicaGroupIdException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::Ice::InvalidReplicaGroupIdException");
    }
};

static __F__Ice__InvalidReplicaGroupIdException__Init __F__Ice__InvalidReplicaGroupIdException__i;

#ifdef __APPLE__
extern "C" { void __F__Ice__InvalidReplicaGroupIdException__initializer() {} }
#endif

Ice::AdapterAlreadyActiveException::~AdapterAlreadyActiveException() throw()
{
}

static const char* __Ice__AdapterAlreadyActiveException_name = "Ice::AdapterAlreadyActiveException";

::std::string
Ice::AdapterAlreadyActiveException::ice_name() const
{
    return __Ice__AdapterAlreadyActiveException_name;
}

::Ice::Exception*
Ice::AdapterAlreadyActiveException::ice_clone() const
{
    return new AdapterAlreadyActiveException(*this);
}

void
Ice::AdapterAlreadyActiveException::ice_throw() const
{
    throw *this;
}

void
Ice::AdapterAlreadyActiveException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::Ice::AdapterAlreadyActiveException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
}

void
Ice::AdapterAlreadyActiveException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
}

void
Ice::AdapterAlreadyActiveException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::AdapterAlreadyActiveException was not generated with stream support";
    throw ex;
}

void
Ice::AdapterAlreadyActiveException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::AdapterAlreadyActiveException was not generated with stream support";
    throw ex;
}

struct __F__Ice__AdapterAlreadyActiveException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::Ice::AdapterAlreadyActiveException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__Ice__AdapterAlreadyActiveException__Ptr = new __F__Ice__AdapterAlreadyActiveException;

const ::IceInternal::UserExceptionFactoryPtr&
Ice::AdapterAlreadyActiveException::ice_factory()
{
    return __F__Ice__AdapterAlreadyActiveException__Ptr;
}

class __F__Ice__AdapterAlreadyActiveException__Init
{
public:

    __F__Ice__AdapterAlreadyActiveException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::Ice::AdapterAlreadyActiveException", ::Ice::AdapterAlreadyActiveException::ice_factory());
    }

    ~__F__Ice__AdapterAlreadyActiveException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::Ice::AdapterAlreadyActiveException");
    }
};

static __F__Ice__AdapterAlreadyActiveException__Init __F__Ice__AdapterAlreadyActiveException__i;

#ifdef __APPLE__
extern "C" { void __F__Ice__AdapterAlreadyActiveException__initializer() {} }
#endif

Ice::ObjectNotFoundException::~ObjectNotFoundException() throw()
{
}

static const char* __Ice__ObjectNotFoundException_name = "Ice::ObjectNotFoundException";

::std::string
Ice::ObjectNotFoundException::ice_name() const
{
    return __Ice__ObjectNotFoundException_name;
}

::Ice::Exception*
Ice::ObjectNotFoundException::ice_clone() const
{
    return new ObjectNotFoundException(*this);
}

void
Ice::ObjectNotFoundException::ice_throw() const
{
    throw *this;
}

void
Ice::ObjectNotFoundException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::Ice::ObjectNotFoundException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
}

void
Ice::ObjectNotFoundException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
}

void
Ice::ObjectNotFoundException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::ObjectNotFoundException was not generated with stream support";
    throw ex;
}

void
Ice::ObjectNotFoundException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::ObjectNotFoundException was not generated with stream support";
    throw ex;
}

struct __F__Ice__ObjectNotFoundException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::Ice::ObjectNotFoundException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__Ice__ObjectNotFoundException__Ptr = new __F__Ice__ObjectNotFoundException;

const ::IceInternal::UserExceptionFactoryPtr&
Ice::ObjectNotFoundException::ice_factory()
{
    return __F__Ice__ObjectNotFoundException__Ptr;
}

class __F__Ice__ObjectNotFoundException__Init
{
public:

    __F__Ice__ObjectNotFoundException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::Ice::ObjectNotFoundException", ::Ice::ObjectNotFoundException::ice_factory());
    }

    ~__F__Ice__ObjectNotFoundException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::Ice::ObjectNotFoundException");
    }
};

static __F__Ice__ObjectNotFoundException__Init __F__Ice__ObjectNotFoundException__i;

#ifdef __APPLE__
extern "C" { void __F__Ice__ObjectNotFoundException__initializer() {} }
#endif

Ice::ServerNotFoundException::~ServerNotFoundException() throw()
{
}

static const char* __Ice__ServerNotFoundException_name = "Ice::ServerNotFoundException";

::std::string
Ice::ServerNotFoundException::ice_name() const
{
    return __Ice__ServerNotFoundException_name;
}

::Ice::Exception*
Ice::ServerNotFoundException::ice_clone() const
{
    return new ServerNotFoundException(*this);
}

void
Ice::ServerNotFoundException::ice_throw() const
{
    throw *this;
}

void
Ice::ServerNotFoundException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::Ice::ServerNotFoundException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
}

void
Ice::ServerNotFoundException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
}

void
Ice::ServerNotFoundException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::ServerNotFoundException was not generated with stream support";
    throw ex;
}

void
Ice::ServerNotFoundException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception Ice::ServerNotFoundException was not generated with stream support";
    throw ex;
}

struct __F__Ice__ServerNotFoundException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::Ice::ServerNotFoundException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__Ice__ServerNotFoundException__Ptr = new __F__Ice__ServerNotFoundException;

const ::IceInternal::UserExceptionFactoryPtr&
Ice::ServerNotFoundException::ice_factory()
{
    return __F__Ice__ServerNotFoundException__Ptr;
}

class __F__Ice__ServerNotFoundException__Init
{
public:

    __F__Ice__ServerNotFoundException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::Ice::ServerNotFoundException", ::Ice::ServerNotFoundException::ice_factory());
    }

    ~__F__Ice__ServerNotFoundException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::Ice::ServerNotFoundException");
    }
};

static __F__Ice__ServerNotFoundException__Init __F__Ice__ServerNotFoundException__i;

#ifdef __APPLE__
extern "C" { void __F__Ice__ServerNotFoundException__initializer() {} }
#endif

bool
Ice::AMI_Locator_findObjectById::__invoke(const ::Ice::LocatorPrx& __prx, const ::Ice::Identity& id, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prx->__checkTwowayOnly("findObjectById");
        __prepare(__prx, __Ice__Locator__findObjectById_name, ::Ice::Nonmutating, __ctx);
        id.__write(__os);
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
Ice::AMI_Locator_findObjectById::__response(bool __ok)
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
            catch(const ::Ice::ObjectNotFoundException& __ex)
            {
                __exception(__ex);
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
Ice::AMI_Locator_findAdapterById::__invoke(const ::Ice::LocatorPrx& __prx, const ::std::string& id, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prx->__checkTwowayOnly("findAdapterById");
        __prepare(__prx, __Ice__Locator__findAdapterById_name, ::Ice::Nonmutating, __ctx);
        __os->write(id);
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
Ice::AMI_Locator_findAdapterById::__response(bool __ok)
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
            catch(const ::Ice::AdapterNotFoundException& __ex)
            {
                __exception(__ex);
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
Ice::AMI_LocatorRegistry_setAdapterDirectProxy::__invoke(const ::Ice::LocatorRegistryPrx& __prx, const ::std::string& id, const ::Ice::ObjectPrx& proxy, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prx->__checkTwowayOnly("setAdapterDirectProxy");
        __prepare(__prx, __Ice__LocatorRegistry__setAdapterDirectProxy_name, ::Ice::Idempotent, __ctx);
        __os->write(id);
        __os->write(proxy);
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
Ice::AMI_LocatorRegistry_setAdapterDirectProxy::__response(bool __ok)
{
    try
    {
        if(!__ok)
        {
            try
            {
                __throwUserException();
            }
            catch(const ::Ice::AdapterAlreadyActiveException& __ex)
            {
                __exception(__ex);
            }
            catch(const ::Ice::AdapterNotFoundException& __ex)
            {
                __exception(__ex);
            }
            catch(const ::Ice::UserException& __ex)
            {
                throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
            }
            return;
        }
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __finished(__ex);
        return;
    }
    ice_response();
    __releaseCallback();
}

bool
Ice::AMI_LocatorRegistry_setReplicatedAdapterDirectProxy::__invoke(const ::Ice::LocatorRegistryPrx& __prx, const ::std::string& adapterId, const ::std::string& replicaGroupId, const ::Ice::ObjectPrx& p, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prx->__checkTwowayOnly("setReplicatedAdapterDirectProxy");
        __prepare(__prx, __Ice__LocatorRegistry__setReplicatedAdapterDirectProxy_name, ::Ice::Idempotent, __ctx);
        __os->write(adapterId);
        __os->write(replicaGroupId);
        __os->write(p);
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
Ice::AMI_LocatorRegistry_setReplicatedAdapterDirectProxy::__response(bool __ok)
{
    try
    {
        if(!__ok)
        {
            try
            {
                __throwUserException();
            }
            catch(const ::Ice::AdapterAlreadyActiveException& __ex)
            {
                __exception(__ex);
            }
            catch(const ::Ice::AdapterNotFoundException& __ex)
            {
                __exception(__ex);
            }
            catch(const ::Ice::InvalidReplicaGroupIdException& __ex)
            {
                __exception(__ex);
            }
            catch(const ::Ice::UserException& __ex)
            {
                throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
            }
            return;
        }
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __finished(__ex);
        return;
    }
    ice_response();
    __releaseCallback();
}

IceAsync::Ice::AMD_Locator_findObjectById::AMD_Locator_findObjectById(::IceInternal::Incoming& in) :
    ::IceInternal::IncomingAsync(in)
{
}

void
IceAsync::Ice::AMD_Locator_findObjectById::ice_response(const ::Ice::ObjectPrx& __ret)
{
    if(__validateResponse(true))
    {
        try
        {
            ::IceInternal::BasicStream* __os = this->__os();
            __os->write(__ret);
        }
        catch(const ::Ice::Exception& __ex)
        {
            __exception(__ex);
            return;
        }
        __response(true);
    }
}

void
IceAsync::Ice::AMD_Locator_findObjectById::ice_exception(const ::std::exception& ex)
{
    if(const ::Ice::ObjectNotFoundException* __ex = dynamic_cast<const ::Ice::ObjectNotFoundException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else
    {
        if(__validateException(ex))
        {
            __exception(ex);
        }
    }
}

void
IceAsync::Ice::AMD_Locator_findObjectById::ice_exception()
{
    if(__validateException())
    {
        __exception();
    }
}

IceAsync::Ice::AMD_Locator_findAdapterById::AMD_Locator_findAdapterById(::IceInternal::Incoming& in) :
    ::IceInternal::IncomingAsync(in)
{
}

void
IceAsync::Ice::AMD_Locator_findAdapterById::ice_response(const ::Ice::ObjectPrx& __ret)
{
    if(__validateResponse(true))
    {
        try
        {
            ::IceInternal::BasicStream* __os = this->__os();
            __os->write(__ret);
        }
        catch(const ::Ice::Exception& __ex)
        {
            __exception(__ex);
            return;
        }
        __response(true);
    }
}

void
IceAsync::Ice::AMD_Locator_findAdapterById::ice_exception(const ::std::exception& ex)
{
    if(const ::Ice::AdapterNotFoundException* __ex = dynamic_cast<const ::Ice::AdapterNotFoundException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else
    {
        if(__validateException(ex))
        {
            __exception(ex);
        }
    }
}

void
IceAsync::Ice::AMD_Locator_findAdapterById::ice_exception()
{
    if(__validateException())
    {
        __exception();
    }
}

IceAsync::Ice::AMD_LocatorRegistry_setAdapterDirectProxy::AMD_LocatorRegistry_setAdapterDirectProxy(::IceInternal::Incoming& in) :
    ::IceInternal::IncomingAsync(in)
{
}

void
IceAsync::Ice::AMD_LocatorRegistry_setAdapterDirectProxy::ice_response()
{
    if(__validateResponse(true))
    {
        __response(true);
    }
}

void
IceAsync::Ice::AMD_LocatorRegistry_setAdapterDirectProxy::ice_exception(const ::std::exception& ex)
{
    if(const ::Ice::AdapterAlreadyActiveException* __ex = dynamic_cast<const ::Ice::AdapterAlreadyActiveException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else if(const ::Ice::AdapterNotFoundException* __ex = dynamic_cast<const ::Ice::AdapterNotFoundException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else
    {
        if(__validateException(ex))
        {
            __exception(ex);
        }
    }
}

void
IceAsync::Ice::AMD_LocatorRegistry_setAdapterDirectProxy::ice_exception()
{
    if(__validateException())
    {
        __exception();
    }
}

IceAsync::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxy::AMD_LocatorRegistry_setReplicatedAdapterDirectProxy(::IceInternal::Incoming& in) :
    ::IceInternal::IncomingAsync(in)
{
}

void
IceAsync::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxy::ice_response()
{
    if(__validateResponse(true))
    {
        __response(true);
    }
}

void
IceAsync::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxy::ice_exception(const ::std::exception& ex)
{
    if(const ::Ice::AdapterAlreadyActiveException* __ex = dynamic_cast<const ::Ice::AdapterAlreadyActiveException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else if(const ::Ice::AdapterNotFoundException* __ex = dynamic_cast<const ::Ice::AdapterNotFoundException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else if(const ::Ice::InvalidReplicaGroupIdException* __ex = dynamic_cast<const ::Ice::InvalidReplicaGroupIdException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else
    {
        if(__validateException(ex))
        {
            __exception(ex);
        }
    }
}

void
IceAsync::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxy::ice_exception()
{
    if(__validateException())
    {
        __exception();
    }
}

IceAsync::Ice::AMD_LocatorRegistry_setServerProcessProxy::AMD_LocatorRegistry_setServerProcessProxy(::IceInternal::Incoming& in) :
    ::IceInternal::IncomingAsync(in)
{
}

void
IceAsync::Ice::AMD_LocatorRegistry_setServerProcessProxy::ice_response()
{
    if(__validateResponse(true))
    {
        __response(true);
    }
}

void
IceAsync::Ice::AMD_LocatorRegistry_setServerProcessProxy::ice_exception(const ::std::exception& ex)
{
    if(const ::Ice::ServerNotFoundException* __ex = dynamic_cast<const ::Ice::ServerNotFoundException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else
    {
        if(__validateException(ex))
        {
            __exception(ex);
        }
    }
}

void
IceAsync::Ice::AMD_LocatorRegistry_setServerProcessProxy::ice_exception()
{
    if(__validateException())
    {
        __exception();
    }
}
#ifdef __SUNPRO_CC
class ICE_DECLSPEC_EXPORT IceProxy::Ice::Locator;
#endif

::Ice::ObjectPrx
IceProxy::Ice::Locator::findObjectById(const ::Ice::Identity& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__Locator__findObjectById_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Locator* __del = dynamic_cast< ::IceDelegate::Ice::Locator*>(__delBase.get());
            return __del->findObjectById(id, __ctx);
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
IceProxy::Ice::Locator::findObjectById_async(const ::Ice::AMI_Locator_findObjectByIdPtr& __cb, const ::Ice::Identity& id)
{
    return __cb->__invoke(this, id, 0);
}

bool
IceProxy::Ice::Locator::findObjectById_async(const ::Ice::AMI_Locator_findObjectByIdPtr& __cb, const ::Ice::Identity& id, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, id, &__ctx);
}

::Ice::ObjectPrx
IceProxy::Ice::Locator::findAdapterById(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__Locator__findAdapterById_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Locator* __del = dynamic_cast< ::IceDelegate::Ice::Locator*>(__delBase.get());
            return __del->findAdapterById(id, __ctx);
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
IceProxy::Ice::Locator::findAdapterById_async(const ::Ice::AMI_Locator_findAdapterByIdPtr& __cb, const ::std::string& id)
{
    return __cb->__invoke(this, id, 0);
}

bool
IceProxy::Ice::Locator::findAdapterById_async(const ::Ice::AMI_Locator_findAdapterByIdPtr& __cb, const ::std::string& id, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, id, &__ctx);
}

::Ice::LocatorRegistryPrx
IceProxy::Ice::Locator::getRegistry(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__Locator__getRegistry_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Locator* __del = dynamic_cast< ::IceDelegate::Ice::Locator*>(__delBase.get());
            return __del->getRegistry(__ctx);
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

const ::std::string&
IceProxy::Ice::Locator::ice_staticId()
{
    return ::Ice::Locator::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::Ice::Locator::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::Ice::Locator);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::Ice::Locator::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::Ice::Locator);
}

::IceProxy::Ice::Object*
IceProxy::Ice::Locator::__newInstance() const
{
    return new Locator;
}
#ifdef __SUNPRO_CC
class ICE_DECLSPEC_EXPORT IceProxy::Ice::LocatorRegistry;
#endif

void
IceProxy::Ice::LocatorRegistry::setAdapterDirectProxy(const ::std::string& id, const ::Ice::ObjectPrx& proxy, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__LocatorRegistry__setAdapterDirectProxy_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::LocatorRegistry* __del = dynamic_cast< ::IceDelegate::Ice::LocatorRegistry*>(__delBase.get());
            __del->setAdapterDirectProxy(id, proxy, __ctx);
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

bool
IceProxy::Ice::LocatorRegistry::setAdapterDirectProxy_async(const ::Ice::AMI_LocatorRegistry_setAdapterDirectProxyPtr& __cb, const ::std::string& id, const ::Ice::ObjectPrx& proxy)
{
    return __cb->__invoke(this, id, proxy, 0);
}

bool
IceProxy::Ice::LocatorRegistry::setAdapterDirectProxy_async(const ::Ice::AMI_LocatorRegistry_setAdapterDirectProxyPtr& __cb, const ::std::string& id, const ::Ice::ObjectPrx& proxy, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, id, proxy, &__ctx);
}

void
IceProxy::Ice::LocatorRegistry::setReplicatedAdapterDirectProxy(const ::std::string& adapterId, const ::std::string& replicaGroupId, const ::Ice::ObjectPrx& p, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__LocatorRegistry__setReplicatedAdapterDirectProxy_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::LocatorRegistry* __del = dynamic_cast< ::IceDelegate::Ice::LocatorRegistry*>(__delBase.get());
            __del->setReplicatedAdapterDirectProxy(adapterId, replicaGroupId, p, __ctx);
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

bool
IceProxy::Ice::LocatorRegistry::setReplicatedAdapterDirectProxy_async(const ::Ice::AMI_LocatorRegistry_setReplicatedAdapterDirectProxyPtr& __cb, const ::std::string& adapterId, const ::std::string& replicaGroupId, const ::Ice::ObjectPrx& p)
{
    return __cb->__invoke(this, adapterId, replicaGroupId, p, 0);
}

bool
IceProxy::Ice::LocatorRegistry::setReplicatedAdapterDirectProxy_async(const ::Ice::AMI_LocatorRegistry_setReplicatedAdapterDirectProxyPtr& __cb, const ::std::string& adapterId, const ::std::string& replicaGroupId, const ::Ice::ObjectPrx& p, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, adapterId, replicaGroupId, p, &__ctx);
}

void
IceProxy::Ice::LocatorRegistry::setServerProcessProxy(const ::std::string& id, const ::Ice::ProcessPrx& proxy, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__Ice__LocatorRegistry__setServerProcessProxy_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::LocatorRegistry* __del = dynamic_cast< ::IceDelegate::Ice::LocatorRegistry*>(__delBase.get());
            __del->setServerProcessProxy(id, proxy, __ctx);
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

const ::std::string&
IceProxy::Ice::LocatorRegistry::ice_staticId()
{
    return ::Ice::LocatorRegistry::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::Ice::LocatorRegistry::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::Ice::LocatorRegistry);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::Ice::LocatorRegistry::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::Ice::LocatorRegistry);
}

::IceProxy::Ice::Object*
IceProxy::Ice::LocatorRegistry::__newInstance() const
{
    return new LocatorRegistry;
}

::Ice::ObjectPrx
IceDelegateM::Ice::Locator::findObjectById(const ::Ice::Identity& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Locator__findObjectById_name, ::Ice::Nonmutating, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        id.__write(__os);
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
            catch(const ::Ice::ObjectNotFoundException&)
            {
                throw;
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
IceDelegateM::Ice::Locator::findAdapterById(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Locator__findAdapterById_name, ::Ice::Nonmutating, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
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
            catch(const ::Ice::AdapterNotFoundException&)
            {
                throw;
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

::Ice::LocatorRegistryPrx
IceDelegateM::Ice::Locator::getRegistry(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Locator__getRegistry_name, ::Ice::Nonmutating, __context);
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
        ::Ice::LocatorRegistryPrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::Ice::__read(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::Ice::LocatorRegistry::setAdapterDirectProxy(const ::std::string& id, const ::Ice::ObjectPrx& proxy, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__LocatorRegistry__setAdapterDirectProxy_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(proxy);
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
            catch(const ::Ice::AdapterAlreadyActiveException&)
            {
                throw;
            }
            catch(const ::Ice::AdapterNotFoundException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::Ice::LocatorRegistry::setReplicatedAdapterDirectProxy(const ::std::string& adapterId, const ::std::string& replicaGroupId, const ::Ice::ObjectPrx& p, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__LocatorRegistry__setReplicatedAdapterDirectProxy_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(adapterId);
        __os->write(replicaGroupId);
        __os->write(p);
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
            catch(const ::Ice::AdapterAlreadyActiveException&)
            {
                throw;
            }
            catch(const ::Ice::AdapterNotFoundException&)
            {
                throw;
            }
            catch(const ::Ice::InvalidReplicaGroupIdException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::Ice::LocatorRegistry::setServerProcessProxy(const ::std::string& id, const ::Ice::ProcessPrx& proxy, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__LocatorRegistry__setServerProcessProxy_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(proxy.get())));
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
            catch(const ::Ice::ServerNotFoundException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::ObjectPrx
IceDelegateD::Ice::Locator::findObjectById(const ::Ice::Identity&, const ::Ice::Context*)
{
    throw ::Ice::CollocationOptimizationException(__FILE__, __LINE__);
    return ::Ice::ObjectPrx(); // to avoid a warning with some compilers;
}

::Ice::ObjectPrx
IceDelegateD::Ice::Locator::findAdapterById(const ::std::string&, const ::Ice::Context*)
{
    throw ::Ice::CollocationOptimizationException(__FILE__, __LINE__);
    return ::Ice::ObjectPrx(); // to avoid a warning with some compilers;
}

::Ice::LocatorRegistryPrx
IceDelegateD::Ice::Locator::getRegistry(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::LocatorRegistryPrx& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::Locator* servant = dynamic_cast< ::Ice::Locator*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getRegistry(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::LocatorRegistryPrx& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__Locator__getRegistry_name, ::Ice::Nonmutating, __context);
    ::Ice::LocatorRegistryPrx __result;
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
IceDelegateD::Ice::LocatorRegistry::setAdapterDirectProxy(const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*)
{
    throw ::Ice::CollocationOptimizationException(__FILE__, __LINE__);
}

void
IceDelegateD::Ice::LocatorRegistry::setReplicatedAdapterDirectProxy(const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*)
{
    throw ::Ice::CollocationOptimizationException(__FILE__, __LINE__);
}

void
IceDelegateD::Ice::LocatorRegistry::setServerProcessProxy(const ::std::string&, const ::Ice::ProcessPrx&, const ::Ice::Context*)
{
    throw ::Ice::CollocationOptimizationException(__FILE__, __LINE__);
}

::Ice::ObjectPtr
Ice::Locator::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __Ice__Locator_ids[2] =
{
    "::Ice::Locator",
    "::Ice::Object"
};

bool
Ice::Locator::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__Ice__Locator_ids, __Ice__Locator_ids + 2, _s);
}

::std::vector< ::std::string>
Ice::Locator::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__Ice__Locator_ids[0], &__Ice__Locator_ids[2]);
}

const ::std::string&
Ice::Locator::ice_id(const ::Ice::Current&) const
{
    return __Ice__Locator_ids[0];
}

const ::std::string&
Ice::Locator::ice_staticId()
{
    return __Ice__Locator_ids[0];
}

::Ice::DispatchStatus
Ice::Locator::___findObjectById(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Identity id;
    id.__read(__is);
    __is->endReadEncaps();
    ::Ice::AMD_Locator_findObjectByIdPtr __cb = new IceAsync::Ice::AMD_Locator_findObjectById(__inS);
    try
    {
        findObjectById_async(__cb, id, __current);
    }
    catch(const ::std::exception& __ex)
    {
        __cb->ice_exception(__ex);
    }
    catch(...)
    {
        __cb->ice_exception();
    }
    return ::Ice::DispatchAsync;
}

::Ice::DispatchStatus
Ice::Locator::___findAdapterById(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::Ice::AMD_Locator_findAdapterByIdPtr __cb = new IceAsync::Ice::AMD_Locator_findAdapterById(__inS);
    try
    {
        findAdapterById_async(__cb, id, __current);
    }
    catch(const ::std::exception& __ex)
    {
        __cb->ice_exception(__ex);
    }
    catch(...)
    {
        __cb->ice_exception();
    }
    return ::Ice::DispatchAsync;
}

::Ice::DispatchStatus
Ice::Locator::___getRegistry(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::LocatorRegistryPrx __ret = getRegistry(__current);
    __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    return ::Ice::DispatchOK;
}

static ::std::string __Ice__Locator_all[] =
{
    "findAdapterById",
    "findObjectById",
    "getRegistry",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
Ice::Locator::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__Ice__Locator_all, __Ice__Locator_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __Ice__Locator_all)
    {
        case 0:
        {
            return ___findAdapterById(in, current);
        }
        case 1:
        {
            return ___findObjectById(in, current);
        }
        case 2:
        {
            return ___getRegistry(in, current);
        }
        case 3:
        {
            return ___ice_id(in, current);
        }
        case 4:
        {
            return ___ice_ids(in, current);
        }
        case 5:
        {
            return ___ice_isA(in, current);
        }
        case 6:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
Ice::Locator::__write(::IceInternal::BasicStream* __os) const
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
Ice::Locator::__read(::IceInternal::BasicStream* __is, bool __rid)
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
Ice::Locator::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::Locator was not generated with stream support";
    throw ex;
}

void
Ice::Locator::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::Locator was not generated with stream support";
    throw ex;
}

void ICE_DECLSPEC_EXPORT 
Ice::__patch__LocatorPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::Ice::LocatorPtr* p = static_cast< ::Ice::LocatorPtr*>(__addr);
    assert(p);
    *p = ::Ice::LocatorPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::Ice::Locator::ice_staticId(), v->ice_id());
    }
}

bool
Ice::operator==(const ::Ice::Locator& l, const ::Ice::Locator& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
Ice::operator<(const ::Ice::Locator& l, const ::Ice::Locator& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
Ice::LocatorRegistry::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __Ice__LocatorRegistry_ids[2] =
{
    "::Ice::LocatorRegistry",
    "::Ice::Object"
};

bool
Ice::LocatorRegistry::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__Ice__LocatorRegistry_ids, __Ice__LocatorRegistry_ids + 2, _s);
}

::std::vector< ::std::string>
Ice::LocatorRegistry::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__Ice__LocatorRegistry_ids[0], &__Ice__LocatorRegistry_ids[2]);
}

const ::std::string&
Ice::LocatorRegistry::ice_id(const ::Ice::Current&) const
{
    return __Ice__LocatorRegistry_ids[0];
}

const ::std::string&
Ice::LocatorRegistry::ice_staticId()
{
    return __Ice__LocatorRegistry_ids[0];
}

::Ice::DispatchStatus
Ice::LocatorRegistry::___setAdapterDirectProxy(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::Ice::ObjectPrx proxy;
    __is->read(id);
    __is->read(proxy);
    __is->endReadEncaps();
    ::Ice::AMD_LocatorRegistry_setAdapterDirectProxyPtr __cb = new IceAsync::Ice::AMD_LocatorRegistry_setAdapterDirectProxy(__inS);
    try
    {
        setAdapterDirectProxy_async(__cb, id, proxy, __current);
    }
    catch(const ::std::exception& __ex)
    {
        __cb->ice_exception(__ex);
    }
    catch(...)
    {
        __cb->ice_exception();
    }
    return ::Ice::DispatchAsync;
}

::Ice::DispatchStatus
Ice::LocatorRegistry::___setReplicatedAdapterDirectProxy(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string adapterId;
    ::std::string replicaGroupId;
    ::Ice::ObjectPrx p;
    __is->read(adapterId);
    __is->read(replicaGroupId);
    __is->read(p);
    __is->endReadEncaps();
    ::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr __cb = new IceAsync::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxy(__inS);
    try
    {
        setReplicatedAdapterDirectProxy_async(__cb, adapterId, replicaGroupId, p, __current);
    }
    catch(const ::std::exception& __ex)
    {
        __cb->ice_exception(__ex);
    }
    catch(...)
    {
        __cb->ice_exception();
    }
    return ::Ice::DispatchAsync;
}

::Ice::DispatchStatus
Ice::LocatorRegistry::___setServerProcessProxy(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::Ice::ProcessPrx proxy;
    __is->read(id);
    ::Ice::__read(__is, proxy);
    __is->endReadEncaps();
    ::Ice::AMD_LocatorRegistry_setServerProcessProxyPtr __cb = new IceAsync::Ice::AMD_LocatorRegistry_setServerProcessProxy(__inS);
    try
    {
        setServerProcessProxy_async(__cb, id, proxy, __current);
    }
    catch(const ::std::exception& __ex)
    {
        __cb->ice_exception(__ex);
    }
    catch(...)
    {
        __cb->ice_exception();
    }
    return ::Ice::DispatchAsync;
}

static ::std::string __Ice__LocatorRegistry_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setAdapterDirectProxy",
    "setReplicatedAdapterDirectProxy",
    "setServerProcessProxy"
};

::Ice::DispatchStatus
Ice::LocatorRegistry::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__Ice__LocatorRegistry_all, __Ice__LocatorRegistry_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __Ice__LocatorRegistry_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___setAdapterDirectProxy(in, current);
        }
        case 5:
        {
            return ___setReplicatedAdapterDirectProxy(in, current);
        }
        case 6:
        {
            return ___setServerProcessProxy(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
Ice::LocatorRegistry::__write(::IceInternal::BasicStream* __os) const
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
Ice::LocatorRegistry::__read(::IceInternal::BasicStream* __is, bool __rid)
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
Ice::LocatorRegistry::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::LocatorRegistry was not generated with stream support";
    throw ex;
}

void
Ice::LocatorRegistry::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::LocatorRegistry was not generated with stream support";
    throw ex;
}

void ICE_DECLSPEC_EXPORT 
Ice::__patch__LocatorRegistryPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::Ice::LocatorRegistryPtr* p = static_cast< ::Ice::LocatorRegistryPtr*>(__addr);
    assert(p);
    *p = ::Ice::LocatorRegistryPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::Ice::LocatorRegistry::ice_staticId(), v->ice_id());
    }
}

bool
Ice::operator==(const ::Ice::LocatorRegistry& l, const ::Ice::LocatorRegistry& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
Ice::operator<(const ::Ice::LocatorRegistry& l, const ::Ice::LocatorRegistry& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
