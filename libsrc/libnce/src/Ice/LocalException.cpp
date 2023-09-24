// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `LocalException.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/LocalException.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
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

Ice::InitializationException::InitializationException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::InitializationException::InitializationException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    reason(__ice_reason)
{
}

Ice::InitializationException::~InitializationException() throw()
{
}

static const char* __Ice__InitializationException_name = "Ice::InitializationException";

::std::string
Ice::InitializationException::ice_name() const
{
    return __Ice__InitializationException_name;
}

::Ice::Exception*
Ice::InitializationException::ice_clone() const
{
    return new InitializationException(*this);
}

void
Ice::InitializationException::ice_throw() const
{
    throw *this;
}

Ice::PluginInitializationException::PluginInitializationException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::PluginInitializationException::PluginInitializationException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    reason(__ice_reason)
{
}

Ice::PluginInitializationException::~PluginInitializationException() throw()
{
}

static const char* __Ice__PluginInitializationException_name = "Ice::PluginInitializationException";

::std::string
Ice::PluginInitializationException::ice_name() const
{
    return __Ice__PluginInitializationException_name;
}

::Ice::Exception*
Ice::PluginInitializationException::ice_clone() const
{
    return new PluginInitializationException(*this);
}

void
Ice::PluginInitializationException::ice_throw() const
{
    throw *this;
}

Ice::CollocationOptimizationException::CollocationOptimizationException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::CollocationOptimizationException::~CollocationOptimizationException() throw()
{
}

static const char* __Ice__CollocationOptimizationException_name = "Ice::CollocationOptimizationException";

::std::string
Ice::CollocationOptimizationException::ice_name() const
{
    return __Ice__CollocationOptimizationException_name;
}

::Ice::Exception*
Ice::CollocationOptimizationException::ice_clone() const
{
    return new CollocationOptimizationException(*this);
}

void
Ice::CollocationOptimizationException::ice_throw() const
{
    throw *this;
}

Ice::AlreadyRegisteredException::AlreadyRegisteredException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::AlreadyRegisteredException::AlreadyRegisteredException(const char* __file, int __line, const ::std::string& __ice_kindOfObject, const ::std::string& __ice_id) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    kindOfObject(__ice_kindOfObject),
    id(__ice_id)
{
}

Ice::AlreadyRegisteredException::~AlreadyRegisteredException() throw()
{
}

static const char* __Ice__AlreadyRegisteredException_name = "Ice::AlreadyRegisteredException";

::std::string
Ice::AlreadyRegisteredException::ice_name() const
{
    return __Ice__AlreadyRegisteredException_name;
}

::Ice::Exception*
Ice::AlreadyRegisteredException::ice_clone() const
{
    return new AlreadyRegisteredException(*this);
}

void
Ice::AlreadyRegisteredException::ice_throw() const
{
    throw *this;
}

Ice::NotRegisteredException::NotRegisteredException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::NotRegisteredException::NotRegisteredException(const char* __file, int __line, const ::std::string& __ice_kindOfObject, const ::std::string& __ice_id) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    kindOfObject(__ice_kindOfObject),
    id(__ice_id)
{
}

Ice::NotRegisteredException::~NotRegisteredException() throw()
{
}

static const char* __Ice__NotRegisteredException_name = "Ice::NotRegisteredException";

::std::string
Ice::NotRegisteredException::ice_name() const
{
    return __Ice__NotRegisteredException_name;
}

::Ice::Exception*
Ice::NotRegisteredException::ice_clone() const
{
    return new NotRegisteredException(*this);
}

void
Ice::NotRegisteredException::ice_throw() const
{
    throw *this;
}

Ice::TwowayOnlyException::TwowayOnlyException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::TwowayOnlyException::TwowayOnlyException(const char* __file, int __line, const ::std::string& __ice_operation) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    operation(__ice_operation)
{
}

Ice::TwowayOnlyException::~TwowayOnlyException() throw()
{
}

static const char* __Ice__TwowayOnlyException_name = "Ice::TwowayOnlyException";

::std::string
Ice::TwowayOnlyException::ice_name() const
{
    return __Ice__TwowayOnlyException_name;
}

::Ice::Exception*
Ice::TwowayOnlyException::ice_clone() const
{
    return new TwowayOnlyException(*this);
}

void
Ice::TwowayOnlyException::ice_throw() const
{
    throw *this;
}

Ice::CloneNotImplementedException::CloneNotImplementedException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::CloneNotImplementedException::~CloneNotImplementedException() throw()
{
}

static const char* __Ice__CloneNotImplementedException_name = "Ice::CloneNotImplementedException";

::std::string
Ice::CloneNotImplementedException::ice_name() const
{
    return __Ice__CloneNotImplementedException_name;
}

::Ice::Exception*
Ice::CloneNotImplementedException::ice_clone() const
{
    return new CloneNotImplementedException(*this);
}

void
Ice::CloneNotImplementedException::ice_throw() const
{
    throw *this;
}

Ice::UnknownException::UnknownException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::UnknownException::UnknownException(const char* __file, int __line, const ::std::string& __ice_unknown) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    unknown(__ice_unknown)
{
}

Ice::UnknownException::~UnknownException() throw()
{
}

static const char* __Ice__UnknownException_name = "Ice::UnknownException";

::std::string
Ice::UnknownException::ice_name() const
{
    return __Ice__UnknownException_name;
}

::Ice::Exception*
Ice::UnknownException::ice_clone() const
{
    return new UnknownException(*this);
}

void
Ice::UnknownException::ice_throw() const
{
    throw *this;
}

Ice::UnknownLocalException::UnknownLocalException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UnknownException(__file, __line)
#else
    ::Ice::UnknownException(__file, __line)
#endif
{
}

Ice::UnknownLocalException::UnknownLocalException(const char* __file, int __line, const ::std::string& __ice_unknown) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UnknownException(__file, __line, __ice_unknown)
#else
    ::Ice::UnknownException(__file, __line, __ice_unknown)
