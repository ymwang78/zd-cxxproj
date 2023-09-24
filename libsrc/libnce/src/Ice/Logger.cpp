// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Logger.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/Logger.h>
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

static const ::std::string __Ice__Logger__print_name = "print";

static const ::std::string __Ice__Logger__trace_name = "trace";

static const ::std::string __Ice__Logger__warning_name = "warning";

static const ::std::string __Ice__Logger__error_name = "error";

ICE_DECLSPEC_EXPORT ::Ice::LocalObject* IceInternal::upCast(::Ice::Logger* p) { return p; }

bool
Ice::operator==(const ::Ice::Logger& l, const ::Ice::Logger& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) == static_cast<const ::Ice::LocalObject&>(r);
}


bool
Ice::operator<(const ::Ice::Logger& l, const ::Ice::Logger& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) < static_cast<const ::Ice::LocalObject&>(r);
}
