/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#ifndef _OSS_SOCKET_HPP_
#define _OSS_SOCKET_HPP_

#include "core.hpp"
#define SOCKET_GETLASTERROR		errno

// by default 10ms timeout
#define OSS_SOCKET_DFT_TIMEOUT		10000

// max hostname
#define OSS_MAX_HOSTNAME		NI_MAXHOST
#define OSS_MAX_SERVICENAME		NA_MAXSERV

class _ossSocket{
   private:
	int _fd ;
	socklen_t _addressLen ;
	socklen_t _peerAddressLen ;
	struct sockaddr_in _sockAddress ;
	struct sockaddr_in _peerAddress ;
	bool _init;
	int _timeout;
   protected:
	unsigned int _getPort ( sockaddr_in *addr ) ;
	int _getAddress ( sockaddr_in *addr, char *pAddress, unsigned int length ) ;
   public:
	int setSocketLi ( int lOnOff, int linger ) ;
	void setAddress (const char *pHostName, unsigned int port ) ;
	// create a listening socket
	_ossSocket ();
	_ossSocket ( unsigned int port, int timeout = 0 ) ;
	// create a connection socket
	_ossSocket ( const char *pHostname, unsigned int port, int timeout = 0 ) ;
	// create from a existing socket
	_ossSocket ( int *sock, int timeout = 0 ) ;
	~_ossSocket (){
		close () ;
	}
	int initSocket () ;
	int bind_listen () ;
	int send ( const char *pMsg, int len, int timeout = OSS_SOCKET_DFT_TIMEOUT, int flags = 0 ) ;
	int recv ( char *pMsg, int len, int timeout = OSS_SOCKET_DFT_TIMEOUT, int flags = 0 ) ;
	int recvNF ( char *pMsg, int len, int timeout = OSS_SOCKET_DFT_TIMEOUT ) ;
	int connect () ;
	bool isConnected() ;
	void close () ;
	int accept ( int *sock, struct sockaddr *addr, socklen_t *addrlen, int timeout = OSS_SOCKET_DFT_TIMEOUT ) ;
	int disableNagle () ;
	unsigned int getPeerPort () ;
	int getPeerAddress ( char *pAddress, unsigned int length ) ;
	unsigned int getLocalPort () ;
	int getLocalAddress ( char *pAddress, unsigned int length ) ;
	int setTimeout ( int seconds ) ;
	static int getHostName ( char *pName, int nameLen ) ;
	static int getPort ( const char *pServiceName, unsigned short &port ) ;
} ;
typedef class _ossSocket ossSocket ;

#endif