#endif
{
}

Ice::UnknownLocalException::~UnknownLocalException() throw()
{
}

static const char* __Ice__UnknownLocalException_name = "Ice::UnknownLocalException";

::std::string
Ice::UnknownLocalException::ice_name() const
{
    return __Ice__UnknownLocalException_name;
}

::Ice::Exception*
Ice::UnknownLocalException::ice_clone() const
{
    return new UnknownLocalException(*this);
}

void
Ice::UnknownLocalException::ice_throw() const
{
    throw *this;
}

Ice::UnknownUserException::UnknownUserException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UnknownException(__file, __line)
#else
    ::Ice::UnknownException(__file, __line)
#endif
{
}

Ice::UnknownUserException::UnknownUserException(const char* __file, int __line, const ::std::string& __ice_unknown) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UnknownException(__file, __line, __ice_unknown)
#else
    ::Ice::UnknownException(__file, __line, __ice_unknown)
#endif
{
}

Ice::UnknownUserException::~UnknownUserException() throw()
{
}

static const char* __Ice__UnknownUserException_name = "Ice::UnknownUserException";

::std::string
Ice::UnknownUserException::ice_name() const
{
    return __Ice__UnknownUserException_name;
}

::Ice::Exception*
Ice::UnknownUserException::ice_clone() const
{
    return new UnknownUserException(*this);
}

void
Ice::UnknownUserException::ice_throw() const
{
    throw *this;
}

Ice::VersionMismatchException::VersionMismatchException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::VersionMismatchException::~VersionMismatchException() throw()
{
}

static const char* __Ice__VersionMismatchException_name = "Ice::VersionMismatchException";

::std::string
Ice::VersionMismatchException::ice_name() const
{
    return __Ice__VersionMismatchException_name;
}

::Ice::Exception*
Ice::VersionMismatchException::ice_clone() const
{
    return new VersionMismatchException(*this);
}

void
Ice::VersionMismatchException::ice_throw() const
{
    throw *this;
}

Ice::CommunicatorDestroyedException::CommunicatorDestroyedException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::CommunicatorDestroyedException::~CommunicatorDestroyedException() throw()
{
}

static const char* __Ice__CommunicatorDestroyedException_name = "Ice::CommunicatorDestroyedException";

::std::string
Ice::CommunicatorDestroyedException::ice_name() const
{
    return __Ice__CommunicatorDestroyedException_name;
}

::Ice::Exception*
Ice::CommunicatorDestroyedException::ice_clone() const
{
    return new CommunicatorDestroyedException(*this);
}

void
Ice::CommunicatorDestroyedException::ice_throw() const
{
    throw *this;
}

Ice::ObjectAdapterDeactivatedException::ObjectAdapterDeactivatedException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::ObjectAdapterDeactivatedException::ObjectAdapterDeactivatedException(const char* __file, int __line, const ::std::string& __ice_name) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    name(__ice_name)
{
}

Ice::ObjectAdapterDeactivatedException::~ObjectAdapterDeactivatedException() throw()
{
}

static const char* __Ice__ObjectAdapterDeactivatedException_name = "Ice::ObjectAdapterDeactivatedException";

::std::string
Ice::ObjectAdapterDeactivatedException::ice_name() const
{
    return __Ice__ObjectAdapterDeactivatedException_name;
}

::Ice::Exception*
Ice::ObjectAdapterDeactivatedException::ice_clone() const
{
    return new ObjectAdapterDeactivatedException(*this);
}

void
Ice::ObjectAdapterDeactivatedException::ice_throw() const
{
    throw *this;
}

Ice::ObjectAdapterIdInUseException::ObjectAdapterIdInUseException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::ObjectAdapterIdInUseException::ObjectAdapterIdInUseException(const char* __file, int __line, const ::std::string& __ice_id) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    id(__ice_id)
{
}

Ice::ObjectAdapterIdInUseException::~ObjectAdapterIdInUseException() throw()
{
}

static const char* __Ice__ObjectAdapterIdInUseException_name = "Ice::ObjectAdapterIdInUseException";

::std::string
Ice::ObjectAdapterIdInUseException::ice_name() const
{
    return __Ice__ObjectAdapterIdInUseException_name;
}

::Ice::Exception*
Ice::ObjectAdapterIdInUseException::ice_clone() const
{
    return new ObjectAdapterIdInUseException(*this);
}

void
Ice::ObjectAdapterIdInUseException::ice_throw() const
{
    throw *this;
}

Ice::NoEndpointException::NoEndpointException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::NoEndpointException::NoEndpointException(const char* __file, int __line, const ::std::string& __ice_proxy) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    proxy(__ice_proxy)
{
}

Ice::NoEndpointException::~NoEndpointException() throw()
{
}

static const char* __Ice__NoEndpointException_name = "Ice::NoEndpointException";

::std::string
Ice::NoEndpointException::ice_name() const
{
    return __Ice__NoEndpointException_name;
}

::Ice::Exception*
Ice::NoEndpointException::ice_clone() const
{
    return new NoEndpointException(*this);
}

void
Ice::NoEndpointException::ice_throw() const
{
    throw *this;
}

Ice::EndpointParseException::EndpointParseException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::EndpointParseException::EndpointParseException(const char* __file, int __line, const ::std::string& __ice_str) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    str(__ice_str)
{
}

Ice::EndpointParseException::~EndpointParseException() throw()
{
}

static const char* __Ice__EndpointParseException_name = "Ice::EndpointParseException";

::std::string
Ice::EndpointParseException::ice_name() const
{
    return __Ice__EndpointParseException_name;
}

::Ice::Exception*
Ice::EndpointParseException::ice_clone() const
{
    return new EndpointParseException(*this);
}

void
Ice::EndpointParseException::ice_throw() const
{
    throw *this;
}

