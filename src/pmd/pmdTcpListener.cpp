/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#include "core.hpp"
#include "ossSocket.hpp"

#define PMD_TCPLISTENER_RETRY 5
#define OSS_MAX_SERVICENAME NI_MAXSERV

int pmdTcpListenerEntryPoint () {
	int rc = GHDB_OK ;
	int port = 48120 ;
	ossSocket sock ( port ) ;
	rc = sock.initSocket () ;
	if ( rc ) {
		printf("Failed to initailize socket, rc = %d", rc ) ;
		goto error ;
	}
	rc = sock.bind_listen () ;
	if ( rc ) {
		printf("Failed to bind/listen socket, rc = %d", rc ) ;
		goto error ;
	}
	// master loop for tcp listener
	while ( true ) {
		int s ;
		rc = sock.accept ( &s, NULL, NULL ) ;
		// if we don't get anything from period of time, let's loop
		if ( GHDB_TIMEOUT == rc ) {
			rc = GHDB_OK ;
			continue ;
		}
		char buffer [1024] ;
		int size ;
		ossSocket sock1 ( &s ) ;
		sock1.disableNagle () ;
		do {
			rc = sock1.recv ( ( char*)&size, 4 ) ;
			if ( rc && rc != GHDB_TIMEOUT ) {
				printf("Failed to receive size, rc = %d", rc ) ;
				goto error ;
			}
		}
		while ( GHDB_TIMEOUT == rc ) ;
		do {
			rc = sock1.recv (&buffer[0], size - sizeof(int) ) ;
			if ( rc && rc != GHDB_TIMEOUT ) {
				printf("Failed to receive buffer, rc = %d", rc ) ;
				goto error ;
			}
		}
		while ( GHDB_TIMEOUT == rc ) ;
		printf("%s\n", buffer ) ;
		sock1.close ();
	}
	done :
		return rc ;
	error :
		switch ( rc ) {
			case GHDB_SYS :
				printf("System error occured") ;
				break;
			default :
				printf("Internal error") ;
		}
		goto done ;
}
