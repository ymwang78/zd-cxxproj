// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Process.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/Process.h>
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

static const ::std::string __Ice__Process__shutdown_name = "shutdown";

static const ::std::string __Ice__Process__writeMessage_name = "writeMessage";

ICE_DECLSPEC_EXPORT ::Ice::Object* IceInternal::upCast(::Ice::Process* p) { return p; }
ICE_DECLSPEC_EXPORT ::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::Ice::Process* p) { return p; }

void
Ice::__read(::IceInternal::BasicStream* __is, ::Ice::ProcessPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::Ice::Process;
        v->__copyFrom(proxy);
    }
}

bool
Ice::AMI_Process_shutdown::__invoke(const ::Ice::ProcessPrx& __prx, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prepare(__prx, __Ice__Process__shutdown_name, ::Ice::Normal, __ctx);
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
Ice::AMI_Process_shutdown::__response(bool __ok)
{
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
        __is->skipEmptyEncaps();
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
Ice::AMI_Process_writeMessage::__invoke(const ::Ice::ProcessPrx& __prx, const ::std::string& message, ::Ice::Int fd, const ::Ice::Context* __ctx)
{
    __acquireCallback(__prx);
    try
    {
        __prepare(__prx, __Ice__Process__writeMessage_name, ::Ice::Normal, __ctx);
        __os->write(message);
        __os->write(fd);
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
Ice::AMI_Process_writeMessage::__response(bool __ok)
{
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
        __is->skipEmptyEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __finished(__ex);
        return;
    }
    ice_response();
    __releaseCallback();
}
#ifdef __SUNPRO_CC
class ICE_DECLSPEC_EXPORT IceProxy::Ice::Process;
#endif

void
IceProxy::Ice::Process::shutdown(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Process* __del = dynamic_cast< ::IceDelegate::Ice::Process*>(__delBase.get());
            __del->shutdown(__ctx);
            return;
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
IceProxy::Ice::Process::shutdown_async(const ::Ice::AMI_Process_shutdownPtr& __cb)
{
    return __cb->__invoke(this, 0);
}

bool
IceProxy::Ice::Process::shutdown_async(const ::Ice::AMI_Process_shutdownPtr& __cb, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, &__ctx);
}

void
IceProxy::Ice::Process::writeMessage(const ::std::string& message, ::Ice::Int fd, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::Ice::Process* __del = dynamic_cast< ::IceDelegate::Ice::Process*>(__delBase.get());
            __del->writeMessage(message, fd, __ctx);
            return;
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
IceProxy::Ice::Process::writeMessage_async(const ::Ice::AMI_Process_writeMessagePtr& __cb, const ::std::string& message, ::Ice::Int fd)
{
    return __cb->__invoke(this, message, fd, 0);
}

bool
IceProxy::Ice::Process::writeMessage_async(const ::Ice::AMI_Process_writeMessagePtr& __cb, const ::std::string& message, ::Ice::Int fd, const ::Ice::Context& __ctx)
{
    return __cb->__invoke(this, message, fd, &__ctx);
}

const ::std::string&
IceProxy::Ice::Process::ice_staticId()
{
    return ::Ice::Process::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::Ice::Process::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::Ice::Process);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::Ice::Process::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::Ice::Process);
}

::IceProxy::Ice::Object*
IceProxy::Ice::Process::__newInstance() const
{
    return new Process;
}

void
IceDelegateM::Ice::Process::shutdown(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Process__shutdown_name, ::Ice::Normal, __context);
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

void
IceDelegateM::Ice::Process::writeMessage(const ::std::string& message, ::Ice::Int fd, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Ice__Process__writeMessage_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(message);
        __os->write(fd);
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

void
IceDelegateD::Ice::Process::shutdown(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::Process* servant = dynamic_cast< ::Ice::Process*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->shutdown(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__Process__shutdown_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
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

void
IceDelegateD::Ice::Process::writeMessage(const ::std::string& message, ::Ice::Int fd, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& message, ::Ice::Int fd, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_message(message),
            _m_fd(fd)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Ice::Process* servant = dynamic_cast< ::Ice::Process*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->writeMessage(_m_message, _m_fd, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_message;
        ::Ice::Int _m_fd;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Ice__Process__writeMessage_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(message, fd, __current);
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

::Ice::ObjectPtr
Ice::Process::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __Ice__Process_ids[2] =
{
    "::Ice::Object",
    "::Ice::Process"
};

bool
Ice::Process::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__Ice__Process_ids, __Ice__Process_ids + 2, _s);
}

::std::vector< ::std::string>
Ice::Process::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__Ice__Process_ids[0], &__Ice__Process_ids[2]);
}

const ::std::string&
Ice::Process::ice_id(const ::Ice::Current&) const
{
    return __Ice__Process_ids[1];
}

const ::std::string&
Ice::Process::ice_staticId()
{
    return __Ice__Process_ids[1];
}

::Ice::DispatchStatus
Ice::Process::___shutdown(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    shutdown(__current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
Ice::Process::___writeMessage(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string message;
    ::Ice::Int fd;
    __is->read(message);
    __is->read(fd);
    __is->endReadEncaps();
    writeMessage(message, fd, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __Ice__Process_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "shutdown",
    "writeMessage"
};

::Ice::DispatchStatus
Ice::Process::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__Ice__Process_all, __Ice__Process_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __Ice__Process_all)
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
            return ___shutdown(in, current);
        }
        case 5:
        {
            return ___writeMessage(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
Ice::Process::__write(::IceInternal::BasicStream* __os) const
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
Ice::Process::__read(::IceInternal::BasicStream* __is, bool __rid)
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
Ice::Process::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::Process was not generated with stream support";
    throw ex;
}

void
Ice::Process::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Ice::Process was not generated with stream support";
    throw ex;
}

void ICE_DECLSPEC_EXPORT 
Ice::__patch__ProcessPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::Ice::ProcessPtr* p = static_cast< ::Ice::ProcessPtr*>(__addr);
    assert(p);
    *p = ::Ice::ProcessPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::Ice::Process::ice_staticId(), v->ice_id());
    }
}

bool
Ice::operator==(const ::Ice::Process& l, const ::Ice::Process& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
Ice::operator<(const ::Ice::Process& l, const ::Ice::Process& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