Ice::EndpointSelectionTypeParseException::EndpointSelectionTypeParseException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::EndpointSelectionTypeParseException::EndpointSelectionTypeParseException(const char* __file, int __line, const ::std::string& __ice_str) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    str(__ice_str)
{
}

Ice::EndpointSelectionTypeParseException::~EndpointSelectionTypeParseException() throw()
{
}

static const char* __Ice__EndpointSelectionTypeParseException_name = "Ice::EndpointSelectionTypeParseException";

::std::string
Ice::EndpointSelectionTypeParseException::ice_name() const
{
    return __Ice__EndpointSelectionTypeParseException_name;
}

::Ice::Exception*
Ice::EndpointSelectionTypeParseException::ice_clone() const
{
    return new EndpointSelectionTypeParseException(*this);
}

void
Ice::EndpointSelectionTypeParseException::ice_throw() const
{
    throw *this;
}

Ice::IdentityParseException::IdentityParseException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::IdentityParseException::IdentityParseException(const char* __file, int __line, const ::std::string& __ice_str) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    str(__ice_str)
{
}

Ice::IdentityParseException::~IdentityParseException() throw()
{
}

static const char* __Ice__IdentityParseException_name = "Ice::IdentityParseException";

::std::string
Ice::IdentityParseException::ice_name() const
{
    return __Ice__IdentityParseException_name;
}

::Ice::Exception*
Ice::IdentityParseException::ice_clone() const
{
    return new IdentityParseException(*this);
}

void
Ice::IdentityParseException::ice_throw() const
{
    throw *this;
}

Ice::ProxyParseException::ProxyParseException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::ProxyParseException::ProxyParseException(const char* __file, int __line, const ::std::string& __ice_str) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    str(__ice_str)
{
}

Ice::ProxyParseException::~ProxyParseException() throw()
{
}

static const char* __Ice__ProxyParseException_name = "Ice::ProxyParseException";

::std::string
Ice::ProxyParseException::ice_name() const
{
    return __Ice__ProxyParseException_name;
}

::Ice::Exception*
Ice::ProxyParseException::ice_clone() const
{
    return new ProxyParseException(*this);
}

void
Ice::ProxyParseException::ice_throw() const
{
    throw *this;
}

Ice::IllegalIdentityException::IllegalIdentityException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::IllegalIdentityException::IllegalIdentityException(const char* __file, int __line, const ::Ice::Identity& __ice_id) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    id(__ice_id)
{
}

Ice::IllegalIdentityException::~IllegalIdentityException() throw()
{
}

static const char* __Ice__IllegalIdentityException_name = "Ice::IllegalIdentityException";

::std::string
Ice::IllegalIdentityException::ice_name() const
{
    return __Ice__IllegalIdentityException_name;
}

::Ice::Exception*
Ice::IllegalIdentityException::ice_clone() const
{
    return new IllegalIdentityException(*this);
}

void
Ice::IllegalIdentityException::ice_throw() const
{
    throw *this;
}

Ice::RequestFailedException::RequestFailedException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::RequestFailedException::RequestFailedException(const char* __file, int __line, const ::Ice::Identity& __ice_id, const ::std::string& __ice_facet, const ::std::string& __ice_operation) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    id(__ice_id),
    facet(__ice_facet),
    operation(__ice_operation)
{
}

Ice::RequestFailedException::~RequestFailedException() throw()
{
}

static const char* __Ice__RequestFailedException_name = "Ice::RequestFailedException";

::std::string
Ice::RequestFailedException::ice_name() const
{
    return __Ice__RequestFailedException_name;
}

::Ice::Exception*
Ice::RequestFailedException::ice_clone() const
{
    return new RequestFailedException(*this);
}

void
Ice::RequestFailedException::ice_throw() const
{
    throw *this;
}

Ice::ObjectNotExistException::ObjectNotExistException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    RequestFailedException(__file, __line)
#else
    ::Ice::RequestFailedException(__file, __line)
#endif
{
}

Ice::ObjectNotExistException::ObjectNotExistException(const char* __file, int __line, const ::Ice::Identity& __ice_id, const ::std::string& __ice_facet, const ::std::string& __ice_operation) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    RequestFailedException(__file, __line, __ice_id, __ice_facet, __ice_operation)
#else
    ::Ice::RequestFailedException(__file, __line, __ice_id, __ice_facet, __ice_operation)
#endif
{
}

Ice::ObjectNotExistException::~ObjectNotExistException() throw()
{
}

static const char* __Ice__ObjectNotExistException_name = "Ice::ObjectNotExistException";

::std::string
Ice::ObjectNotExistException::ice_name() const
{
    return __Ice__ObjectNotExistException_name;
}

::Ice::Exception*
Ice::ObjectNotExistException::ice_clone() const
{
    return new ObjectNotExistException(*this);
}

void
Ice::ObjectNotExistException::ice_throw() const
{
    throw *this;
}

Ice::FacetNotExistException::FacetNotExistException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    RequestFailedException(__file, __line)
#else
    ::Ice::RequestFailedException(__file, __line)
#endif
{
}

Ice::FacetNotExistException::FacetNotExistException(const char* __file, int __line, const ::Ice::Identity& __ice_id, const ::std::string& __ice_facet, const ::std::string& __ice_operation) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    RequestFailedException(__file, __line, __ice_id, __ice_facet, __ice_operation)
#else
    ::Ice::RequestFailedException(__file, __line, __ice_id, __ice_facet, __ice_operation)
#endif
{
}

Ice::FacetNotExistException::~FacetNotExistException() throw()
{
}

static const char* __Ice__FacetNotExistException_name = "Ice::FacetNotExistException";

::std::string
Ice::FacetNotExistException::ice_name() const
{
    return __Ice__FacetNotExistException_name;
}

::Ice::Exception*
Ice::FacetNotExistException::ice_clone() const
{
    return new FacetNotExistException(*this);
}

