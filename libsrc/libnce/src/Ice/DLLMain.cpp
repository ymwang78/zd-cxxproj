// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Ice/EventLoggerI.h>
#include <Ice/ImplicitContextI.h>
#include <Ice/Service.h>

extern "C" BOOL WINAPI _CRT_INIT(HINSTANCE, DWORD, LPVOID);

#ifndef __NO_STATIC_MUTEX__
extern void StaticMutex_Init();
extern void StaticMutex_Fini();
#endif

extern "C"
{

BOOL WINAPI
#ifdef __BCPLUSPLUS__
DllMain(HINSTANCE hDLL, DWORD reason, LPVOID reserved)
{
#else
ice_DLL_Main(HINSTANCE hDLL, DWORD reason, LPVOID reserved)
{
#ifndef __NO_STATIC_MUTEX__
	if (reason == DLL_PROCESS_ATTACH)
		StaticMutex_Init();
#endif

    //
    // During ATTACH, we must call _CRT_INIT first.
    //
    if(reason == DLL_PROCESS_ATTACH || reason == DLL_THREAD_ATTACH)
    {
        if(!_CRT_INIT(hDLL, reason, reserved))
        {
            return FALSE;
        }
    }
#endif

    if(reason == DLL_PROCESS_ATTACH)
    {
        Ice::EventLoggerI::setModuleHandle(hDLL);
        Ice::Service::setModuleHandle(hDLL);
    }
    else if(reason == DLL_THREAD_DETACH)
    {
        Ice::ImplicitContextI::cleanupThread();
    }

#ifndef __BCPLUSPLUS__
    //
    // During DETACH, we must call _CRT_INIT last.
    //
    if(reason == DLL_PROCESS_DETACH || reason == DLL_THREAD_DETACH)
    {
        if(!_CRT_INIT(hDLL, reason, reserved))
        {
            return FALSE;
        }
    }
#endif

#ifndef __NO_STATIC_MUTEX__
	if (reason == DLL_PROCESS_DETACH)
		StaticMutex_Fini();
#endif

    return TRUE;
}

}
