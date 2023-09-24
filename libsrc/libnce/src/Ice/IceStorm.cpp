// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `IceStorm.ice'

#ifndef ICE_API_EXPORTS
#   define ICE_API_EXPORTS
#endif
#include <Ice/IceStorm.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
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

static const ::std::string __IceStorm__Topic__getName_name = "getName";

static const ::std::string __IceStorm__Topic__getPublisher_name = "getPublisher";

static const ::std::string __IceStorm__Topic__getNonReplicatedPublisher_name = "getNonReplicatedPublisher";

static const ::std::string __IceStorm__Topic__subscribe_name = "subscribe";

static const ::std::string __IceStorm__Topic__subscribeAndGetPublisher_name = "subscribeAndGetPublisher";

static const ::std::string __IceStorm__Topic__unsubscribe_name = "unsubscribe";

static const ::std::string __IceStorm__Topic__link_name = "link";

static const ::std::string __IceStorm__Topic__unlink_name = "unlink";

static const ::std::string __IceStorm__Topic__getLinkInfoSeq_name = "getLinkInfoSeq";

static const ::std::string __IceStorm__Topic__destroy_name = "destroy";

static const ::std::string __IceStorm__TopicManager__create_name = "create";

static const ::std::string __IceStorm__TopicManager__retrieve_name = "retrieve";

static const ::std::string __IceStorm__TopicManager__retrieveAll_name = "retrieveAll";

static const ::std::string __IceStorm__TopicManager__getSliceChecksums_name = "getSliceChecksums";

ICE_DECLSPEC_EXPORT ::Ice::Object* IceInternal::upCast(::IceStorm::Topic* p) { return p; }
ICE_DECLSPEC_EXPORT ::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::IceStorm::Topic* p) { return p; }

ICE_DECLSPEC_EXPORT ::Ice::Object* IceInternal::upCast(::IceStorm::TopicManager* p) { return p; }
ICE_DECLSPEC_EXPORT ::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::IceStorm::TopicManager* p) { return p; }

void
IceStorm::__read(::IceInternal::BasicStream* __is, ::IceStorm::TopicPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::IceStorm::Topic;
        v->__copyFrom(proxy);
    }
}

void
IceStorm::__read(::IceInternal::BasicStream* __is, ::IceStorm::TopicManagerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::IceStorm::TopicManager;
        v->__copyFrom(proxy);
    }
}

bool
IceStorm::LinkInfo::operator==(const LinkInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(theTopic != __rhs.theTopic)
    {
        return false;
    }
    if(name != __rhs.name)
    {
        return false;
    }
    if(cost != __rhs.cost)
    {
        return false;
    }
    return true;
}

bool
IceStorm::LinkInfo::operator<(const LinkInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(theTopic < __rhs.theTopic)
    {
        return true;
    }
    else if(__rhs.theTopic < theTopic)
    {
        return false;
    }
    if(name < __rhs.name)
    {
        return true;
    }
    else if(__rhs.name < name)
    {
        return false;
    }
    if(cost < __rhs.cost)
    {
        return true;
    }
    else if(__rhs.cost < cost)
    {
        return false;
    }
    return false;
}

void
IceStorm::LinkInfo::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::Ice::ObjectPrx(::IceInternal::upCast(theTopic.get())));
    __os->write(name);
    __os->write(cost);
}

void
IceStorm::LinkInfo::__read(::IceInternal::BasicStream* __is)
{
    ::IceStorm::__read(__is, theTopic);
    __is->read(name);
    __is->read(cost);
}

void
IceStorm::__writeLinkInfoSeq(::IceInternal::BasicStream* __os, const ::IceStorm::LinkInfo* begin, const ::IceStorm::LinkInfo* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
IceStorm::__readLinkInfoSeq(::IceInternal::BasicStream* __is, ::IceStorm::LinkInfoSeq& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 7);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

void
IceStorm::__writeQoS(::IceInternal::BasicStream* __os, const ::IceStorm::QoS& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::IceStorm::QoS::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        __os->write(p->second);
    }
}

void
IceStorm::__readQoS(::IceInternal::BasicStream* __is, ::IceStorm::QoS& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::std::string> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::IceStorm::QoS::iterator __i = v.insert(v.end(), pair);
        __is->read(__i->second);
    }
}

IceStorm::LinkExists::LinkExists(const ::std::string& __ice_name) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    name(__ice_name)
{
}

IceStorm::LinkExists::~LinkExists() throw()
{
}

static const char* __IceStorm__LinkExists_name = "IceStorm::LinkExists";

::std::string
IceStorm::LinkExists::ice_name() const
{
    return __IceStorm__LinkExists_name;
}