void
Ice::FacetNotExistException::ice_throw() const
{
    throw *this;
}

Ice::OperationNotExistException::OperationNotExistException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    RequestFailedException(__file, __line)
#else
    ::Ice::RequestFailedException(__file, __line)
#endif
{
}

Ice::OperationNotExistException::OperationNotExistException(const char* __file, int __line, const ::Ice::Identity& __ice_id, const ::std::string& __ice_facet, const ::std::string& __ice_operation) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    RequestFailedException(__file, __line, __ice_id, __ice_facet, __ice_operation)
#else
    ::Ice::RequestFailedException(__file, __line, __ice_id, __ice_facet, __ice_operation)
#endif
{
}

Ice::OperationNotExistException::~OperationNotExistException() throw()
{
}

static const char* __Ice__OperationNotExistException_name = "Ice::OperationNotExistException";

::std::string
Ice::OperationNotExistException::ice_name() const
{
    return __Ice__OperationNotExistException_name;
}

::Ice::Exception*
Ice::OperationNotExistException::ice_clone() const
{
    return new OperationNotExistException(*this);
}

void
Ice::OperationNotExistException::ice_throw() const
{
    throw *this;
}

Ice::SyscallException::SyscallException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::SyscallException::SyscallException(const char* __file, int __line, ::Ice::Int __ice_error) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    error(__ice_error)
{
}

Ice::SyscallException::~SyscallException() throw()
{
}

static const char* __Ice__SyscallException_name = "Ice::SyscallException";

::std::string
Ice::SyscallException::ice_name() const
{
    return __Ice__SyscallException_name;
}

::Ice::Exception*
Ice::SyscallException::ice_clone() const
{
    return new SyscallException(*this);
}

void
Ice::SyscallException::ice_throw() const
{
    throw *this;
}

Ice::SocketException::SocketException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    SyscallException(__file, __line)
#else
    ::Ice::SyscallException(__file, __line)
#endif
{
}

Ice::SocketException::SocketException(const char* __file, int __line, ::Ice::Int __ice_error) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    SyscallException(__file, __line, __ice_error)
#else
    ::Ice::SyscallException(__file, __line, __ice_error)
#endif
{
}

Ice::SocketException::~SocketException() throw()
{
}

static const char* __Ice__SocketException_name = "Ice::SocketException";

::std::string
Ice::SocketException::ice_name() const
{
    return __Ice__SocketException_name;
}

::Ice::Exception*
Ice::SocketException::ice_clone() const
{
    return new SocketException(*this);
}

void
Ice::SocketException::ice_throw() const
{
    throw *this;
}

Ice::FileException::FileException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    SyscallException(__file, __line)
#else
    ::Ice::SyscallException(__file, __line)
#endif
{
}

Ice::FileException::FileException(const char* __file, int __line, ::Ice::Int __ice_error, const ::std::string& __ice_path) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    SyscallException(__file, __line, __ice_error),
#else
    ::Ice::SyscallException(__file, __line, __ice_error),
#endif
    path(__ice_path)
{
}

Ice::FileException::~FileException() throw()
{
}

static const char* __Ice__FileException_name = "Ice::FileException";

::std::string
Ice::FileException::ice_name() const
{
    return __Ice__FileException_name;
}

::Ice::Exception*
Ice::FileException::ice_clone() const
{
    return new FileException(*this);
}

void
Ice::FileException::ice_throw() const
{
    throw *this;
}

Ice::ConnectFailedException::ConnectFailedException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    SocketException(__file, __line)
#else
    ::Ice::SocketException(__file, __line)
#endif
{
}

Ice::ConnectFailedException::ConnectFailedException(const char* __file, int __line, ::Ice::Int __ice_error) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    SocketException(__file, __line, __ice_error)
#else
    ::Ice::SocketException(__file, __line, __ice_error)
#endif
{
}

Ice::ConnectFailedException::~ConnectFailedException() throw()
{
}

static const char* __Ice__ConnectFailedException_name = "Ice::ConnectFailedException";

::std::string
Ice::ConnectFailedException::ice_name() const
{
    return __Ice__ConnectFailedException_name;
}

::Ice::Exception*
Ice::ConnectFailedException::ice_clone() const
{
    return new ConnectFailedException(*this);
}

void
Ice::ConnectFailedException::ice_throw() const
{
    throw *this;
}

Ice::ConnectionRefusedException::ConnectionRefusedException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ConnectFailedException(__file, __line)
#else
    ::Ice::ConnectFailedException(__file, __line)
#endif
{
}

Ice::ConnectionRefusedException::ConnectionRefusedException(const char* __file, int __line, ::Ice::Int __ice_error) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ConnectFailedException(__file, __line, __ice_error)
#else
    ::Ice::ConnectFailedException(__file, __line, __ice_error)
#endif
{
}

Ice::ConnectionRefusedException::~ConnectionRefusedException() throw()
{
}

static const char* __Ice__ConnectionRefusedException_name = "Ice::ConnectionRefusedException";

::std::string
Ice::ConnectionRefusedException::ice_name() const
{
    return __Ice__ConnectionRefusedException_name;
}

::Ice::Exception*
Ice::ConnectionRefusedException::ice_clone() const
{
    return new ConnectionRefusedException(*this);
}

void
Ice::ConnectionRefusedException::ice_throw() const
{
    throw *this;
}

Ice::ConnectionLostException::ConnectionLostException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    SocketException(__file, __line)
#else
    ::Ice::SocketException(__file, __line)
#endif
{
}

Ice::ConnectionLostException::ConnectionLostException(const char* __file, int __line, ::Ice::Int __ice_error) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    SocketException(__file, __line, __ice_error)
#else
    ::Ice::SocketException(__file, __line, __ice_error)
#endif
{
}

Ice::ConnectionLostException::~ConnectionLostException() throw()
{
}

static const char* __Ice__ConnectionLostException_name = "Ice::ConnectionLostException";

