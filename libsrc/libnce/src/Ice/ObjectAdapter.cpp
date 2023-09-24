// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `ObjectAdapter.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/ObjectAdapter.h>
#include <Ice/BasicStream.h>
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

static const ::std::string __Ice__ObjectAdapter__getName_name = "getName";

static const ::std::string __Ice__ObjectAdapter__getCommunicator_name = "getCommunicator";

static const ::std::string __Ice__ObjectAdapter__activate_name = "activate";

static const ::std::string __Ice__ObjectAdapter__hold_name = "hold";

static const ::std::string __Ice__ObjectAdapter__waitForHold_name = "waitForHold";

static const ::std::string __Ice__ObjectAdapter__deactivate_name = "deactivate";

static const ::std::string __Ice__ObjectAdapter__waitForDeactivate_name = "waitForDeactivate";

static const ::std::string __Ice__ObjectAdapter__isDeactivated_name = "isDeactivated";

static const ::std::string __Ice__ObjectAdapter__destroy_name = "destroy";

static const ::std::string __Ice__ObjectAdapter__add_name = "add";

static const ::std::string __Ice__ObjectAdapter__addFacet_name = "addFacet";

static const ::std::string __Ice__ObjectAdapter__addWithUUID_name = "addWithUUID";

static const ::std::string __Ice__ObjectAdapter__addFacetWithUUID_name = "addFacetWithUUID";

static const ::std::string __Ice__ObjectAdapter__remove_name = "remove";

static const ::std::string __Ice__ObjectAdapter__removeFacet_name = "removeFacet";

static const ::std::string __Ice__ObjectAdapter__removeAllFacets_name = "removeAllFacets";

static const ::std::string __Ice__ObjectAdapter__find_name = "find";

static const ::std::string __Ice__ObjectAdapter__findFacet_name = "findFacet";

static const ::std::string __Ice__ObjectAdapter__findAllFacets_name = "findAllFacets";

static const ::std::string __Ice__ObjectAdapter__findByProxy_name = "findByProxy";

static const ::std::string __Ice__ObjectAdapter__addServantLocator_name = "addServantLocator";

static const ::std::string __Ice__ObjectAdapter__findServantLocator_name = "findServantLocator";

static const ::std::string __Ice__ObjectAdapter__createProxy_name = "createProxy";

static const ::std::string __Ice__ObjectAdapter__createDirectProxy_name = "createDirectProxy";

static const ::std::string __Ice__ObjectAdapter__createIndirectProxy_name = "createIndirectProxy";

static const ::std::string __Ice__ObjectAdapter__setLocator_name = "setLocator";

static const ::std::string __Ice__ObjectAdapter__refreshPublishedEndpoints_name = "refreshPublishedEndpoints";

ICE_DECLSPEC_EXPORT ::Ice::LocalObject* IceInternal::upCast(::Ice::ObjectAdapter* p) { return p; }

bool
Ice::operator==(const ::Ice::ObjectAdapter& l, const ::Ice::ObjectAdapter& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) == static_cast<const ::Ice::LocalObject&>(r);
}


bool
Ice::operator<(const ::Ice::ObjectAdapter& l, const ::Ice::ObjectAdapter& r)
{
    return static_cast<const ::Ice::LocalObject&>(l) < static_cast<const ::Ice::LocalObject&>(r);
}
