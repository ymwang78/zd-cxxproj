// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Plugin.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/Plugin.h>
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

static const ::std::string __Ice__Plugin__initialize_name = "initialize";

static const ::std::string __Ice__Plugin__destroy_name = "destroy";

static const ::std::string __Ice__PluginManager__initializePlugins_name = "initializePlugins";

static const ::std::string __Ice__PluginManager__getPlugin_name = "getPlugin";

static const ::std::string __Ice__PluginManager__addPlugin_name = "addPlugin";

static const ::std::string __Ice__PluginManager__destroy_name = "destroy";

ICE_DECLSPEC_EXPORT ::Ice::LocalObject* IceInternal::upCast(::Ice::Plugin* p) { return p; }

ICE_DECLSPEC_EXPORT ::Ice::LocalObject* IceInternal::upCast(::Ice::PluginManager* p) { return p; }

bool
Ice::operator==(const ::Ice::Plugin& l, const ::Ice::Plugin& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) == static_cast<const ::Ice::LocalObject&>(r);
}


bool
Ice::operator<(const ::Ice::Plugin& l, const ::Ice::Plugin& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) < static_cast<const ::Ice::LocalObject&>(r);
}

bool
Ice::operator==(const ::Ice::PluginManager& l, const ::Ice::PluginManager& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) == static_cast<const ::Ice::LocalObject&>(r);
}


bool
Ice::operator<(const ::Ice::PluginManager& l, const ::Ice::PluginManager& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) < static_cast<const ::Ice::LocalObject&>(r);
}
