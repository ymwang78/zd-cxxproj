// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `ImplicitContext.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/ImplicitContext.h>
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

static const ::std::string __Ice__ImplicitContext__getContext_name = "getContext";

static const ::std::string __Ice__ImplicitContext__setContext_name = "setContext";

static const ::std::string __Ice__ImplicitContext__containsKey_name = "containsKey";

static const ::std::string __Ice__ImplicitContext__get_name = "get";

static const ::std::string __Ice__ImplicitContext__put_name = "put";

static const ::std::string __Ice__ImplicitContext__remove_name = "remove";

ICE_DECLSPEC_EXPORT ::Ice::LocalObject* IceInternal::upCast(::Ice::ImplicitContext* p) { return p; }

bool
Ice::operator==(const ::Ice::ImplicitContext& l, const ::Ice::ImplicitContext& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) == static_cast<const ::Ice::LocalObject&>(r);
}


bool
Ice::operator<(const ::Ice::ImplicitContext& l, const ::Ice::ImplicitContext& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) < static_cast<const ::Ice::LocalObject&>(r);
}
