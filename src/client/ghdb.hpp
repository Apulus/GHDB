/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#ifndef _GHDB_HPP_
#define _GHDB_HPP_

#include "core.hpp"
#include "ossSocket.hpp"
#include "commandFactory.hpp"

const int CMD_BUFFER_SIZE = 512 ;
class Ghdb {
   public :
	Ghdb () {}
	~Ghdb () {} ;
   public :
	void	start ( void ) ;
   protected :
	void	prompt ( void ) ;
   private :
	void 	split ( const std::string &text, char delim, std::vector<std::string> &result ) ;
	char* 	readLine ( char *p, int length ) ;
	int 	readInput ( const char *pPrompt, int numIndent ) ;
   private :
	ossSocket 	_sock ;
	CommandFactory 	_cmdFactory ;
	char 		_cmdBuffer [ CMD_BUFFER_SIZE ] ;
}; 

#endif
