/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#ifndef CORE_HPP__
#define CORE_HPP__

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<linux/limits.h>
#include<sys/time.h>
#include<time.h>
#include<stdarg.h>
#include<unistd.h>
#include<syscall.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<errno.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<netinet/tcp.h>
#include<sys/mman.h>

#include<string>
#include<map>
#include<set>
#include<vector>
#include<iostream>

#define OSS_MAX_PATHSIZE	PATH_MAX
#define OSS_FILE_SEP_STR	"/"
#define OSS_FILE_SEP_CHAR	*((const char*)OSS_FILE_SET_STR)[0]
#define OSS_NEWLINE		"\n"

//error code list
#define GHDB_OK				0
#define GHDB_IO				-1
#define GHDB_INVALIDARG			-2
#define GHDB_PERM			-3
#define GHDB_OOM			-4
#define GHDB_SYS			-5
#define GHDB_PMD_HELP_ONLY		-6
#define GHDB_PMD_FORCE_SYSTEM_EDU	-7
#define GHDB_TIMEOUT			-8
#define GHDB_QUIESCED			-9
#define GHDB_EDU_INVAL_STATUS		-10
#define GHDB_NETWORK			-11
#define GHDB_NETWORK_CLOSE		-12
#define GHDB_APP_FORCED			-13
#define GHDB_IXM_ID_NOT_EXIST		-14
#define GHDB_IXM_ID_EXIST		-15
#define GHDB_NO_ID			-17


#define GHDB_QUERY_INVALID_ARGUMENT	-101
#define GHDB_INSERT_INVALID_ARGUMENT	-102
#define GHDB_DELETE_INVALID_ARGUMENT	-103

#define GHDB_INVALID_RECORD		-104
#define GHDB_RECV_DATA_LENGTH_ERROR	-107

#define GHDB_SOCK_INIT_FAILED		-113
#define GHDB_SOCK_CONNECT_FAILED	-114
#define GHDB_SOCK_NOT_CONNECT		-115
#define GHDB_SOCK_REMOTE_CLOSED		-116
#define GHDB_SOCK_SEND_FAILED		-117

#define	GHDB_MSG_BUILD_FAILED		-119
#define GHDB_HEADER_INVALID		-120

#endif