::Ice::Exception*
IceStorm::LinkExists::ice_clone() const
{
    return new LinkExists(*this);
}

void
IceStorm::LinkExists::ice_throw() const
{
    throw *this;
}

void
IceStorm::LinkExists::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::IceStorm::LinkExists"), false);
    __os->startWriteSlice();
    __os->write(name);
    __os->endWriteSlice();
}

void
IceStorm::LinkExists::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(name);
    __is->endReadSlice();
}

void
IceStorm::LinkExists::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::LinkExists was not generated with stream support";
    throw ex;
}

void
IceStorm::LinkExists::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::LinkExists was not generated with stream support";
    throw ex;
}

struct __F__IceStorm__LinkExists : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::IceStorm::LinkExists();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__IceStorm__LinkExists__Ptr = new __F__IceStorm__LinkExists;

const ::IceInternal::UserExceptionFactoryPtr&
IceStorm::LinkExists::ice_factory()
{
    return __F__IceStorm__LinkExists__Ptr;
}

class __F__IceStorm__LinkExists__Init
{
public:

    __F__IceStorm__LinkExists__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::IceStorm::LinkExists", ::IceStorm::LinkExists::ice_factory());
    }

    ~__F__IceStorm__LinkExists__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::IceStorm::LinkExists");
    }
};

static __F__IceStorm__LinkExists__Init __F__IceStorm__LinkExists__i;

#ifdef __APPLE__
extern "C" { void __F__IceStorm__LinkExists__initializer() {} }
#endif

IceStorm::NoSuchLink::NoSuchLink(const ::std::string& __ice_name) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    name(__ice_name)
{
}

IceStorm::NoSuchLink::~NoSuchLink() throw()
{
}

static const char* __IceStorm__NoSuchLink_name = "IceStorm::NoSuchLink";

::std::string
IceStorm::NoSuchLink::ice_name() const
{
    return __IceStorm__NoSuchLink_name;
}

::Ice::Exception*
IceStorm::NoSuchLink::ice_clone() const
{
    return new NoSuchLink(*this);
}

void
IceStorm::NoSuchLink::ice_throw() const
{
    throw *this;
}

void
IceStorm::NoSuchLink::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::IceStorm::NoSuchLink"), false);
    __os->startWriteSlice();
    __os->write(name);
    __os->endWriteSlice();
}

void
IceStorm::NoSuchLink::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(name);
    __is->endReadSlice();
}

void
IceStorm::NoSuchLink::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::NoSuchLink was not generated with stream support";
    throw ex;
}

void
IceStorm::NoSuchLink::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::NoSuchLink was not generated with stream support";
    throw ex;
}

struct __F__IceStorm__NoSuchLink : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::IceStorm::NoSuchLink();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__IceStorm__NoSuchLink__Ptr = new __F__IceStorm__NoSuchLink;

const ::IceInternal::UserExceptionFactoryPtr&
IceStorm::NoSuchLink::ice_factory()
{
    return __F__IceStorm__NoSuchLink__Ptr;
}

class __F__IceStorm__NoSuchLink__Init
{
public:

    __F__IceStorm__NoSuchLink__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::IceStorm::NoSuchLink", ::IceStorm::NoSuchLink::ice_factory());
    }

    ~__F__IceStorm__NoSuchLink__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::IceStorm::NoSuchLink");
    }
};

static __F__IceStorm__NoSuchLink__Init __F__IceStorm__NoSuchLink__i;

#ifdef __APPLE__
extern "C" { void __F__IceStorm__NoSuchLink__initializer() {} }
#endif

IceStorm::AlreadySubscribed::~AlreadySubscribed() throw()
{
}

static const char* __IceStorm__AlreadySubscribed_name = "IceStorm::AlreadySubscribed";

::std::string
IceStorm::AlreadySubscribed::ice_name() const
{
    return __IceStorm__AlreadySubscribed_name;
}

::Ice::Exception*
IceStorm::AlreadySubscribed::ice_clone() const
{
    return new AlreadySubscribed(*this);
}

void
IceStorm::AlreadySubscribed::ice_throw() const
{
    throw *this;
}

void
IceStorm::AlreadySubscribed::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::IceStorm::AlreadySubscribed"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
}

void
IceStorm::AlreadySubscribed::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
}

void
IceStorm::AlreadySubscribed::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::AlreadySubscribed was not generated with stream support";
    throw ex;
}

void
IceStorm::AlreadySubscribed::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::AlreadySubscribed was not generated with stream support";
    throw ex;
}

struct __F__IceStorm__AlreadySubscribed : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::IceStorm::AlreadySubscribed();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__IceStorm__AlreadySubscribed__Ptr = new __F__IceStorm__AlreadySubscribed;