::std::string
Ice::ConnectionLostException::ice_name() const
{
    return __Ice__ConnectionLostException_name;
}

::Ice::Exception*
Ice::ConnectionLostException::ice_clone() const
{
    return new ConnectionLostException(*this);
}

void
Ice::ConnectionLostException::ice_throw() const
{
    throw *this;
}

Ice::DNSException::DNSException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::DNSException::DNSException(const char* __file, int __line, ::Ice::Int __ice_error, const ::std::string& __ice_host) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    error(__ice_error),
    host(__ice_host)
{
}

Ice::DNSException::~DNSException() throw()
{
}

static const char* __Ice__DNSException_name = "Ice::DNSException";

::std::string
Ice::DNSException::ice_name() const
{
    return __Ice__DNSException_name;
}

::Ice::Exception*
Ice::DNSException::ice_clone() const
{
    return new DNSException(*this);
}

void
Ice::DNSException::ice_throw() const
{
    throw *this;
}

Ice::TimeoutException::TimeoutException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::TimeoutException::~TimeoutException() throw()
{
}

static const char* __Ice__TimeoutException_name = "Ice::TimeoutException";

::std::string
Ice::TimeoutException::ice_name() const
{
    return __Ice__TimeoutException_name;
}

::Ice::Exception*
Ice::TimeoutException::ice_clone() const
{
    return new TimeoutException(*this);
}

void
Ice::TimeoutException::ice_throw() const
{
    throw *this;
}

Ice::ConnectTimeoutException::ConnectTimeoutException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    TimeoutException(__file, __line)
#else
    ::Ice::TimeoutException(__file, __line)
#endif
{
}

Ice::ConnectTimeoutException::~ConnectTimeoutException() throw()
{
}

static const char* __Ice__ConnectTimeoutException_name = "Ice::ConnectTimeoutException";

::std::string
Ice::ConnectTimeoutException::ice_name() const
{
    return __Ice__ConnectTimeoutException_name;
}

::Ice::Exception*
Ice::ConnectTimeoutException::ice_clone() const
{
    return new ConnectTimeoutException(*this);
}

void
Ice::ConnectTimeoutException::ice_throw() const
{
    throw *this;
}

Ice::CloseTimeoutException::CloseTimeoutException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    TimeoutException(__file, __line)
#else
    ::Ice::TimeoutException(__file, __line)
#endif
{
}

Ice::CloseTimeoutException::~CloseTimeoutException() throw()
{
}

static const char* __Ice__CloseTimeoutException_name = "Ice::CloseTimeoutException";

::std::string
Ice::CloseTimeoutException::ice_name() const
{
    return __Ice__CloseTimeoutException_name;
}

::Ice::Exception*
Ice::CloseTimeoutException::ice_clone() const
{
    return new CloseTimeoutException(*this);
}

void
Ice::CloseTimeoutException::ice_throw() const
{
    throw *this;
}

Ice::ConnectionTimeoutException::ConnectionTimeoutException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    TimeoutException(__file, __line)
#else
    ::Ice::TimeoutException(__file, __line)
#endif
{
}

Ice::ConnectionTimeoutException::~ConnectionTimeoutException() throw()
{
}

static const char* __Ice__ConnectionTimeoutException_name = "Ice::ConnectionTimeoutException";

::std::string
Ice::ConnectionTimeoutException::ice_name() const
{
    return __Ice__ConnectionTimeoutException_name;
}

::Ice::Exception*
Ice::ConnectionTimeoutException::ice_clone() const
{
    return new ConnectionTimeoutException(*this);
}

void
Ice::ConnectionTimeoutException::ice_throw() const
{
    throw *this;
}

Ice::ProtocolException::ProtocolException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::ProtocolException::ProtocolException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    reason(__ice_reason)
{
}

Ice::ProtocolException::~ProtocolException() throw()
{
}

static const char* __Ice__ProtocolException_name = "Ice::ProtocolException";

::std::string
Ice::ProtocolException::ice_name() const
{
    return __Ice__ProtocolException_name;
}

::Ice::Exception*
Ice::ProtocolException::ice_clone() const
{
    return new ProtocolException(*this);
}

void
Ice::ProtocolException::ice_throw() const
{
    throw *this;
}

Ice::BadMagicException::BadMagicException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::BadMagicException::BadMagicException(const char* __file, int __line, const ::std::string& __ice_reason, const ::Ice::ByteSeq& __ice_badMagic) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason),
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason),
#endif
    badMagic(__ice_badMagic)
{
}

Ice::BadMagicException::~BadMagicException() throw()
{
}

static const char* __Ice__BadMagicException_name = "Ice::BadMagicException";

::std::string
Ice::BadMagicException::ice_name() const
{
    return __Ice__BadMagicException_name;
}

::Ice::Exception*
Ice::BadMagicException::ice_clone() const
{
    return new BadMagicException(*this);
}

void
Ice::BadMagicException::ice_throw() const
{
    throw *this;
}

Ice::UnsupportedProtocolException::UnsupportedProtocolException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::UnsupportedProtocolException::UnsupportedProtocolException(const char* __file, int __line, const ::std::string& __ice_reason, ::Ice::Int __ice_badMajor, ::Ice::Int __ice_badMinor, ::Ice::Int __ice_major, ::Ice::Int __ice_minor) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason),
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason),
#endif
    badMajor(__ice_badMajor),
    badMinor(__ice_badMinor),
    major(__ice_major),
    minor(__ice_minor)
{
}

Ice::UnsupportedProtocolException::~UnsupportedProtocolException() throw()
{
}

static const char* __Ice__UnsupportedProtocolException_name = "Ice::UnsupportedProtocolException";

::std::string
Ice::UnsupportedProtocolException::ice_name() const
{
    return __Ice__UnsupportedProtocolException_name;
}

::Ice::Exception*
Ice::UnsupportedProtocolException::ice_clone() const
{
    return new UnsupportedProtocolException(*this);
}

