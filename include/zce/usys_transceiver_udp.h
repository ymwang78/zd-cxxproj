/* ***************************************************************
//  usys_transceiver_tcp   version:  1.0  date: 27/27/2009
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// **************************************************************/
#ifndef __usys_transceiver_udp_h__
#define __usys_transceiver_udp_h__

#include <utiny/usys_transceiver.h>

class UBEDA_API usys_transceiver_udp : public usys_transceiver
{
public:
    usys_transceiver_udp(const usys_smartptr<usys_reactor>& reactor, unsigned send_retry_deque_size = 0, unsigned recv_buffer_size = 1500);
    virtual ~usys_transceiver_udp();

	virtual bool readable() { return true; }
	virtual bool writable();
	virtual bool exceptable() { return false; }

    int connect(sockaddr* addr, int length);
    int connect(const char* addr, unsigned short port);
	
	virtual bool init_socket(int af = AF_INET);
	virtual void close();
    virtual int get_remote_addr(sockaddr_storage* addr);

	// --------------------------------------------------
    // event handlers
    // --------------------------------------------------
    virtual void handle_read_event();
    virtual void handle_write_event();
    virtual void handle_except_event();

	virtual int handle_read();
};

#endif // __usys_transceiver_tcp_h__
