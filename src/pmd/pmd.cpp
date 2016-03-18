/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#include "pmd.hpp"
#include "pmdOptions.hpp"
#include "pd.hpp"

GHDB_KRCB pmd_krcb ;

extern char _pdDiagLogPath [ OSS_MAX_PATHSIZE+1 ] ;

int GHDB_KRCB::init ( pmdOptions *options ) {
   setDBStatus ( GHDB_DB_NORMAL ) ;
   setDataFilePath ( options->getDBPath () ) ;
   setLogFilePath ( options->getLogPath () ) ;
   strncpy ( _pdDiagLogPath, getLogFilePath(), sizeof(_pdDiagLogPath) ) ;
   setSvcName ( options->getServiceName () ) ;
   setMaxPool ( options->getMaxPool () ) ;
   // return _rtnMgr.rtnInitialize() ;
   return GHDB_OK ;
}
