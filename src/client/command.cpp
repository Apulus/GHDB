/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#include "core.hpp"
#include "command.hpp"
#include "commandFactory.hpp"
#include "pd.hpp"
#include "msg.hpp"

COMMAND_BEGIN
COMMAND_ADD(COMMAND_INSERT,InsertCommand)
COMMAND_ADD(COMMAND_QUERY,QueryCommand)
COMMAND_ADD(COMMAND_DELETE, DeleteCommand)
COMMAND_ADD(COMMAND_CONNECT,ConnectCommand)
COMMAND_ADD(COMMAND_QUIT, QuitCommand)
COMMAND_ADD(COMMAND_HELP, HelpCommand)
COMMAND_ADD(COMMAND_SNAPSHOT, SnapshotCommand)
COMMAND_END

extern int gQuit ;

int ICommand::execute ( ossSocket &sock, std::vector<std::string> &argVec ) {
	return GHDB_OK ;
}

int ICommand::getError ( int code ) {
	switch ( code ) {
		case GHDB_OK :
			break ;
		case GHDB_IO :
			std::cout << "io error is occurred." << std::endl ;
			break ;
		case GHDB_INVALIDARG :
			std::cout << "invalid argument." << std::endl ;
			break ;
		case GHDB_PERM :
			std::cout << "ghdb_perm" << std::endl ;
			break ;
		case GHDB_OOM :
			std::cout << "ghdb_oom" << std::endl ;
			break ;
		case GHDB_SYS :
			std::cout << "system error is occurred." << std::endl ;
			break ;
		case GHDB_QUIESCED :
			std::cout << "GHDB_QUIESCED" << std::endl ;
			break ;
		case GHDB_NETWORK_CLOSE :
			std::cout << "network is closed." << std::endl ;
			break ;
		case GHDB_HEADER_INVALID :
			std::cout << "record header is not right." << std::endl ;
			break ;
		case GHDB_IXM_ID_EXIST :
			std::cout << "record key is exist." << std::endl ;
			break ;
		case GHDB_IXM_ID_NOT_EXIST :
			std::cout << "record key is not exist." << std::endl ;
			break ;
		case GHDB_NO_ID :
			std::cout << "_id is needed" << std::endl ;
			break ;
		case GHDB_QUERY_INVALID_ARGUMENT :
			std::cout << "invalid query argument" << std::endl ;
			break ;
		case GHDB_INSERT_INVALID_ARGUMENT :
			std::cout << "invalid insert argument" << std::endl ;
			break ;
		case GHDB_DELETE_INVALID_ARGUMENT :
			std::cout << "invalid delete argument" << std::endl ;
			break ;
		case GHDB_INVALID_RECORD :
			std::cout << "invalid record string" << std::endl ;
			break ;
		case GHDB_SOCK_REMOTE_CLOSED :
			std::cout << "remote sock connection is closed" << std::endl ;
			break ;
		case GHDB_SOCK_NOT_CONNECT :
			std::cout << "sock connection does not exist" << std::endl ;
			break ;
		case GHDB_MSG_BUILD_FAILED :
			std::cout << "msg build faild" << std::endl ;
			break ;
		case GHDB_SOCK_SEND_FAILED :
			std::cout << "sock send msg failed" << std::endl ;
			break ;
		case GHDB_SOCK_INIT_FAILED :
			std::cout << "sock init failed" << std::endl ;
			break ;
		case GHDB_SOCK_CONNECT_FAILED :
			std::cout << "sock connect remote server failed" << std::endl ;
			break ;
		default :
			break ;
	}
	return code ;
}

int ICommand::recvReply ( ossSocket & sock ) {
	// define message data length
	int length = 0 ;
	int ret = GHDB_OK ;
	// fill receiv buffer with 0.
	memset ( _recvBuf, 0, RECV_BUF_SIZE ) ;
	if ( !sock.isConnected() ) 
		return getError( GHDB_SOCK_NOT_CONNECT ) ;
	while ( 1 ) {
		// receive data from the server. first receive the length of the data.
		ret = sock.recv( _recvBuf, sizeof(int) ) ;
		if ( GHDB_TIMEOUT == ret ) 
			continue ;
		else if ( GHDB_NETWORK_CLOSE == ret ) 
			return getError ( GHDB_SOCK_REMOTE_CLOSED ) ;
		else
			break ;
	}
	// get the value of length.
	length = *(int*)_recvBuf ;
	if ( length > RECV_BUF_SIZE ) 
		return getError( GHDB_RECV_DATA_LENGTH_ERROR ) ;
	// receive data from the server. second receive the last data.
	while ( 1 ) {
		ret = sock.recv( &_recvBuf[sizeof(int)], length - sizeof(int) ) ;
		if ( ret == GHDB_TIMEOUT ) 
			continue ;
		else if ( ret == GHDB_NETWORK_CLOSE ) 
			return getError ( GHDB_SOCK_REMOTE_CLOSED ) ;
		else
			break ;
	} 
	return ret ;
}

