// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_CONFIG_H
#define ICE_CONFIG_H

#include <IceUtil/Config.h>

//
// Some include files we need almost everywhere
//
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cstddef>
#include <map>

#if defined(_WIN32)
#   include <process.h>
#else
#   include <sys/types.h>
#   include <unistd.h>
#endif

#ifndef ICE_API
#   ifdef ICE_API_EXPORTS
#       define ICE_API ICE_DECLSPEC_EXPORT
#    else
#       define ICE_API ICE_DECLSPEC_IMPORT
#    endif
#endif

namespace Ice
{

typedef unsigned char Byte;
typedef short Short;
typedef int Int;
typedef IceUtil::Int64 Long;
typedef float Float;
typedef double Double;

}

// TODO: Should not be inline, this is not performance critical.
#ifdef _WIN32
inline int getSystemErrno() { return GetLastError(); }
#else
inline int getSystemErrno() { return errno; }
#endif

namespace IceUtil
{
    class Shared;
}


namespace IceDelegate
{
    namespace Ice {
        class Object;
    }
}

namespace IceDelegateM
{
    namespace Ice {
        class Object;
    }
}

namespace IceDelegateD
{
    namespace Ice {
        class Object;
    }
}

namespace IceProxy
{
    namespace Ice {
        class Object;
        class Process;
        class PropertiesAdmin;
        class LocatorRegistry;
        class Locator;
        class Router;
    }

    namespace IceStorm {
        class Topic;
        class TopicManager;
    }
}

namespace Ice
{
    class Object;

    class LocalObject;

    class Communicator;
    ICE_API bool operator==(const Communicator&, const Communicator&);
    ICE_API bool operator<(const Communicator&, const Communicator&);

    class Connection;
    ICE_API bool operator==(const Connection&, const Connection&);
    ICE_API bool operator<(const Connection&, const Connection&);

    class LocatorRegistry;
    ICE_API bool operator==(const LocatorRegistry&, const LocatorRegistry&);
    ICE_API bool operator<(const LocatorRegistry&, const LocatorRegistry&);

    class Locator;
    ICE_API bool operator==(const Locator&, const Locator&);
    ICE_API bool operator<(const Locator&, const Locator&);

    class ConnectionI;

    class Endpoint;
    ICE_API bool operator==(const Endpoint&, const Endpoint&);
    ICE_API bool operator<(const Endpoint&, const Endpoint&);

    class ImplicitContext;

    class LocalObject;

    class Logger;

    class ObjectAdapter;

    class ObjectFactory;

    class AMD_Object_ice_invoke;
    class AMD_Array_Object_ice_invoke;

    class AMI_Object_ice_invoke;
    class AMI_Array_Object_ice_invoke;
    class AMI_Object_ice_flushBatchRequests;

    class Plugin;
    class PluginManager;

    class Process;

    class Properties;
    class PropertiesAdmin;

    class Router;

    class ServantLocator;

    class Stats;
    ICE_API bool operator==(const Stats&, const Stats&);
    ICE_API bool operator<(const Stats&, const Stats&);

    class InputStream;
    class OutputStream;
}


namespace IceStorm
{
    class Topic;
    class TopicManager;
}

namespace IceInternal
{
    class Acceptor;
    ICE_API IceUtil::Shared* upCast(Acceptor*);

    class Connector;
    ICE_API IceUtil::Shared* upCast(Connector*);

    class DefaultsAndOverrides;
    IceUtil::Shared* upCast(DefaultsAndOverrides*);

    class EndpointFactoryManager;
    IceUtil::Shared* upCast(EndpointFactoryManager*);

    class EventHandler;
    ICE_API IceUtil::Shared* upCast(EventHandler*);

    class ThreadPoolWorkItem;
    ICE_API IceUtil::Shared* upCast(ThreadPoolWorkItem*);

    ICE_API ::Ice::Object* upCast(::IceStorm::Topic*);
    ICE_API ::IceProxy::Ice::Object* upCast(::IceProxy::IceStorm::Topic*);

    ICE_API ::Ice::Object* upCast(::IceStorm::TopicManager*);
    ICE_API ::IceProxy::Ice::Object* upCast(::IceProxy::IceStorm::TopicManager*);

    class LocatorManager;
    IceUtil::Shared* upCast(LocatorManager*);

    class LocatorInfo;
    IceUtil::Shared* upCast(LocatorInfo*);

    class LocatorTable;
    IceUtil::Shared* upCast(LocatorTable*);
    
    class ObjectFactoryManager;
    IceUtil::Shared* upCast(ObjectFactoryManager*);

    class ReferenceFactory;
    IceUtil::Shared* upCast(ReferenceFactory*);

    class RouterManager;
    IceUtil::Shared* upCast(RouterManager*);

    class RouterInfo;
    IceUtil::Shared* upCast(RouterInfo*);

    class SelectorThread;
    ICE_API IceUtil::Shared* upCast(SelectorThread*);

    class ThreadPool;
    ICE_API IceUtil::Shared* upCast(ThreadPool*);

    class TraceLevels;
    ICE_API IceUtil::Shared* upCast(TraceLevels*);

    class Transceiver;
    ICE_API IceUtil::Shared* upCast(Transceiver*);

    ICE_API ::Ice::LocalObject* upCast(::Ice::Communicator*);

    ICE_API ::Ice::LocalObject* upCast(::Ice::Connection*);