void
Ice::UnsupportedProtocolException::ice_throw() const
{
    throw *this;
}

Ice::UnsupportedEncodingException::UnsupportedEncodingException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::UnsupportedEncodingException::UnsupportedEncodingException(const char* __file, int __line, const ::std::string& __ice_reason, ::Ice::Int __ice_badMajor, ::Ice::Int __ice_badMinor, ::Ice::Int __ice_major, ::Ice::Int __ice_minor) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason),
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason),
#endif
    badMajor(__ice_badMajor),
    badMinor(__ice_badMinor),
    major(__ice_major),
    minor(__ice_minor)
{
}

Ice::UnsupportedEncodingException::~UnsupportedEncodingException() throw()
{
}

static const char* __Ice__UnsupportedEncodingException_name = "Ice::UnsupportedEncodingException";

::std::string
Ice::UnsupportedEncodingException::ice_name() const
{
    return __Ice__UnsupportedEncodingException_name;
}

::Ice::Exception*
Ice::UnsupportedEncodingException::ice_clone() const
{
    return new UnsupportedEncodingException(*this);
}

void
Ice::UnsupportedEncodingException::ice_throw() const
{
    throw *this;
}

Ice::UnknownMessageException::UnknownMessageException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::UnknownMessageException::UnknownMessageException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::UnknownMessageException::~UnknownMessageException() throw()
{
}

static const char* __Ice__UnknownMessageException_name = "Ice::UnknownMessageException";

::std::string
Ice::UnknownMessageException::ice_name() const
{
    return __Ice__UnknownMessageException_name;
}

::Ice::Exception*
Ice::UnknownMessageException::ice_clone() const
{
    return new UnknownMessageException(*this);
}

void
Ice::UnknownMessageException::ice_throw() const
{
    throw *this;
}

Ice::ConnectionNotValidatedException::ConnectionNotValidatedException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::ConnectionNotValidatedException::ConnectionNotValidatedException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::ConnectionNotValidatedException::~ConnectionNotValidatedException() throw()
{
}

static const char* __Ice__ConnectionNotValidatedException_name = "Ice::ConnectionNotValidatedException";

::std::string
Ice::ConnectionNotValidatedException::ice_name() const
{
    return __Ice__ConnectionNotValidatedException_name;
}

::Ice::Exception*
Ice::ConnectionNotValidatedException::ice_clone() const
{
    return new ConnectionNotValidatedException(*this);
}

void
Ice::ConnectionNotValidatedException::ice_throw() const
{
    throw *this;
}

Ice::UnknownRequestIdException::UnknownRequestIdException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::UnknownRequestIdException::UnknownRequestIdException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::UnknownRequestIdException::~UnknownRequestIdException() throw()
{
}

static const char* __Ice__UnknownRequestIdException_name = "Ice::UnknownRequestIdException";

::std::string
Ice::UnknownRequestIdException::ice_name() const
{
    return __Ice__UnknownRequestIdException_name;
}

::Ice::Exception*
Ice::UnknownRequestIdException::ice_clone() const
{
    return new UnknownRequestIdException(*this);
}

void
Ice::UnknownRequestIdException::ice_throw() const
{
    throw *this;
}

Ice::UnknownReplyStatusException::UnknownReplyStatusException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::UnknownReplyStatusException::UnknownReplyStatusException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::UnknownReplyStatusException::~UnknownReplyStatusException() throw()
{
}

static const char* __Ice__UnknownReplyStatusException_name = "Ice::UnknownReplyStatusException";

::std::string
Ice::UnknownReplyStatusException::ice_name() const
{
    return __Ice__UnknownReplyStatusException_name;
}

::Ice::Exception*
Ice::UnknownReplyStatusException::ice_clone() const
{
    return new UnknownReplyStatusException(*this);
}

void
Ice::UnknownReplyStatusException::ice_throw() const
{
    throw *this;
}

Ice::CloseConnectionException::CloseConnectionException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::CloseConnectionException::CloseConnectionException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::CloseConnectionException::~CloseConnectionException() throw()
{
}

static const char* __Ice__CloseConnectionException_name = "Ice::CloseConnectionException";

::std::string
Ice::CloseConnectionException::ice_name() const
{
    return __Ice__CloseConnectionException_name;
}

::Ice::Exception*
Ice::CloseConnectionException::ice_clone() const
{
    return new CloseConnectionException(*this);
}

void
Ice::CloseConnectionException::ice_throw() const
{
    throw *this;
}

Ice::ForcedCloseConnectionException::ForcedCloseConnectionException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::ForcedCloseConnectionException::ForcedCloseConnectionException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::ForcedCloseConnectionException::~ForcedCloseConnectionException() throw()
{
}

static const char* __Ice__ForcedCloseConnectionException_name = "Ice::ForcedCloseConnectionException";

::std::string
Ice::ForcedCloseConnectionException::ice_name() const
{
    return __Ice__ForcedCloseConnectionException_name;
}

::Ice::Exception*
Ice::ForcedCloseConnectionException::ice_clone() const
{
    return new ForcedCloseConnectionException(*this);
}

void
Ice::ForcedCloseConnectionException::ice_throw() const
{
    throw *this;
}

Ice::IllegalMessageSizeException::IllegalMessageSizeException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::IllegalMessageSizeException::IllegalMessageSizeException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::IllegalMessageSizeException::~IllegalMessageSizeException() throw()
{
}

static const char* __Ice__IllegalMessageSizeException_name = "Ice::IllegalMessageSizeException";

::std::string
Ice::IllegalMessageSizeException::ice_name() const
{
    return __Ice__IllegalMessageSizeException_name;
}

::Ice::Exception*
Ice::IllegalMessageSizeException::ice_clone() const
{
    return new IllegalMessageSizeException(*this);
}

void
Ice::IllegalMessageSizeException::ice_throw() const
{
    throw *this;
}

