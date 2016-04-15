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

#define GHDB_QUERY_INVALID_ARGUMENT     -101
#define GHDB_INSERT_INVALID_ARGUMENT    -102
#define GHDB_DELETE_INVALID_ARGUMENT    -103

#define GHDB_INVALID_RECORD             -104
#define GHDB_RECV_DATA_LENGTH_ERROR     -107

#define GHDB_SOCK_INIT_FAILED           -113
#define GHDB_SOCK_CONNECT_FAILED        -114
#define GHDB_SOCK_NOT_CONNECT           -115
#define GHDB_SOCK_REMOTE_CLOSED         -116
#define GHDB_SOCK_SEND_FAILD            -117

#define EDB_MSG_BUILD_FAILED           -119

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

class InsertCommand : public ICommand {
   public:
      	int   execute( ossSocket & sock, std::vector<std::string> & argVec );
   protected:
     	int   handleReply () ;
};

class QueryCommand : public ICommand {
   public: 
	int execute ( ossSocket & sock , std::vector<std::string> & argVec ) ;
   protected:
	int handleReply () ;
} ;

class DeleteCommand : public ICommand {
   public: 
	int execute ( ossSocket & sock , std::vector<std::string> & argVec ) ;
   protected:
	int handleReply () ;
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

class SnapshotCommand : public ICommand {
   public: 
	int execute ( ossSocket & sock , std::vector<std::string> & argVec ) ;
   protected:
	int handleReply () ;
} ;
#endif
