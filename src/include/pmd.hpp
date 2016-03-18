/**************************************************
	Copyright (C) 2016 CHEN Gonghao.
	chengonghao@yeah.net
**************************************************/
#ifndef _PMD_HPP__
#define _PMD_HPP__

#include "core.hpp"

enum GHDB_DB_STATUS {
   GHDB_DB_NORMAL = 0,
   GHDB_DB_SHUTDOWN,
   GHDB_DB_PANIC
} ;

#define GHDB_IS_DB_NORMAL ( GHDB_DB_NORMAL == pmdGetKRCB()->getDBStatus () )
#define GHDB_IS_DB_DOWN   ( GHDB_DB_SHUTDOWN == pmdGetKRCB()->getDBStatus () || \
                           GHDB_DB_PANIC    == pmdGetKRCB()->getDBStatus () )
#define GHDB_IS_DB_UP     ( !GHDB_IS_DB_DOWN )

#define GHDB_SHUTDOWN_DB  { pmdGetKRCB()->setDBStatus(GHDB_DB_SHUTDOWN); }

class pmdOptions ;
class GHDB_KRCB {
   private :
	 // configured options
	char          _dataFilePath [ OSS_MAX_PATHSIZE + 1 ] ;
	char          _logFilePath  [ OSS_MAX_PATHSIZE + 1 ] ;
	int           _maxPool ;
	char          _svcName [ NI_MAXSERV + 1 ] ;
	GHDB_DB_STATUS _dbStatus ;
   public :
   	// constructor
   	GHDB_KRCB () {
      		_dbStatus = GHDB_DB_NORMAL ;
      		memset ( _dataFilePath, 0, sizeof(_dataFilePath) ) ;
      		memset ( _logFilePath, 0, sizeof(_logFilePath) ) ;
      		_maxPool = 0 ;
      		memset ( _svcName, 0, sizeof(_svcName) ) ;
   	}
   	// destructor
   	~GHDB_KRCB () {}

   	// inline function
   	// get database status
   	inline GHDB_DB_STATUS getDBStatus () {
      		return _dbStatus ;
   	}

   	// get data file path
   	inline const char *getDataFilePath () {
      		return _dataFilePath ;
   	}

   	// get log file path
   	inline const char *getLogFilePath () {
      		return _logFilePath ;
   	}

   	// get service name
   	inline const char *getSvcName () {
    	 	return _svcName ;
   	}

   	// get max thread pool
   	inline int getMaxPool () {
     		return _maxPool ;
   	}

   	// setup database status
   	inline void setDBStatus ( GHDB_DB_STATUS status ) {
      		_dbStatus = status ;
   	}

   	// set data file path
   	void setDataFilePath ( const char *pPath ) {
      		strncpy ( _dataFilePath, pPath, sizeof(_dataFilePath) ) ;
   	}

   	// set log file path
   	void setLogFilePath ( const char *pPath ) {
      		strncpy ( _logFilePath, pPath, sizeof(_logFilePath) ) ;
   	}

   	// set service name
   	void setSvcName ( const char *pName ) {
      		strncpy ( _svcName, pName, sizeof(_svcName) ) ;
   	}

   	// set max pool
   	void setMaxPool ( int maxPool ) {
      		_maxPool = maxPool ;
   	}

   	// setup from pmdOptions
   	int init ( pmdOptions *options ) ;
} ;

extern GHDB_KRCB pmd_krcb ;

inline GHDB_KRCB *pmdGetKRCB() {
   return &pmd_krcb ;
}

#endif
