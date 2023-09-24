// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Ice/TcpConnector.h>
#include <Ice/TcpTransceiver.h>
#include <Ice/TcpEndpointI.h>
#include <Ice/Instance.h>
#include <Ice/TraceLevels.h>
#include <Ice/LoggerUtil.h>
#include <Ice/Network.h>
#include <Ice/Exception.h>

using namespace std;
using namespace Ice;
using namespace IceInternal;

TransceiverPtr
IceInternal::TcpConnector::connect()
{
    if(_traceLevels->network >= 2)
    {
        Trace out(_logger, _traceLevels->networkCat);
        out << "trying to establish tcp connection to " << toString();
    }

    try
    {
        SOCKET fd = createSocket(false, _addr.ss_family);
        setBlock(fd, false);
        setTcpBufSize(fd, _instance->initializationData().properties, _logger);
        bool connected = doConnect(fd, _addr);
        if(connected)
        {
            if(_traceLevels->network >= 1)
            {
                Trace out(_logger, _traceLevels->networkCat);
                out << "tcp connection established\n" << fdToString(fd);
            }
        }
        return new TcpTransceiver(_instance, fd, connected);
    }
    catch(const Ice::LocalException& ex)
    {
        if(_traceLevels->network >= 2)
        {
            Trace out(_logger, _traceLevels->networkCat);
            out << "failed to establish tcp connection to " << toString() << "\n" << ex;
        }
        throw;
    }
}

Short
IceInternal::TcpConnector::type() const
{
    return TcpEndpointType;
}

string
IceInternal::TcpConnector::toString() const
{
    return addrToString(_addr);
}

bool
IceInternal::TcpConnector::operator==(const Connector& r) const
{
    const TcpConnector* p = dynamic_cast<const TcpConnector*>(&r);
    if(!p)
    {
        return false;
    }

    if(compareAddress(_addr, p->_addr) != 0)
    {
        return false;
    }

    if(_timeout != p->_timeout)
    {
        return false;
    }

    if(_connectionId != p->_connectionId)
    {
        return false;
    }

    return true;
}

bool
IceInternal::TcpConnector::operator!=(const Connector& r) const
{
    return !operator==(r);
}

bool
IceInternal::TcpConnector::operator<(const Connector& r) const
{
    const TcpConnector* p = dynamic_cast<const TcpConnector*>(&r);
    if(!p)
    {
        return type() < r.type();
    }

    if(_timeout < p->_timeout)
    {
        return true;
    }
    else if(p->_timeout < _timeout)
    {
        return false;
    }

    if(_connectionId < p->_connectionId)
    {
        return true;
    }
    else if(p->_connectionId < _connectionId)
    {
        return false;
    }
    return compareAddress(_addr, p->_addr) == -1;
}

IceInternal::TcpConnector::TcpConnector(const InstancePtr& instance, const struct sockaddr_storage& addr,
                                        Ice::Int timeout, const string& connectionId) :
    _instance(instance),
    _traceLevels(instance->traceLevels()),
    _logger(instance->initializationData().logger),
    _addr(addr),
    _timeout(timeout),
    _connectionId(connectionId)
{
}

IceInternal::TcpConnector::~TcpConnector()
{
}
