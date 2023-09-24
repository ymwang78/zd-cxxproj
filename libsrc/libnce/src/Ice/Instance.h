// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_INSTANCE_H
#define ICE_INSTANCE_H

#include <IceUtil/Shared.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/RecMutex.h>
#include <IceUtil/Timer.h>
#include <Ice/InstanceF.h>
#include <Ice/CommunicatorF.h>
#include <Ice/StatsF.h>
#include <Ice/TraceLevelsF.h>
#include <Ice/DefaultsAndOverridesF.h>
#include <Ice/RouterInfoF.h>
#include <Ice/LocatorInfoF.h>
#include <Ice/ReferenceFactoryF.h>
#include <Ice/ProxyFactoryF.h>
#include <Ice/ThreadPoolF.h>
#include <Ice/SelectorThreadF.h>
#include <Ice/ConnectionFactoryF.h>
#include <Ice/ConnectionMonitorF.h>
#include <Ice/ObjectFactoryManagerF.h>
#include <Ice/ObjectAdapterFactoryF.h>
#include <Ice/EndpointFactoryManagerF.h>
#include <Ice/DynamicLibraryF.h>
#include <Ice/PluginF.h>
#include <Ice/Initialize.h>
#include <Ice/SharedContext.h>
#include <Ice/ImplicitContextI.h>
#include <Ice/FacetMap.h>
#include <Ice/Process.h>
#include <list>

namespace Ice
{

class CommunicatorI;

}

namespace IceInternal
{

class Instance : public IceUtil::Shared, public IceUtil::RecMutex
{
public:

    bool destroyed() const;
    const Ice::InitializationData& initializationData() const { return _initData; }
    void stats(const Ice::StatsPtr&);
    TraceLevelsPtr traceLevels() const;
    DefaultsAndOverridesPtr defaultsAndOverrides() const;
    RouterManagerPtr routerManager() const;
    LocatorManagerPtr locatorManager() const;
    ReferenceFactoryPtr referenceFactory() const;
    ProxyFactoryPtr proxyFactory() const;
    OutgoingConnectionFactoryPtr outgoingConnectionFactory() const;
    ConnectionMonitorPtr connectionMonitor() const;
    ObjectFactoryManagerPtr servantFactoryManager() const;
    ObjectAdapterFactoryPtr objectAdapterFactory() const;
    ProtocolSupport protocolSupport() const;
    ThreadPoolPtr clientThreadPool();
    ThreadPoolPtr serverThreadPool();
    SelectorThreadPtr selectorThread();
    EndpointHostResolverPtr endpointHostResolver();
    IceUtil::TimerPtr timer();
    EndpointFactoryManagerPtr endpointFactoryManager() const;
    DynamicLibraryListPtr dynamicLibraryList() const;
    Ice::PluginManagerPtr pluginManager() const;
    size_t messageSizeMax() const { return _messageSizeMax; }
    Ice::Int clientACM() const;
    Ice::Int serverACM() const;
    void flushBatchRequests();
    void setDefaultContext(const ::Ice::Context&);
    SharedContextPtr getDefaultContext() const;
    Ice::Identity stringToIdentity(const std::string&) const;
    std::string identityToString(const Ice::Identity&) const;

    Ice::ObjectPrx getAdmin();
    void addAdminFacet(const Ice::ObjectPtr&, const std::string&);
    Ice::ObjectPtr removeAdminFacet(const std::string&);
    
    const Ice::ImplicitContextIPtr& getImplicitContext() const
    {
        return _implicitContext;
    }

    void setDefaultLocator(const Ice::LocatorPrx&);
    void setDefaultRouter(const Ice::RouterPrx&);

    void setStringConverter(const Ice::StringConverterPtr&);
    void setWstringConverter(const Ice::WstringConverterPtr&);
    void setLogger(const Ice::LoggerPtr&);

private:

    Instance(const Ice::CommunicatorPtr&, const Ice::InitializationData&);
    virtual ~Instance();
    void finishSetup(int&, char*[]);
    bool destroy();
    friend class Ice::CommunicatorI;

    enum State
    {
        StateActive,
        StateDestroyInProgress,
        StateDestroyed
    };
    State _state;
    Ice::InitializationData _initData;
    const TraceLevelsPtr _traceLevels; // Immutable, not reset by destroy().
    const DefaultsAndOverridesPtr _defaultsAndOverrides; // Immutable, not reset by destroy().
    const size_t _messageSizeMax; // Immutable, not reset by destroy().
    const Ice::Int _clientACM; // Immutable, not reset by destroy().
    const Ice::Int _serverACM; // Immutable, not reset by destroy().
    RouterManagerPtr _routerManager;
    LocatorManagerPtr _locatorManager;
    ReferenceFactoryPtr _referenceFactory;
    ProxyFactoryPtr _proxyFactory;
    OutgoingConnectionFactoryPtr _outgoingConnectionFactory;
    ConnectionMonitorPtr _connectionMonitor;
    ObjectFactoryManagerPtr _servantFactoryManager;
    ObjectAdapterFactoryPtr _objectAdapterFactory;
    ProtocolSupport _protocolSupport;
    ThreadPoolPtr _clientThreadPool;
    ThreadPoolPtr _serverThreadPool;
    SelectorThreadPtr _selectorThread;
    EndpointHostResolverPtr _endpointHostResolver;
    IceUtil::TimerPtr _timer;
    EndpointFactoryManagerPtr _endpointFactoryManager;
    DynamicLibraryListPtr _dynamicLibraryList;
    Ice::PluginManagerPtr _pluginManager;
    SharedContextPtr _defaultContext;
    const Ice::ImplicitContextIPtr _implicitContext;
    Ice::ObjectAdapterPtr _adminAdapter;
    Ice::FacetMap _adminFacets;
    Ice::Identity _adminIdentity;
    std::set<std::string> _adminFacetFilter;
};

class UTF8BufferI : public Ice::UTF8Buffer
{
public:

   UTF8BufferI();
   ~UTF8BufferI();

   Ice::Byte* getMoreBytes(size_t howMany, Ice::Byte* firstUnused);
   Ice::Byte* getBuffer();
   void reset();

private:

    Ice::Byte* _buffer;
    size_t _offset;
};


class ProcessI : public Ice::Process
{
public:
    
    ProcessI(const Ice::CommunicatorPtr&);
    
    virtual void shutdown(const Ice::Current&);
    virtual void writeMessage(const std::string&, Ice::Int, const Ice::Current&);
    
private:
    
    const Ice::CommunicatorPtr _communicator;
};

}

#endif