int ICommand::sendOrder ( ossSocket &sock, OnMsgBuild onMsgBuild ) {
	int ret = GHDB_OK ;
	bson::BSONObj bsonData ;
	try {
		bsonData = bson::fromjson( _jsonString ) ;
	}catch ( std::exception &e ) {
		return getError( GHDB_INVALID_RECORD ) ;
	}
	memset ( _sendBuf, 0, SEND_BUF_SIZE ) ;
	int size = SEND_BUF_SIZE ;
	char * pSendBuf = _sendBuf ;
	ret = onMsgBuild(&pSendBuf, &size, bsonData);
	if ( ret ) 
		return getError ( GHDB_MSG_BUILD_FAILED ) ;
	ret = sock.send( pSendBuf, *(int*)pSendBuf ) ;
	if ( ret ) 
		return getError ( GHDB_SOCK_SEND_FAILED ) ;
	return ret ;
}

int ICommand::sendOrder ( ossSocket &sock, int opCode ) {
	int ret = GHDB_OK ;
	memset ( _sendBuf, 0, SEND_BUF_SIZE ) ;
	char * pSendBuf = _sendBuf ;
	MsgHeader * header = (MsgHeader*)pSendBuf ;
		header->messageLen = sizeof(MsgHeader) ;
		header->opCode = opCode;
	ret = sock.send( pSendBuf, *(int*)pSendBuf ) ;
	return ret ;
}

/******************************InsertCommand********************************************/
int InsertCommand::handleReply() {
   	MsgReply * msg = (MsgReply*)_recvBuf;
   	int returnCode = msg->returnCode;
   	int ret = getError(returnCode);
   	return ret;
   	return GHDB_OK ;
}

int InsertCommand::execute( ossSocket & sock, std::vector<std::string> & argVec )
{
   	int rc = GHDB_OK;
   	if( argVec.size() <1 ) {
      		return getError(GHDB_INSERT_INVALID_ARGUMENT);
   	}
   	_jsonString = argVec[0];
     	if( !sock.isConnected() ) {
      		return getError(GHDB_SOCK_NOT_CONNECT);
   	}

   	rc = sendOrder( sock, msgBuildInsert );
   	//PD_RC_CHECK ( rc, PDERROR, "Failed to send order, rc = %d", rc ) ;

   	rc = recvReply( sock );
   	//PD_RC_CHECK ( rc, PDERROR, "Failed to receive reply, rc = %d", rc ) ;
   	rc = handleReply();
   	//PD_RC_CHECK ( rc, PDERROR, "Failed to receive reply, rc = %d", rc ) ;

	done :
   		return rc;
	error :
   		goto done ;
}

/******************************QueryCommand**********************************************/
int QueryCommand::handleReply() {
   	MsgReply * msg = (MsgReply*)_recvBuf;
   	int returnCode = msg->returnCode;
   	int ret = getError(returnCode);
   	if(ret) {
      		return ret;
   	}
   	if ( msg->numReturn ) {
      		bson::BSONObj bsonData = bson::BSONObj( &(msg->data[0]) );
      		std::cout << bsonData.toString() << std::endl;
   	}
   	return ret;
}

int QueryCommand::execute( ossSocket & sock, std::vector<std::string> & argVec ) {
   	int rc = GHDB_OK;
   	if( argVec.size() <1 ) {
      		return getError(GHDB_QUERY_INVALID_ARGUMENT);
   	}
   	_jsonString = argVec[0];
   	if( !sock.isConnected() ) {
      		return getError(GHDB_SOCK_NOT_CONNECT);
   	}

   	rc = sendOrder( sock, msgBuildQuery );
   	PD_RC_CHECK ( rc, PDERROR, "Failed to send order, rc = %d", rc ) ;
   	rc = recvReply( sock );
   	PD_RC_CHECK ( rc, PDERROR, "Failed to receive reply, rc = %d", rc ) ;
   	rc = handleReply();
   	PD_RC_CHECK ( rc, PDERROR, "Failed to receive reply, rc = %d", rc ) ;

	done :
   		return rc;
	error :
   		goto done ;
}

/******************************DeleteCommand**********************************************/
int DeleteCommand::handleReply() {
   	MsgReply * msg = (MsgReply*)_recvBuf;
   	int returnCode = msg->returnCode;
   	int ret = getError(returnCode);
   	return ret;
}

