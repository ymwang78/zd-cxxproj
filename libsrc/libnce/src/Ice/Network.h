// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_NETWORK_H
#define ICE_NETWORK_H

#ifdef __hpux
#   define _XOPEN_SOURCE_EXTENDED
#endif

#include <Ice/Config.h>
#include <Ice/PropertiesF.h> // For setTcpBufSize
#include <Ice/LoggerF.h> // For setTcpBufSize
#include <Ice/Protocol.h> 

#ifdef _WIN32
#   include <winsock2.h>
#   include <ws2tcpip.h>
typedef int ssize_t;
#else
#   include <unistd.h>
#   include <fcntl.h>
#   include <sys/socket.h>
#   include <sys/poll.h>
#   include <netinet/in.h>
#   include <netinet/tcp.h>
#   include <arpa/inet.h>
#   include <netdb.h>
#endif

#if defined(_WIN32) || defined(__osf__) 
typedef int socklen_t;
#endif

#ifndef _WIN32
#   define SOCKET int
#   define SOCKET_ERROR -1
#   define INVALID_SOCKET -1
#endif

#ifndef SHUT_RD
#   define SHUT_RD 0
#endif

#ifndef SHUT_WR
#   define SHUT_WR 1
#endif

#ifndef SHUT_RDWR
#   define SHUT_RDWR 2
#endif

#ifndef NETDB_INTERNAL
#   define NETDB_INTERNAL -1
#endif

#ifndef NETDB_SUCCESS
#   define NETDB_SUCCESS 0
#endif

namespace IceInternal
{

ICE_API bool interrupted();
ICE_API bool acceptInterrupted();
ICE_API bool noBuffers();
ICE_API bool wouldBlock();
ICE_API bool connectFailed();
ICE_API bool connectionRefused();
ICE_API bool connectInProgress();
ICE_API bool connectionLost();
ICE_API bool notConnected();
ICE_API bool recvTruncated();
ICE_API bool noMoreFds(int);

ICE_API SOCKET createSocket(bool, int);
ICE_API void closeSocket(SOCKET);
ICE_API void closeSocketNoThrow(SOCKET);
ICE_API void shutdownSocketWrite(SOCKET);
ICE_API void shutdownSocketReadWrite(SOCKET);

ICE_API void setBlock(SOCKET, bool);
ICE_API void setTcpNoDelay(SOCKET);
ICE_API void setKeepAlive(SOCKET);
ICE_API void setSendBufferSize(SOCKET, int);
ICE_API int getSendBufferSize(SOCKET);
ICE_API void setRecvBufferSize(SOCKET, int);
ICE_API int getRecvBufferSize(SOCKET);
ICE_API void setMcastGroup(SOCKET, const struct sockaddr_storage&, const std::string&);
ICE_API void setMcastInterface(SOCKET, const std::string&, bool);
ICE_API void setMcastTtl(SOCKET, int, bool);
ICE_API void setReuseAddress(SOCKET, bool);

ICE_API void doBind(SOCKET, struct sockaddr_storage&);
ICE_API void doListen(SOCKET, int);
ICE_API bool doConnect(SOCKET, struct sockaddr_storage&);
ICE_API void doFinishConnect(SOCKET);
ICE_API SOCKET doAccept(SOCKET);

ICE_API void getAddressForServer(const std::string&, int, struct sockaddr_storage&, ProtocolSupport);
ICE_API void getAddress(const std::string&, int, struct sockaddr_storage&, ProtocolSupport);
ICE_API std::vector<struct sockaddr_storage> getAddresses(const std::string&, int, ProtocolSupport, bool);

ICE_API int compareAddress(const struct sockaddr_storage&, const struct sockaddr_storage&);

ICE_API void createPipe(SOCKET fds[2]);

ICE_API std::string errorToStringDNS(int);

ICE_API std::string fdToString(SOCKET);
ICE_API std::string addressesToString(const struct sockaddr_storage&, const struct sockaddr_storage&, bool);
ICE_API void fdToLocalAddress(SOCKET, struct sockaddr_storage&);
ICE_API bool fdToRemoteAddress(SOCKET, struct sockaddr_storage&);
ICE_API std::string inetAddrToString(const struct sockaddr_storage&);
ICE_API std::string addrToString(const struct sockaddr_storage&);
ICE_API bool isMulticast(const struct sockaddr_storage&);
ICE_API int getPort(const struct sockaddr_storage&);

ICE_API std::vector<std::string> getHostsForEndpointExpand(const std::string&, ProtocolSupport);
ICE_API void setTcpBufSize(SOCKET, const Ice::PropertiesPtr&, const Ice::LoggerPtr&);

ICE_API int getSocketErrno();

}

#endif