const ::IceInternal::UserExceptionFactoryPtr&
IceStorm::AlreadySubscribed::ice_factory()
{
    return __F__IceStorm__AlreadySubscribed__Ptr;
}

class __F__IceStorm__AlreadySubscribed__Init
{
public:

    __F__IceStorm__AlreadySubscribed__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::IceStorm::AlreadySubscribed", ::IceStorm::AlreadySubscribed::ice_factory());
    }

    ~__F__IceStorm__AlreadySubscribed__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::IceStorm::AlreadySubscribed");
    }
};

static __F__IceStorm__AlreadySubscribed__Init __F__IceStorm__AlreadySubscribed__i;

#ifdef __APPLE__
extern "C" { void __F__IceStorm__AlreadySubscribed__initializer() {} }
#endif

IceStorm::BadQoS::BadQoS(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    reason(__ice_reason)
{
}

IceStorm::BadQoS::~BadQoS() throw()
{
}

static const char* __IceStorm__BadQoS_name = "IceStorm::BadQoS";

::std::string
IceStorm::BadQoS::ice_name() const
{
    return __IceStorm__BadQoS_name;
}

::Ice::Exception*
IceStorm::BadQoS::ice_clone() const
{
    return new BadQoS(*this);
}

void
IceStorm::BadQoS::ice_throw() const
{
    throw *this;
}

void
IceStorm::BadQoS::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::IceStorm::BadQoS"), false);
    __os->startWriteSlice();
    __os->write(reason);
    __os->endWriteSlice();
}

void
IceStorm::BadQoS::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(reason);
    __is->endReadSlice();
}

void
IceStorm::BadQoS::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::BadQoS was not generated with stream support";
    throw ex;
}

void
IceStorm::BadQoS::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::BadQoS was not generated with stream support";
    throw ex;
}

struct __F__IceStorm__BadQoS : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::IceStorm::BadQoS();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__IceStorm__BadQoS__Ptr = new __F__IceStorm__BadQoS;

const ::IceInternal::UserExceptionFactoryPtr&
IceStorm::BadQoS::ice_factory()
{
    return __F__IceStorm__BadQoS__Ptr;
}

class __F__IceStorm__BadQoS__Init
{
public:

    __F__IceStorm__BadQoS__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::IceStorm::BadQoS", ::IceStorm::BadQoS::ice_factory());
    }

    ~__F__IceStorm__BadQoS__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::IceStorm::BadQoS");
    }
};

static __F__IceStorm__BadQoS__Init __F__IceStorm__BadQoS__i;

#ifdef __APPLE__
extern "C" { void __F__IceStorm__BadQoS__initializer() {} }
#endif

void
IceStorm::__writeTopicDict(::IceInternal::BasicStream* __os, const ::IceStorm::TopicDict& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::IceStorm::TopicDict::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(p->second.get())));
    }
}

void
IceStorm::__readTopicDict(::IceInternal::BasicStream* __is, ::IceStorm::TopicDict& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::IceStorm::TopicPrx> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::IceStorm::TopicDict::iterator __i = v.insert(v.end(), pair);
        ::IceStorm::__read(__is, __i->second);
    }
}

IceStorm::TopicExists::TopicExists(const ::std::string& __ice_name) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    name(__ice_name)
{
}

IceStorm::TopicExists::~TopicExists() throw()
{
}

static const char* __IceStorm__TopicExists_name = "IceStorm::TopicExists";

::std::string
IceStorm::TopicExists::ice_name() const
{
    return __IceStorm__TopicExists_name;
}

::Ice::Exception*
IceStorm::TopicExists::ice_clone() const
{
    return new TopicExists(*this);
}

void
IceStorm::TopicExists::ice_throw() const
{
    throw *this;
}

void
IceStorm::TopicExists::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::IceStorm::TopicExists"), false);
    __os->startWriteSlice();
    __os->write(name);
    __os->endWriteSlice();
}

void
IceStorm::TopicExists::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(name);
    __is->endReadSlice();
}

void
IceStorm::TopicExists::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::TopicExists was not generated with stream support";
    throw ex;
}

void
IceStorm::TopicExists::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::TopicExists was not generated with stream support";
    throw ex;
}

struct __F__IceStorm__TopicExists : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::IceStorm::TopicExists();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__IceStorm__TopicExists__Ptr = new __F__IceStorm__TopicExists;

const ::IceInternal::UserExceptionFactoryPtr&
IceStorm::TopicExists::ice_factory()
{
    return __F__IceStorm__TopicExists__Ptr;
}

class __F__IceStorm__TopicExists__Init
{
public:

    __F__IceStorm__TopicExists__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::IceStorm::TopicExists", ::IceStorm::TopicExists::ice_factory());
    }

    ~__F__IceStorm__TopicExists__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::IceStorm::TopicExists");
    }
};

static __F__IceStorm__TopicExists__Init __F__IceStorm__TopicExists__i;

#ifdef __APPLE__
extern "C" { void __F__IceStorm__TopicExists__initializer() {} }
#endif

IceStorm::NoSuchTopic::NoSuchTopic(const ::std::string& __ice_name) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    name(__ice_name)
{
}

IceStorm::NoSuchTopic::~NoSuchTopic() throw()
{
}

static const char* __IceStorm__NoSuchTopic_name = "IceStorm::NoSuchTopic";

::std::string
IceStorm::NoSuchTopic::ice_name() const
{
    return __IceStorm__NoSuchTopic_name;
}

::Ice::Exception*
IceStorm::NoSuchTopic::ice_clone() const
{
    return new NoSuchTopic(*this);
}

void
IceStorm::NoSuchTopic::ice_throw() const
{
    throw *this;
}

void
IceStorm::NoSuchTopic::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::IceStorm::NoSuchTopic"), false);
    __os->startWriteSlice();
    __os->write(name);
    __os->endWriteSlice();
}

void
IceStorm::NoSuchTopic::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(name);
    __is->endReadSlice();
}

void
IceStorm::NoSuchTopic::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::NoSuchTopic was not generated with stream support";
    throw ex;
}

void
IceStorm::NoSuchTopic::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception IceStorm::NoSuchTopic was not generated with stream support";
    throw ex;
}

struct __F__IceStorm__NoSuchTopic : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::IceStorm::NoSuchTopic();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__IceStorm__NoSuchTopic__Ptr = new __F__IceStorm__NoSuchTopic;

const ::IceInternal::UserExceptionFactoryPtr&
IceStorm::NoSuchTopic::ice_factory()
{
    return __F__IceStorm__NoSuchTopic__Ptr;
}

class __F__IceStorm__NoSuchTopic__Init
{
public:

    __F__IceStorm__NoSuchTopic__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::IceStorm::NoSuchTopic", ::IceStorm::NoSuchTopic::ice_factory());
    }

    ~__F__IceStorm__NoSuchTopic__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::IceStorm::NoSuchTopic");
    }
};

static __F__IceStorm__NoSuchTopic__Init __F__IceStorm__NoSuchTopic__i;

#ifdef __APPLE__
extern "C" { void __F__IceStorm__NoSuchTopic__initializer() {} }
#endif
#ifdef __SUNPRO_CC
class ICE_DECLSPEC_EXPORT IceProxy::IceStorm::Topic;
#endif