    class OutgoingConnectionFactory;
    ICE_API IceUtil::Shared* upCast(OutgoingConnectionFactory*);

    class IncomingConnectionFactory;
    ICE_API IceUtil::Shared* upCast(IncomingConnectionFactory*);

    ICE_API Ice::LocalObject* upCast(Ice::ConnectionI*);

    class ConnectionMonitor;
    ICE_API IceUtil::Shared* upCast(ConnectionMonitor*);

    class DynamicLibrary;
    ICE_API IceUtil::Shared* upCast(DynamicLibrary*);

    class DynamicLibraryList;
    ICE_API IceUtil::Shared* upCast(DynamicLibraryList*);

    ICE_API ::Ice::LocalObject* upCast(::Ice::Endpoint*);

    class EndpointFactory;
    ICE_API IceUtil::Shared* upCast(EndpointFactory*);

    class EndpointI;
    ICE_API Ice::LocalObject* upCast(EndpointI*);

    class EndpointHostResolver;
    ICE_API IceUtil::Shared* upCast(EndpointHostResolver*);

    ICE_API ::Ice::LocalObject* upCast(::Ice::ImplicitContext*);

    class IncomingAsync;
    ICE_API IceUtil::Shared* upCast(IncomingAsync*);

    ICE_API IceUtil::Shared* upCast(::Ice::AMD_Object_ice_invoke*);
    ICE_API IceUtil::Shared* upCast(::Ice::AMD_Array_Object_ice_invoke*);

    class Instance;
    ICE_API IceUtil::Shared* upCast(Instance*);

    ICE_API IceUtil::Shared* upCast(::Ice::LocalObject*);

    ICE_API ::Ice::Object* upCast(::Ice::LocatorRegistry*);
    ICE_API ::IceProxy::Ice::Object* upCast(::IceProxy::Ice::LocatorRegistry*);

    ICE_API ::Ice::Object* upCast(::Ice::Locator*);
    ICE_API ::IceProxy::Ice::Object* upCast(::IceProxy::Ice::Locator*);

    ICE_API ::Ice::LocalObject* upCast(::Ice::Logger*);

    ICE_API ::Ice::LocalObject* upCast(::Ice::ObjectAdapter*);

    class ObjectAdapterFactory;
    IceUtil::Shared* upCast(ObjectAdapterFactory*);

    class GCShared;
    ICE_API GCShared* upCast(::Ice::Object*);

    ICE_API ::Ice::LocalObject* upCast(::Ice::ObjectFactory*);

    class OutgoingAsync;
    ICE_API IceUtil::Shared* upCast(OutgoingAsync*);

    class OutgoingAsyncMessageCallback;
    ICE_API IceUtil::Shared* upCast(OutgoingAsyncMessageCallback*);

    class BatchOutgoingAsync;
    ICE_API IceUtil::Shared* upCast(BatchOutgoingAsync*);

    ICE_API IceUtil::Shared* upCast(::Ice::AMI_Object_ice_invoke*);
    ICE_API IceUtil::Shared* upCast(::Ice::AMI_Array_Object_ice_invoke*);
    ICE_API IceUtil::Shared* upCast(::Ice::AMI_Object_ice_flushBatchRequests*);

    ICE_API::Ice::LocalObject* upCast(::Ice::Plugin*);
    ICE_API::Ice::LocalObject* upCast(::Ice::PluginManager*);

    ICE_API::Ice::Object* upCast(::Ice::Process*);
    ICE_API::IceProxy::Ice::Object* upCast(::IceProxy::Ice::Process*);

    ICE_API::Ice::LocalObject* upCast(::Ice::Properties*);
    ICE_API::Ice::Object* upCast(::Ice::PropertiesAdmin*);
    ICE_API::IceProxy::Ice::Object* upCast(::IceProxy::Ice::PropertiesAdmin*);

    ICE_API::IceProxy::Ice::Object* upCast(::IceProxy::Ice::Locator*);
    ICE_API::IceProxy::Ice::Object* upCast(::IceProxy::Ice::Router*);

    ICE_API::IceProxy::Ice::Object* upCast(::IceProxy::Ice::Object* o);
    ICE_API::IceDelegate::Ice::Object* upCast(::IceDelegate::Ice::Object* o);
    ICE_API::IceDelegateD::Ice::Object* upCast(::IceDelegateD::Ice::Object* o);
    ICE_API::IceDelegateM::Ice::Object* upCast(::IceDelegateM::Ice::Object* o);

    class Reference;
    ICE_API IceUtil::Shared* upCast(Reference*);

    class FixedReference;
    ICE_API IceUtil::Shared* upCast(FixedReference*);

    class RoutableReference;
    ICE_API IceUtil::Shared* upCast(RoutableReference*);

    class RequestHandler;
    ICE_API IceUtil::Shared* upCast(RequestHandler*);

    ICE_API::Ice::Object* upCast(::Ice::Router*);
    ICE_API::IceProxy::Ice::Object* upCast(::IceProxy::Ice::Router*);

    ICE_API::Ice::LocalObject* upCast(::Ice::ServantLocator*);

    class ServantManager;
    ICE_API IceUtil::Shared* upCast(ServantManager*);

    ICE_API::Ice::LocalObject* upCast(::Ice::Stats*);

    ICE_API IceUtil::Shared* upCast(::Ice::InputStream*);
    ICE_API IceUtil::Shared* upCast(::Ice::OutputStream*);

}

#endif
