// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_TRANSCEIVER_H
#define ICE_TRANSCEIVER_H

#include <IceUtil/Shared.h>
#include <Ice/TransceiverF.h>
#include <Ice/SelectorF.h>

#ifdef _WIN32
#   include <winsock2.h>
typedef int ssize_t;
#else
#   define SOCKET int
#endif

namespace IceInternal
{

class Buffer;

class ICE_API Transceiver : public ::IceUtil::Shared
{
public:
    
    virtual SOCKET fd() = 0;
    virtual void close() = 0;
    virtual bool write(Buffer&) = 0;
    virtual bool read(Buffer&) = 0;
    virtual std::string type() const = 0;
    virtual std::string toString() const = 0;
    virtual SocketStatus initialize() = 0;
    virtual void checkSendSize(const Buffer&, size_t) = 0;
};

}

#endif
