// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Current.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/Current.h>
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

void
Ice::__writeContext(::IceInternal::BasicStream* __os, const ::Ice::Context& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::Ice::Context::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        __os->write(p->second);
    }
}

void
Ice::__readContext(::IceInternal::BasicStream* __is, ::Ice::Context& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::std::string> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::Ice::Context::iterator __i = v.insert(v.end(), pair);
        __is->read(__i->second);
    }
}

void
Ice::__write(::IceInternal::BasicStream* __os, ::Ice::OperationMode v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 3);
}

void
Ice::__read(::IceInternal::BasicStream* __is, ::Ice::OperationMode& v)
{
    ::Ice::Byte val;
    __is->read(val, 3);
    v = static_cast< ::Ice::OperationMode>(val);
}

bool
Ice::Current::operator==(const Current& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(adapter != __rhs.adapter)
    {
        return false;
    }
    if(con != __rhs.con)
    {
        return false;
    }
    if(id != __rhs.id)
    {
        return false;
    }
    if(facet != __rhs.facet)
    {
        return false;
    }
    if(operation != __rhs.operation)
    {
        return false;
    }
    if(mode != __rhs.mode)
    {
        return false;
    }
    if(ctx != __rhs.ctx)
    {
        return false;
    }
    if(requestId != __rhs.requestId)
    {
        return false;
    }
    return true;
}

bool
Ice::Current::operator<(const Current& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(adapter < __rhs.adapter)
    {
        return true;
    }
    else if(__rhs.adapter < adapter)
    {
        return false;
    }
    if(con < __rhs.con)
    {
        return true;
    }
    else if(__rhs.con < con)
    {
        return false;
    }
    if(id < __rhs.id)
    {
        return true;
    }
    else if(__rhs.id < id)
    {
        return false;
    }
    if(facet < __rhs.facet)
    {
        return true;
    }
    else if(__rhs.facet < facet)
    {
        return false;
    }
    if(operation < __rhs.operation)
    {
        return true;
    }
    else if(__rhs.operation < operation)
    {
        return false;
    }
    if(mode < __rhs.mode)
    {
        return true;
    }
    else if(__rhs.mode < mode)
    {
        return false;
    }
    if(ctx < __rhs.ctx)
    {
        return true;
    }
    else if(__rhs.ctx < ctx)
    {
        return false;
    }
    if(requestId < __rhs.requestId)
    {
        return true;
    }
    else if(__rhs.requestId < requestId)
    {
        return false;
    }
    return false;
}
