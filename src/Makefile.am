AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=gonghaodb ghdb
gonghaodb_SOURCES=\
   pmd/pmdMain.cpp pmd/pmdTcpListener.cpp pmd/pmdOptions.cpp  \
   pmd/pmd.cpp						\
   bson/src/bsonobj.cpp bson/src/util/json.cpp bson/src/oid.cpp \
   bson/src/lib/base64.cpp bson/src/lib/md5.cpp bson/src/lib/nonce.cpp \
   oss/ossSocket.cpp oss/ossPrimitiveFileOp.cpp                        \
   pd/pd.cpp

ghdb_SOURCES=\
   client/ghdb.cpp client/command.cpp client/commandFactory.cpp \
   bson/src/bsonobj.cpp bson/src/util/json.cpp bson/src/oid.cpp \
   bson/src/lib/base64.cpp bson/src/lib/md5.cpp bson/src/lib/nonce.cpp \
   oss/ossSocket.cpp oss/ossPrimitiveFileOp.cpp                        \
   pd/pd.cpp

gonghaodb_CXXFLAGS=-I../boost -Ibson/src -Iinclude -D_FILE_OFFSET_BITS=64 -ggdb -Wall -O0
gonghaodb_LDADD=-lpthread -lm -lboost_system -lboost_thread -lboost_program_options -lrt
gonghaodb_LDFLAGS=-fPIC -rdynamic -L../boost/stage/lib -pthread

ghdb_CXXFLAGS=-I../boost -Ibson/src -Iinclude -D_FILE_OFFSET_BITS=64 -ggdb -Wall -O0
ghdb_LDADD=-lpthread -lm -lboost_system -lboost_thread -lrt
ghdb_LDFLAGS=-fPIC -rdynamic -L../boost/stage/lib
