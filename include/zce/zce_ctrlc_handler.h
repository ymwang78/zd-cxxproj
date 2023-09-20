/* ***************************************************************
//  zce_ctrlc_handler   version:  1.0  date: 6/5/2009
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// **************************************************************/
#ifndef __zce_ctrlc_handler_h__
#define __zce_ctrlc_handler_h__

#include <zce/zce_config.h>

// The zce_ctrlc_handler provides a portable way to handle CTRL+C and
// CTRL+C like signals 
// On Unix/POSIX, the zce_ctrlc_handler handles SIGHUP, SIGINT and SIGTERM. 
// On Windows, it is essentially a wrapper for SetConsoleCtrlHandler().
// 
// In a process, only one zce_ctrlc_handler can exist at a given time: 
// the zce_ctrlc_handler constructor raises CtrlCHandlerException if
// you attempt to create a second zce_ctrlc_handler.
// On Unix/POSIX, it is essential to create the zce_ctrlc_handler before
// creating any thread, as the zce_ctrlc_handler constructor masks (blocks)
// SIGHUP, SIGINT and SIGTERM; by default, threads created later will 
// inherit this signal mask.
//
// When a CTRL+C or CTRL+C like signal is sent to the process, the 
// user-registered callback is called in a separate thread; it is 
// given the signal number. The callback must not raise exceptions.
// On Unix/POSIX, the callback is NOT a signal handler and can call 
// functions that are not async-signal safe.
//
// The zce_ctrlc_handler destructor "unregisters" the callback. However
// on Unix/POSIX it does not restore the old signal mask in any
// thread, so SIGHUP, SIGINT and SIGTERM remain blocked.
//
// TODO: Maybe the behavior on Windows should be the same? Now we
// just restore the default behavior (TerminateProcess).

typedef void (*zce_ctrlc_handler_callback)(int);

class ZCE_API zce_ctrlc_handler
{
public:
    zce_ctrlc_handler(zce_ctrlc_handler_callback = 0);
    ~zce_ctrlc_handler();

    void set_callback(zce_ctrlc_handler_callback);
    zce_ctrlc_handler_callback get_callback() const;
};

#endif // __zce_ctrlc_handler_h__