int DeleteCommand::execute( ossSocket & sock, std::vector<std::string> & argVec ) {
   	int rc = GHDB_OK;
   	if( argVec.size() < 1 ) {
      		return getError(GHDB_DELETE_INVALID_ARGUMENT);
   	}
   	_jsonString = argVec[0];
   	if( !sock.isConnected() ) {
      		return getError(GHDB_SOCK_NOT_CONNECT);
   	}
   	rc = sendOrder( sock, msgBuildDelete );
   	PD_RC_CHECK ( rc, PDERROR, "Failed to send order, rc = %d", rc ) ;
   	rc = recvReply( sock );
   	PD_RC_CHECK ( rc, PDERROR, "Failed to receive reply, rc = %d", rc ) ;
   	rc = handleReply();
   	PD_RC_CHECK ( rc, PDERROR, "Failed to receive reply, rc = %d", rc ) ;

	done :
   		return rc;
	error :
   		goto done ;
}

/*******************************ConnectCommand****************************************/
int ConnectCommand::execute ( ossSocket &sock, std::vector<std::string> &argVec ) {
	int ret = GHDB_OK ;
	_address = argVec[0] ;
	_port = atoi ( argVec[1].c_str() ) ;
	sock.close() ;
	sock.setAddress( _address.c_str(), _port ) ;
	ret = sock.initSocket() ;
	if ( ret ) 
		return getError ( GHDB_SOCK_INIT_FAILED ) ;
	ret = sock.connect() ;
	if ( ret ) 
		return getError ( GHDB_SOCK_CONNECT_FAILED ) ;
	sock.disableNagle() ;
	return ret ;
}

/*********************************QuitCommand*****************************************/
int QuitCommand::handleReply () {
	int ret = GHDB_OK ;
	gQuit = 1 ;
	return ret ;
}

int QuitCommand::execute( ossSocket &sock, std::vector<std::string> &argVec) {
	int ret = GHDB_OK ;
	if ( !sock.isConnected() ) {
		return getError ( GHDB_SOCK_NOT_CONNECT ) ;
	}
	ret = sendOrder ( sock, OP_DISCONNECT ) ;
	sock.close();
	ret = handleReply () ;
	return ret ;
}

/*********************************SnapshotCommand******************************************/
int SnapshotCommand::handleReply() {
   	int ret = GHDB_OK;
   	MsgReply * msg = (MsgReply*)_recvBuf;
   	int returnCode = msg->returnCode;
   	ret = getError(returnCode);
   	if(ret) {
      		return ret;
   	}
   	bson::BSONObj bsonData = bson::BSONObj( &(msg->data[0]) );
   	printf( "insert times is %d\n", bsonData.getIntField("insertTimes") );
   	printf( "del times is %d\n", bsonData.getIntField("delTimes") );
   	printf( "query times is %d\n", bsonData.getIntField("queryTimes") );
   	printf( "server run time is %dm\n", bsonData.getIntField("serverRunTime") );

   	return ret;
}

int SnapshotCommand::execute( ossSocket & sock, std::vector<std::string> &argVec) {
   	int rc = GHDB_OK;
   	if( !sock.isConnected() ) {
      		return getError(GHDB_SOCK_NOT_CONNECT);
   	}

   	rc = sendOrder( sock, OP_SNAPSHOT );
   	PD_RC_CHECK ( rc, PDERROR, "Failed to send order, rc = %d", rc ) ;
   	rc = recvReply( sock );
   	PD_RC_CHECK ( rc, PDERROR, "Failed to receive reply, rc = %d", rc ) ;
   	rc = handleReply();
   	PD_RC_CHECK ( rc, PDERROR, "Failed to receive reply, rc = %d", rc ) ;

	done :
   		return rc;
	error :
   		goto done ;
}

/**********************************HelpCommand****************************************/
int HelpCommand::execute ( ossSocket &sock, std::vector<std::string> &argVec ) {
	int ret = GHDB_OK ;
	printf("List of classes of commands:\n\n") ;
	printf("%s [server] [port]-- connecting ghdb server\n", COMMAND_CONNECT) ;
	printf("%s -- sending a insert command to ghdb server\n", COMMAND_INSERT) ;
	printf("%s -- sending a query command to ghdb server\n", COMMAND_QUERY) ;
	printf("%s -- sending a delete command to ghdb server\n", COMMAND_DELETE) ;
	printf("%s [number]-- sending a test command to ghdb server\n", COMMAND_TEST) ;
	printf("%s -- provideing current number of record inserting\n", COMMAND_SNAPSHOT) ;
	printf("%s -- quitting command\n\n", COMMAND_QUIT) ;
	printf("Type \"help\" command for help\n");
	return ret ;
}