Ice::CompressionException::CompressionException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::CompressionException::CompressionException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::CompressionException::~CompressionException() throw()
{
}

static const char* __Ice__CompressionException_name = "Ice::CompressionException";

::std::string
Ice::CompressionException::ice_name() const
{
    return __Ice__CompressionException_name;
}

::Ice::Exception*
Ice::CompressionException::ice_clone() const
{
    return new CompressionException(*this);
}

void
Ice::CompressionException::ice_throw() const
{
    throw *this;
}

Ice::DatagramLimitException::DatagramLimitException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::DatagramLimitException::DatagramLimitException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::DatagramLimitException::~DatagramLimitException() throw()
{
}

static const char* __Ice__DatagramLimitException_name = "Ice::DatagramLimitException";

::std::string
Ice::DatagramLimitException::ice_name() const
{
    return __Ice__DatagramLimitException_name;
}

::Ice::Exception*
Ice::DatagramLimitException::ice_clone() const
{
    return new DatagramLimitException(*this);
}

void
Ice::DatagramLimitException::ice_throw() const
{
    throw *this;
}

Ice::MarshalException::MarshalException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line)
#else
    ::Ice::ProtocolException(__file, __line)
#endif
{
}

Ice::MarshalException::MarshalException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ProtocolException(__file, __line, __ice_reason)
#else
    ::Ice::ProtocolException(__file, __line, __ice_reason)
#endif
{
}

Ice::MarshalException::~MarshalException() throw()
{
}

static const char* __Ice__MarshalException_name = "Ice::MarshalException";

::std::string
Ice::MarshalException::ice_name() const
{
    return __Ice__MarshalException_name;
}

::Ice::Exception*
Ice::MarshalException::ice_clone() const
{
    return new MarshalException(*this);
}

void
Ice::MarshalException::ice_throw() const
{
    throw *this;
}

Ice::ProxyUnmarshalException::ProxyUnmarshalException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::ProxyUnmarshalException::ProxyUnmarshalException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason)
#else
    ::Ice::MarshalException(__file, __line, __ice_reason)
#endif
{
}

Ice::ProxyUnmarshalException::~ProxyUnmarshalException() throw()
{
}

static const char* __Ice__ProxyUnmarshalException_name = "Ice::ProxyUnmarshalException";

::std::string
Ice::ProxyUnmarshalException::ice_name() const
{
    return __Ice__ProxyUnmarshalException_name;
}

::Ice::Exception*
Ice::ProxyUnmarshalException::ice_clone() const
{
    return new ProxyUnmarshalException(*this);
}

void
Ice::ProxyUnmarshalException::ice_throw() const
{
    throw *this;
}

Ice::UnmarshalOutOfBoundsException::UnmarshalOutOfBoundsException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::UnmarshalOutOfBoundsException::UnmarshalOutOfBoundsException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason)
#else
    ::Ice::MarshalException(__file, __line, __ice_reason)
#endif
{
}

Ice::UnmarshalOutOfBoundsException::~UnmarshalOutOfBoundsException() throw()
{
}

static const char* __Ice__UnmarshalOutOfBoundsException_name = "Ice::UnmarshalOutOfBoundsException";

::std::string
Ice::UnmarshalOutOfBoundsException::ice_name() const
{
    return __Ice__UnmarshalOutOfBoundsException_name;
}

::Ice::Exception*
Ice::UnmarshalOutOfBoundsException::ice_clone() const
{
    return new UnmarshalOutOfBoundsException(*this);
}

void
Ice::UnmarshalOutOfBoundsException::ice_throw() const
{
    throw *this;
}

Ice::IllegalIndirectionException::IllegalIndirectionException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::IllegalIndirectionException::IllegalIndirectionException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason)
#else
    ::Ice::MarshalException(__file, __line, __ice_reason)
#endif
{
}

Ice::IllegalIndirectionException::~IllegalIndirectionException() throw()
{
}

static const char* __Ice__IllegalIndirectionException_name = "Ice::IllegalIndirectionException";

::std::string
Ice::IllegalIndirectionException::ice_name() const
{
    return __Ice__IllegalIndirectionException_name;
}

::Ice::Exception*
Ice::IllegalIndirectionException::ice_clone() const
{
    return new IllegalIndirectionException(*this);
}

void
Ice::IllegalIndirectionException::ice_throw() const
{
    throw *this;
}

Ice::NoObjectFactoryException::NoObjectFactoryException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::NoObjectFactoryException::NoObjectFactoryException(const char* __file, int __line, const ::std::string& __ice_reason, const ::std::string& __ice_type) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason),
#else
    ::Ice::MarshalException(__file, __line, __ice_reason),
#endif
    type(__ice_type)
{
}

Ice::NoObjectFactoryException::~NoObjectFactoryException() throw()
{
}

static const char* __Ice__NoObjectFactoryException_name = "Ice::NoObjectFactoryException";

::std::string
Ice::NoObjectFactoryException::ice_name() const
{
    return __Ice__NoObjectFactoryException_name;
}

::Ice::Exception*
Ice::NoObjectFactoryException::ice_clone() const
{
    return new NoObjectFactoryException(*this);
}

void
Ice::NoObjectFactoryException::ice_throw() const
{
    throw *this;
}

Ice::UnexpectedObjectException::UnexpectedObjectException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::UnexpectedObjectException::UnexpectedObjectException(const char* __file, int __line, const ::std::string& __ice_reason, const ::std::string& __ice_type, const ::std::string& __ice_expectedType) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason),
#else
    ::Ice::MarshalException(__file, __line, __ice_reason),
#endif
    type(__ice_type),
    expectedType(__ice_expectedType)
{
}

Ice::UnexpectedObjectException::~UnexpectedObjectException() throw()
{
}

static const char* __Ice__UnexpectedObjectException_name = "Ice::UnexpectedObjectException";

