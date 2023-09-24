// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Ice/EndpointI.h>
#include <Ice/Instance.h>
#include <Ice/LocalException.h>
#include <Ice/Network.h>

using namespace std;
using namespace IceInternal;

Ice::LocalObject* IceInternal::upCast(EndpointI* p) { return p; }
IceUtil::Shared* IceInternal::upCast(EndpointHostResolver* p) { return p; }

vector<ConnectorPtr>
IceInternal::EndpointI::connectors(const vector<struct sockaddr_storage>& addrs) const
{
    //
    // This method must be extended by endpoints which use the EndpointHostResolver to create
    // connectors from IP addresses.
    //
    assert(false);
    return vector<ConnectorPtr>();
}

IceInternal::EndpointHostResolver::EndpointHostResolver(const InstancePtr& instance) :
    _instance(instance),
    _destroyed(false)
{
    start();
}

void
IceInternal::EndpointHostResolver::resolve(const string& host, int port, const EndpointIPtr& endpoint,
                                           const EndpointI_connectorsPtr& callback)
{ 
    //
    // Try to get the addresses without DNS lookup. If this doesn't work, we queue a resolve
    // entry and the thread will take care of getting the endpoint addresses.
    //
    try
    {
        vector<struct sockaddr_storage> addrs = getAddresses(host, port, _instance->protocolSupport(), false);
        if(!addrs.empty())
        {
            callback->connectors(endpoint->connectors(addrs));
            return;
        }
    }
    catch(const Ice::LocalException& ex)
    {
        callback->exception(ex);
        return;
    }

    Lock sync(*this);
    assert(!_destroyed);

    ResolveEntry entry;
    entry.host = host;
    entry.port = port;
    entry.endpoint = endpoint;
    entry.callback = callback;
    _queue.push_back(entry);
    notify();
}

void
IceInternal::EndpointHostResolver::destroy()
{
    Lock sync(*this);
    assert(!_destroyed);
    _destroyed = true;
    notify();
}

void
IceInternal::EndpointHostResolver::run()
{
    if(_instance->initializationData().threadHook)
    {
        _instance->initializationData().threadHook->start();
    }

    while(true)
    {
        ResolveEntry resolve;

        {
            Lock sync(*this);

            while(!_destroyed && _queue.empty())
            {
                wait();
            }

            if(_destroyed)
            {
                break;
            }

            resolve = _queue.front();
            _queue.pop_front();
        }

        try
        {
            resolve.callback->connectors(
                resolve.endpoint->connectors(
                    getAddresses(resolve.host, resolve.port, _instance->protocolSupport(), true)));
        }
        catch(const Ice::LocalException& ex)
        {
            resolve.callback->exception(ex);
        }
    }

    for(deque<ResolveEntry>::const_iterator p = _queue.begin(); p != _queue.end(); ++p)
    {
        p->callback->exception(Ice::CommunicatorDestroyedException(__FILE__, __LINE__));
    }
    _queue.clear();

    if(_instance->initializationData().threadHook)
    {
        _instance->initializationData().threadHook->stop();
    }
}
