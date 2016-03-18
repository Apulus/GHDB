/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include "core.hpp"
#include <bson/src/util/json.h>
#include "ossSocket.hpp"
#define COMMAND_QUIT			"quit"
#define COMMAND_INSERT			"insert"
#define COMMAND_QUERY			"query"
#define COMMAND_DELETE			"delete"
#define COMMAND_HELP			"help"
#define COMMAND_CONNECT			"connect"
#define COMMAND_TEST			"test"
#define COMMAND_SNAPSHOT		"snapshot"

#define RECV_BUF_SIZE			4096
#define SEND_BUF_SIZE			4096

class ICommand {
   typedef int ( *OnMsgBuild ) ( char ** ppBuffer, int * pBufferSize, bson::BSONObj &obj ) ;
   public :
	virtual int	execute ( ossSocket &sock, std::vector<std::string> &argVec ) ;
	int 		getError ( int code ) ;
   protected :
	int 		recvReply ( ossSocket &sock ) ;
	int 		sendOrder ( ossSocket &sock, OnMsgBuild onMsgBuild ) ;
	int		sendOrder ( ossSocket &sock, int opCode ) ;
   protected :
	virtual int handReply () { return GHDB_OK ; }
   protected :
	char _recvBuf[RECV_BUF_SIZE] ;
	char _sendBuf[SEND_BUF_SIZE] ;
	std::string _jsonString ;
} ;

class ConnectCommand : public ICommand {
   public :
	int execute ( ossSocket &sock, std::vector<std::string> &argVec ) ;
   private :
	std::string _address ;
	int _port ;
} ;

class QuitCommand : public ICommand {
   public :
	int execute ( ossSocket &sock, std::vector<std::string> &argVec ) ;
   protected :
	int handleReply () ;
} ;

class HelpCommand : public ICommand {
   public :
	int execute ( ossSocket &sock, std::vector<std::string> &argVec ) ;
} ;

#endif