::std::string
Ice::UnexpectedObjectException::ice_name() const
{
    return __Ice__UnexpectedObjectException_name;
}

::Ice::Exception*
Ice::UnexpectedObjectException::ice_clone() const
{
    return new UnexpectedObjectException(*this);
}

void
Ice::UnexpectedObjectException::ice_throw() const
{
    throw *this;
}

Ice::MemoryLimitException::MemoryLimitException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::MemoryLimitException::MemoryLimitException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason)
#else
    ::Ice::MarshalException(__file, __line, __ice_reason)
#endif
{
}

Ice::MemoryLimitException::~MemoryLimitException() throw()
{
}

static const char* __Ice__MemoryLimitException_name = "Ice::MemoryLimitException";

::std::string
Ice::MemoryLimitException::ice_name() const
{
    return __Ice__MemoryLimitException_name;
}

::Ice::Exception*
Ice::MemoryLimitException::ice_clone() const
{
    return new MemoryLimitException(*this);
}

void
Ice::MemoryLimitException::ice_throw() const
{
    throw *this;
}

Ice::StringConversionException::StringConversionException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::StringConversionException::StringConversionException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason)
#else
    ::Ice::MarshalException(__file, __line, __ice_reason)
#endif
{
}

Ice::StringConversionException::~StringConversionException() throw()
{
}

static const char* __Ice__StringConversionException_name = "Ice::StringConversionException";

::std::string
Ice::StringConversionException::ice_name() const
{
    return __Ice__StringConversionException_name;
}

::Ice::Exception*
Ice::StringConversionException::ice_clone() const
{
    return new StringConversionException(*this);
}

void
Ice::StringConversionException::ice_throw() const
{
    throw *this;
}

Ice::EncapsulationException::EncapsulationException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::EncapsulationException::EncapsulationException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason)
#else
    ::Ice::MarshalException(__file, __line, __ice_reason)
#endif
{
}

Ice::EncapsulationException::~EncapsulationException() throw()
{
}

static const char* __Ice__EncapsulationException_name = "Ice::EncapsulationException";

::std::string
Ice::EncapsulationException::ice_name() const
{
    return __Ice__EncapsulationException_name;
}

::Ice::Exception*
Ice::EncapsulationException::ice_clone() const
{
    return new EncapsulationException(*this);
}

void
Ice::EncapsulationException::ice_throw() const
{
    throw *this;
}

Ice::NegativeSizeException::NegativeSizeException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line)
#else
    ::Ice::MarshalException(__file, __line)
#endif
{
}

Ice::NegativeSizeException::NegativeSizeException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    MarshalException(__file, __line, __ice_reason)
#else
    ::Ice::MarshalException(__file, __line, __ice_reason)
#endif
{
}

Ice::NegativeSizeException::~NegativeSizeException() throw()
{
}

static const char* __Ice__NegativeSizeException_name = "Ice::NegativeSizeException";

::std::string
Ice::NegativeSizeException::ice_name() const
{
    return __Ice__NegativeSizeException_name;
}

::Ice::Exception*
Ice::NegativeSizeException::ice_clone() const
{
    return new NegativeSizeException(*this);
}

void
Ice::NegativeSizeException::ice_throw() const
{
    throw *this;
}

Ice::FeatureNotSupportedException::FeatureNotSupportedException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::FeatureNotSupportedException::FeatureNotSupportedException(const char* __file, int __line, const ::std::string& __ice_unsupportedFeature) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    unsupportedFeature(__ice_unsupportedFeature)
{
}

Ice::FeatureNotSupportedException::~FeatureNotSupportedException() throw()
{
}

static const char* __Ice__FeatureNotSupportedException_name = "Ice::FeatureNotSupportedException";

::std::string
Ice::FeatureNotSupportedException::ice_name() const
{
    return __Ice__FeatureNotSupportedException_name;
}

::Ice::Exception*
Ice::FeatureNotSupportedException::ice_clone() const
{
    return new FeatureNotSupportedException(*this);
}

void
Ice::FeatureNotSupportedException::ice_throw() const
{
    throw *this;
}

Ice::SecurityException::SecurityException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::SecurityException::SecurityException(const char* __file, int __line, const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line),
#else
    ::Ice::LocalException(__file, __line),
#endif
    reason(__ice_reason)
{
}

Ice::SecurityException::~SecurityException() throw()
{
}

static const char* __Ice__SecurityException_name = "Ice::SecurityException";

::std::string
Ice::SecurityException::ice_name() const
{
    return __Ice__SecurityException_name;
}

::Ice::Exception*
Ice::SecurityException::ice_clone() const
{
    return new SecurityException(*this);
}

void
Ice::SecurityException::ice_throw() const
{
    throw *this;
}

Ice::FixedProxyException::FixedProxyException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::FixedProxyException::~FixedProxyException() throw()
{
}

static const char* __Ice__FixedProxyException_name = "Ice::FixedProxyException";

::std::string
Ice::FixedProxyException::ice_name() const
{
    return __Ice__FixedProxyException_name;
}

::Ice::Exception*
Ice::FixedProxyException::ice_clone() const
{
    return new FixedProxyException(*this);
}

void
Ice::FixedProxyException::ice_throw() const
{
    throw *this;
}

Ice::ResponseSentException::ResponseSentException(const char* __file, int __line) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    LocalException(__file, __line)
#else
    ::Ice::LocalException(__file, __line)
#endif
{
}

Ice::ResponseSentException::~ResponseSentException() throw()
{
}

static const char* __Ice__ResponseSentException_name = "Ice::ResponseSentException";

::std::string
Ice::ResponseSentException::ice_name() const
{
    return __Ice__ResponseSentException_name;
}

::Ice::Exception*
Ice::ResponseSentException::ice_clone() const
{
    return new ResponseSentException(*this);
}

void
Ice::ResponseSentException::ice_throw() const
{
    throw *this;
}
