/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#include "commandFactory.hpp"

CommandFactory::CommandFactory(){
	addCommand ();
}

ICommand * CommandFactory::getCommandProcessor ( const char * pCmd ) {
	ICommand *pProcessor = NULL ;
	do {
		COMMAND_MAP::iterator iter ;
		iter = _cmdMap.find( pCmd ) ;
		if ( iter != _cmdMap.end() )
			pProcessor = iter -> second ;
	}while( 0 ) ;
	return pProcessor ;
}
