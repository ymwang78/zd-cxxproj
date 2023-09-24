// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_PROXY_F_H
#define ICE_PROXY_F_H

#include <IceUtil/Shared.h>

#include <Ice/Config.h>


namespace IceProxy
{

namespace Ice
{

class Object;

}

}

namespace IceDelegate
{

namespace Ice
{

class Object;

}

}

namespace IceDelegateM
{

namespace Ice
{

class Object;

}

}

namespace IceDelegateD
{

namespace Ice
{

class Object;

}

}


namespace Ice
{

class LocalObject;

}

namespace IceInternal
{

ICE_API IceUtil::Shared* upCast(::Ice::LocalObject*);

}


namespace IceInternal
{

    ICE_API::IceProxy::Ice::Object*
        upCast(::IceProxy::Ice::Object* o);

    ICE_API::IceDelegate::Ice::Object*
        upCast(::IceDelegate::Ice::Object* o);

    ICE_API::IceDelegateD::Ice::Object*
        upCast(::IceDelegateD::Ice::Object* o);

    ICE_API::IceDelegateM::Ice::Object*
        upCast(::IceDelegateM::Ice::Object* o);

}

#include <Ice/ProxyHandle.h>

namespace Ice
{

typedef IceInternal::ProxyHandle< ::IceProxy::Ice::Object> ObjectPrx;

}


#endif