::std::string
IceProxy::IceStorm::Topic::getName(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__Topic__getName_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            return __del->getName(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::ObjectPrx
IceProxy::IceStorm::Topic::getPublisher(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__Topic__getPublisher_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            return __del->getPublisher(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::ObjectPrx
IceProxy::IceStorm::Topic::getNonReplicatedPublisher(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__Topic__getNonReplicatedPublisher_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            return __del->getNonReplicatedPublisher(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::IceStorm::Topic::subscribe(const ::IceStorm::QoS& theQoS, const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            __del->subscribe(theQoS, subscriber, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::ObjectPrx
IceProxy::IceStorm::Topic::subscribeAndGetPublisher(const ::IceStorm::QoS& theQoS, const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__Topic__subscribeAndGetPublisher_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            return __del->subscribeAndGetPublisher(theQoS, subscriber, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::IceStorm::Topic::unsubscribe(const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            __del->unsubscribe(subscriber, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::IceStorm::Topic::link(const ::IceStorm::TopicPrx& linkTo, ::Ice::Int cost, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__Topic__link_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            __del->link(linkTo, cost, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::IceStorm::Topic::unlink(const ::IceStorm::TopicPrx& linkTo, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__Topic__unlink_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            __del->unlink(linkTo, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::IceStorm::LinkInfoSeq
IceProxy::IceStorm::Topic::getLinkInfoSeq(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__Topic__getLinkInfoSeq_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            return __del->getLinkInfoSeq(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::IceStorm::Topic::destroy(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::Topic* __del = dynamic_cast< ::IceDelegate::IceStorm::Topic*>(__delBase.get());
            __del->destroy(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::IceStorm::Topic::ice_staticId()
{
    return ::IceStorm::Topic::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::IceStorm::Topic::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::IceStorm::Topic);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::IceStorm::Topic::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::IceStorm::Topic);
}

::IceProxy::Ice::Object*
IceProxy::IceStorm::Topic::__newInstance() const
{
    return new Topic;
}
#ifdef __SUNPRO_CC
class ICE_DECLSPEC_EXPORT IceProxy::IceStorm::TopicManager;
#endif

::IceStorm::TopicPrx
IceProxy::IceStorm::TopicManager::create(const ::std::string& name, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__TopicManager__create_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::TopicManager* __del = dynamic_cast< ::IceDelegate::IceStorm::TopicManager*>(__delBase.get());
            return __del->create(name, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::IceStorm::TopicPrx
IceProxy::IceStorm::TopicManager::retrieve(const ::std::string& name, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__TopicManager__retrieve_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::TopicManager* __del = dynamic_cast< ::IceDelegate::IceStorm::TopicManager*>(__delBase.get());
            return __del->retrieve(name, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::IceStorm::TopicDict
IceProxy::IceStorm::TopicManager::retrieveAll(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__TopicManager__retrieveAll_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::TopicManager* __del = dynamic_cast< ::IceDelegate::IceStorm::TopicManager*>(__delBase.get());
            return __del->retrieveAll(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::SliceChecksumDict
IceProxy::IceStorm::TopicManager::getSliceChecksums(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__IceStorm__TopicManager__getSliceChecksums_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::IceStorm::TopicManager* __del = dynamic_cast< ::IceDelegate::IceStorm::TopicManager*>(__delBase.get());
            return __del->getSliceChecksums(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::IceStorm::TopicManager::ice_staticId()
{
    return ::IceStorm::TopicManager::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::IceStorm::TopicManager::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::IceStorm::TopicManager);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::IceStorm::TopicManager::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::IceStorm::TopicManager);
}

::IceProxy::Ice::Object*
IceProxy::IceStorm::TopicManager::__newInstance() const
{
    return new TopicManager;
}

::std::string
IceDelegateM::IceStorm::Topic::getName(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__getName_name, ::Ice::Nonmutating, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::ObjectPrx
IceDelegateM::IceStorm::Topic::getPublisher(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__getPublisher_name, ::Ice::Nonmutating, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::ObjectPrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::ObjectPrx
IceDelegateM::IceStorm::Topic::getNonReplicatedPublisher(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__getNonReplicatedPublisher_name, ::Ice::Nonmutating, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::ObjectPrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::IceStorm::Topic::subscribe(const ::IceStorm::QoS& theQoS, const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__subscribe_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::IceStorm::__writeQoS(__os, theQoS);
        __os->write(subscriber);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::Ice::ObjectPrx
IceDelegateM::IceStorm::Topic::subscribeAndGetPublisher(const ::IceStorm::QoS& theQoS, const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__subscribeAndGetPublisher_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::IceStorm::__writeQoS(__os, theQoS);
        __os->write(subscriber);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::IceStorm::AlreadySubscribed&)
            {
                throw;
            }
            catch(const ::IceStorm::BadQoS&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::ObjectPrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::IceStorm::Topic::unsubscribe(const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__unsubscribe_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(subscriber);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::IceStorm::Topic::link(const ::IceStorm::TopicPrx& linkTo, ::Ice::Int cost, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__link_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(linkTo.get())));
        __os->write(cost);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::IceStorm::LinkExists&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::IceStorm::Topic::unlink(const ::IceStorm::TopicPrx& linkTo, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__unlink_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(linkTo.get())));
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::IceStorm::NoSuchLink&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::IceStorm::LinkInfoSeq
IceDelegateM::IceStorm::Topic::getLinkInfoSeq(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__getLinkInfoSeq_name, ::Ice::Nonmutating, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceStorm::LinkInfoSeq __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::IceStorm::__readLinkInfoSeq(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::IceStorm::Topic::destroy(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__Topic__destroy_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::IceStorm::TopicPrx
IceDelegateM::IceStorm::TopicManager::create(const ::std::string& name, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__TopicManager__create_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(name);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::IceStorm::TopicExists&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceStorm::TopicPrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::IceStorm::__read(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::IceStorm::TopicPrx
IceDelegateM::IceStorm::TopicManager::retrieve(const ::std::string& name, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__TopicManager__retrieve_name, ::Ice::Nonmutating, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(name);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::IceStorm::NoSuchTopic&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceStorm::TopicPrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::IceStorm::__read(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::IceStorm::TopicDict
IceDelegateM::IceStorm::TopicManager::retrieveAll(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__TopicManager__retrieveAll_name, ::Ice::Nonmutating, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceStorm::TopicDict __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::IceStorm::__readTopicDict(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::SliceChecksumDict
IceDelegateM::IceStorm::TopicManager::getSliceChecksums(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __IceStorm__TopicManager__getSliceChecksums_name, ::Ice::Nonmutating, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::SliceChecksumDict __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::Ice::__readSliceChecksumDict(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateD::IceStorm::Topic::getName(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getName(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__getName_name, ::Ice::Nonmutating, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPrx
IceDelegateD::IceStorm::Topic::getPublisher(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::ObjectPrx& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPublisher(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::ObjectPrx& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__getPublisher_name, ::Ice::Nonmutating, __context);
    ::Ice::ObjectPrx __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPrx
IceDelegateD::IceStorm::Topic::getNonReplicatedPublisher(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::ObjectPrx& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getNonReplicatedPublisher(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::ObjectPrx& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__getNonReplicatedPublisher_name, ::Ice::Nonmutating, __context);
    ::Ice::ObjectPrx __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::IceStorm::Topic::subscribe(const ::IceStorm::QoS& theQoS, const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::IceStorm::QoS& theQoS, const ::Ice::ObjectPrx& subscriber, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_theQoS(theQoS),
            _m_subscriber(subscriber)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->subscribe(_m_theQoS, _m_subscriber, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::IceStorm::QoS& _m_theQoS;
        const ::Ice::ObjectPrx& _m_subscriber;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__subscribe_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(theQoS, subscriber, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::ObjectPrx
IceDelegateD::IceStorm::Topic::subscribeAndGetPublisher(const ::IceStorm::QoS& theQoS, const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::ObjectPrx& __result, const ::IceStorm::QoS& theQoS, const ::Ice::ObjectPrx& subscriber, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_theQoS(theQoS),
            _m_subscriber(subscriber)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->subscribeAndGetPublisher(_m_theQoS, _m_subscriber, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::Ice::ObjectPrx& _result;
        const ::IceStorm::QoS& _m_theQoS;
        const ::Ice::ObjectPrx& _m_subscriber;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__subscribeAndGetPublisher_name, ::Ice::Normal, __context);
    ::Ice::ObjectPrx __result;
    try
    {
        _DirectI __direct(__result, theQoS, subscriber, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::IceStorm::AlreadySubscribed&)
    {
        throw;
    }
    catch(const ::IceStorm::BadQoS&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::IceStorm::Topic::unsubscribe(const ::Ice::ObjectPrx& subscriber, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::ObjectPrx& subscriber, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_subscriber(subscriber)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->unsubscribe(_m_subscriber, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::Ice::ObjectPrx& _m_subscriber;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__unsubscribe_name, ::Ice::Idempotent, __context);
    try
    {
        _DirectI __direct(subscriber, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::IceStorm::Topic::link(const ::IceStorm::TopicPrx& linkTo, ::Ice::Int cost, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::IceStorm::TopicPrx& linkTo, ::Ice::Int cost, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_linkTo(linkTo),
            _m_cost(cost)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->link(_m_linkTo, _m_cost, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        const ::IceStorm::TopicPrx& _m_linkTo;
        ::Ice::Int _m_cost;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__link_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(linkTo, cost, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::IceStorm::LinkExists&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::IceStorm::Topic::unlink(const ::IceStorm::TopicPrx& linkTo, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::IceStorm::TopicPrx& linkTo, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_linkTo(linkTo)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->unlink(_m_linkTo, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        const ::IceStorm::TopicPrx& _m_linkTo;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__unlink_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(linkTo, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::IceStorm::NoSuchLink&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::IceStorm::LinkInfoSeq
IceDelegateD::IceStorm::Topic::getLinkInfoSeq(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::IceStorm::LinkInfoSeq& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getLinkInfoSeq(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::IceStorm::LinkInfoSeq& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__getLinkInfoSeq_name, ::Ice::Nonmutating, __context);
    ::IceStorm::LinkInfoSeq __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::IceStorm::Topic::destroy(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::Topic* servant = dynamic_cast< ::IceStorm::Topic*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->destroy(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__Topic__destroy_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::IceStorm::TopicPrx
IceDelegateD::IceStorm::TopicManager::create(const ::std::string& name, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::IceStorm::TopicPrx& __result, const ::std::string& name, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_name(name)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::TopicManager* servant = dynamic_cast< ::IceStorm::TopicManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->create(_m_name, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::IceStorm::TopicPrx& _result;
        const ::std::string& _m_name;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__TopicManager__create_name, ::Ice::Normal, __context);
    ::IceStorm::TopicPrx __result;
    try
    {
        _DirectI __direct(__result, name, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::IceStorm::TopicExists&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::IceStorm::TopicPrx
IceDelegateD::IceStorm::TopicManager::retrieve(const ::std::string& name, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::IceStorm::TopicPrx& __result, const ::std::string& name, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_name(name)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::TopicManager* servant = dynamic_cast< ::IceStorm::TopicManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->retrieve(_m_name, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::IceStorm::TopicPrx& _result;
        const ::std::string& _m_name;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__TopicManager__retrieve_name, ::Ice::Nonmutating, __context);
    ::IceStorm::TopicPrx __result;
    try
    {
        _DirectI __direct(__result, name, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::IceStorm::NoSuchTopic&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::IceStorm::TopicDict
IceDelegateD::IceStorm::TopicManager::retrieveAll(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::IceStorm::TopicDict& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::TopicManager* servant = dynamic_cast< ::IceStorm::TopicManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->retrieveAll(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::IceStorm::TopicDict& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__TopicManager__retrieveAll_name, ::Ice::Nonmutating, __context);
    ::IceStorm::TopicDict __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::SliceChecksumDict
IceDelegateD::IceStorm::TopicManager::getSliceChecksums(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::SliceChecksumDict& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::IceStorm::TopicManager* servant = dynamic_cast< ::IceStorm::TopicManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getSliceChecksums(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::SliceChecksumDict& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __IceStorm__TopicManager__getSliceChecksums_name, ::Ice::Nonmutating, __context);
    ::Ice::SliceChecksumDict __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPtr
IceStorm::Topic::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __IceStorm__Topic_ids[2] =
{
    "::Ice::Object",
    "::IceStorm::Topic"
};

bool
IceStorm::Topic::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__IceStorm__Topic_ids, __IceStorm__Topic_ids + 2, _s);
}

::std::vector< ::std::string>
IceStorm::Topic::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__IceStorm__Topic_ids[0], &__IceStorm__Topic_ids[2]);
}

const ::std::string&
IceStorm::Topic::ice_id(const ::Ice::Current&) const
{
    return __IceStorm__Topic_ids[1];
}

const ::std::string&
IceStorm::Topic::ice_staticId()
{
    return __IceStorm__Topic_ids[1];
}

::Ice::DispatchStatus
IceStorm::Topic::___getName(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = getName(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___getPublisher(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::ObjectPrx __ret = getPublisher(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___getNonReplicatedPublisher(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::ObjectPrx __ret = getNonReplicatedPublisher(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___subscribe(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::IceStorm::QoS theQoS;
    ::Ice::ObjectPrx subscriber;
    ::IceStorm::__readQoS(__is, theQoS);
    __is->read(subscriber);
    __is->endReadEncaps();
    subscribe(theQoS, subscriber, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___subscribeAndGetPublisher(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::IceStorm::QoS theQoS;
    ::Ice::ObjectPrx subscriber;
    ::IceStorm::__readQoS(__is, theQoS);
    __is->read(subscriber);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::Ice::ObjectPrx __ret = subscribeAndGetPublisher(theQoS, subscriber, __current);
        __os->write(__ret);
    }
    catch(const ::IceStorm::AlreadySubscribed& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::IceStorm::BadQoS& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___unsubscribe(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::ObjectPrx subscriber;
    __is->read(subscriber);
    __is->endReadEncaps();
    unsubscribe(subscriber, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___link(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::IceStorm::TopicPrx linkTo;
    ::Ice::Int cost;
    ::IceStorm::__read(__is, linkTo);
    __is->read(cost);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        link(linkTo, cost, __current);
    }
    catch(const ::IceStorm::LinkExists& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___unlink(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::IceStorm::TopicPrx linkTo;
    ::IceStorm::__read(__is, linkTo);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        unlink(linkTo, __current);
    }
    catch(const ::IceStorm::NoSuchLink& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___getLinkInfoSeq(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::IceStorm::LinkInfoSeq __ret = getLinkInfoSeq(__current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::IceStorm::__writeLinkInfoSeq(__os, &__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::Topic::___destroy(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    destroy(__current);
    return ::Ice::DispatchOK;
}

static ::std::string __IceStorm__Topic_all[] =
{
    "destroy",
    "getLinkInfoSeq",
    "getName",
    "getNonReplicatedPublisher",
    "getPublisher",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "link",
    "subscribe",
    "subscribeAndGetPublisher",
    "unlink",
    "unsubscribe"
};

::Ice::DispatchStatus
IceStorm::Topic::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__IceStorm__Topic_all, __IceStorm__Topic_all + 14, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __IceStorm__Topic_all)
    {
        case 0:
        {
            return ___destroy(in, current);
        }
        case 1:
        {
            return ___getLinkInfoSeq(in, current);
        }
        case 2:
        {
            return ___getName(in, current);
        }
        case 3:
        {
            return ___getNonReplicatedPublisher(in, current);
        }
        case 4:
        {
            return ___getPublisher(in, current);
        }
        case 5:
        {
            return ___ice_id(in, current);
        }
        case 6:
        {
            return ___ice_ids(in, current);
        }
        case 7:
        {
            return ___ice_isA(in, current);
        }
        case 8:
        {
            return ___ice_ping(in, current);
        }
        case 9:
        {
            return ___link(in, current);
        }
        case 10:
        {
            return ___subscribe(in, current);
        }
        case 11:
        {
            return ___subscribeAndGetPublisher(in, current);
        }
        case 12:
        {
            return ___unlink(in, current);
        }
        case 13:
        {
            return ___unsubscribe(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
IceStorm::Topic::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
IceStorm::Topic::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
IceStorm::Topic::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type IceStorm::Topic was not generated with stream support";
    throw ex;
}

void
IceStorm::Topic::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type IceStorm::Topic was not generated with stream support";
    throw ex;
}

void ICE_DECLSPEC_EXPORT 
IceStorm::__patch__TopicPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::IceStorm::TopicPtr* p = static_cast< ::IceStorm::TopicPtr*>(__addr);
    assert(p);
    *p = ::IceStorm::TopicPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::IceStorm::Topic::ice_staticId(), v->ice_id());
    }
}

bool
IceStorm::operator==(const ::IceStorm::Topic& l, const ::IceStorm::Topic& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
IceStorm::operator<(const ::IceStorm::Topic& l, const ::IceStorm::Topic& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
IceStorm::TopicManager::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __IceStorm__TopicManager_ids[2] =
{
    "::Ice::Object",
    "::IceStorm::TopicManager"
};

bool
IceStorm::TopicManager::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__IceStorm__TopicManager_ids, __IceStorm__TopicManager_ids + 2, _s);
}

::std::vector< ::std::string>
IceStorm::TopicManager::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__IceStorm__TopicManager_ids[0], &__IceStorm__TopicManager_ids[2]);
}

const ::std::string&
IceStorm::TopicManager::ice_id(const ::Ice::Current&) const
{
    return __IceStorm__TopicManager_ids[1];
}

const ::std::string&
IceStorm::TopicManager::ice_staticId()
{
    return __IceStorm__TopicManager_ids[1];
}

::Ice::DispatchStatus
IceStorm::TopicManager::___create(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string name;
    __is->read(name);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::IceStorm::TopicPrx __ret = create(name, __current);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    }
    catch(const ::IceStorm::TopicExists& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::TopicManager::___retrieve(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string name;
    __is->read(name);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::IceStorm::TopicPrx __ret = retrieve(name, __current);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    }
    catch(const ::IceStorm::NoSuchTopic& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::TopicManager::___retrieveAll(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::IceStorm::TopicDict __ret = retrieveAll(__current);
    ::IceStorm::__writeTopicDict(__os, __ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
IceStorm::TopicManager::___getSliceChecksums(::IceInternal::Incoming& __inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::SliceChecksumDict __ret = getSliceChecksums(__current);
    ::Ice::__writeSliceChecksumDict(__os, __ret);
    return ::Ice::DispatchOK;
}

static ::std::string __IceStorm__TopicManager_all[] =
{
    "create",
    "getSliceChecksums",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "retrieve",
    "retrieveAll"
};

::Ice::DispatchStatus
IceStorm::TopicManager::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__IceStorm__TopicManager_all, __IceStorm__TopicManager_all + 8, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __IceStorm__TopicManager_all)
    {
        case 0:
        {
            return ___create(in, current);
        }
        case 1:
        {
            return ___getSliceChecksums(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
        case 6:
        {
            return ___retrieve(in, current);
        }
        case 7:
        {
            return ___retrieveAll(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
IceStorm::TopicManager::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
IceStorm::TopicManager::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
IceStorm::TopicManager::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type IceStorm::TopicManager was not generated with stream support";
    throw ex;
}

void
IceStorm::TopicManager::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type IceStorm::TopicManager was not generated with stream support";
    throw ex;
}

void ICE_DECLSPEC_EXPORT 
IceStorm::__patch__TopicManagerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::IceStorm::TopicManagerPtr* p = static_cast< ::IceStorm::TopicManagerPtr*>(__addr);
    assert(p);
    *p = ::IceStorm::TopicManagerPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::IceStorm::TopicManager::ice_staticId(), v->ice_id());
    }
}

bool
IceStorm::operator==(const ::IceStorm::TopicManager& l, const ::IceStorm::TopicManager& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
IceStorm::operator<(const ::IceStorm::TopicManager& l, const ::IceStorm::TopicManager& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
