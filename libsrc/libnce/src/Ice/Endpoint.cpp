// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Endpoint.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/Endpoint.h>
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

static const ::std::string __Ice__Endpoint__toString_name = "toString";

ICE_DECLSPEC_EXPORT ::Ice::LocalObject* IceInternal::upCast(::Ice::Endpoint* p) { return p; }

void
Ice::__write(::IceInternal::BasicStream* __os, ::Ice::EndpointSelectionType v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 2);
}

void
Ice::__read(::IceInternal::BasicStream* __is, ::Ice::EndpointSelectionType& v)
{
    ::Ice::Byte val;
    __is->read(val, 2);
    v = static_cast< ::Ice::EndpointSelectionType>(val);
}

bool
Ice::operator==(const ::Ice::Endpoint& l, const ::Ice::Endpoint& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) == static_cast<const ::Ice::LocalObject&>(r);
}


bool
Ice::operator<(const ::Ice::Endpoint& l, const ::Ice::Endpoint& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) < static_cast<const ::Ice::LocalObject&>(r);
}
