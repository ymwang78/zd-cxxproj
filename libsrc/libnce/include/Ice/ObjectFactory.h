// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `ObjectFactory.ice'

#ifndef __Ice_ObjectFactory_h__
#define __Ice_ObjectFactory_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/UndefSysMacros.h>

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

#ifndef ICE_API
#   ifdef ICE_API_EXPORTS
#       define ICE_API ICE_DECLSPEC_EXPORT
#   else
#       define ICE_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace Ice
{

class ObjectFactory;
ICE_API bool operator==(const ObjectFactory&, const ObjectFactory&);
ICE_API bool operator<(const ObjectFactory&, const ObjectFactory&);

}

namespace IceInternal
{

ICE_API ::Ice::LocalObject* upCast(::Ice::ObjectFactory*);

}

namespace Ice
{

typedef ::IceInternal::Handle< ::Ice::ObjectFactory> ObjectFactoryPtr;

}

namespace Ice
{

class ICE_API ObjectFactory : virtual public ::Ice::LocalObject
{
public:

    typedef ObjectFactoryPtr PointerType;
    

    virtual ::Ice::ObjectPtr create(const ::std::string&) = 0;

    virtual void destroy() = 0;
};

}

#endif
