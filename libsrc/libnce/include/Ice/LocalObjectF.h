// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_LOCAL_OBJECT_F_H
#define ICE_LOCAL_OBJECT_F_H

#include <IceUtil/Shared.h>
#include <Ice/Config.h>

namespace Ice
{

class LocalObject;

}

namespace IceInternal
{

ICE_API IceUtil::Shared* upCast(::Ice::LocalObject*);

}

#include <Ice/Handle.h>

namespace Ice
{

typedef IceInternal::Handle< LocalObject > LocalObjectPtr;

}

#endif
