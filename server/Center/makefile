VER = Debug

PLATFORM = x64
OUT_FILE = center

ifeq ($(VER), Debug)
BUILD_MODE = Debug
CPP_FLAGS = -g -Wall -D_DEBUG
LD_FLAGS = -g -pthread
else
BUILD_MODE = Release
CPP_FLAGS = -Wall -O2
LD_FLAGS = -pthread
endif

INCLUDE_SRC = .
INCLUDE_SERVER_BASE = ../ServerBase
INCLUDE_PROTO_MSG = ../../protomsg/msg
INCLUDE_MYSQL = ../../third_part/mysql/include
INCLUDE_TINYXML = ../../third_part/tinyxml/include
INCLUDE_NETLIB = ../../sglib
INCLUDE_APR = ../../third_part/log4cxx/linux/apr/include
INCLUDE_APR_UTIL = ../../third_part/log4cxx/linux/apr-util/include
INCLUDE_LOG4CXX = ../../third_part/log4cxx/linux/log4cxx/include
INCLUDE_PROTO_ROOT = ../../third_part/protobuf/include
INCLUDE_PROTOBUF = ../../third_part/protobuf/include/google/protobuf
INCLUDE_PROTOBUF_STUBS = ../../third_part/protobuf/include/google/protobuf/stubs

SRC_DIR_CURRENT = $(INCLUDE_SRC)
SRC_DIR_SERVER_BASE = $(INCLUDE_SERVER_BASE)
SRC_DIR_PROTO_MSG = $(INCLUDE_PROTO_MSG)

LIB_DIR_NETLIB = $(INCLUDE_NETLIB)
LIB_DIR_MYSQL = /usr/lib
LIB_THRID_PATH = /usr/local/serverdeps
#LIB_DIR_MYSQL = ../MySql/lib/linux
#LIB_THRID_PATH = ../linux/$(PLATFORM)
#LIB_DIR_TINYXML = ../tinyxml/lib/linux/$(PLATFORM)/$(BUILD_MODE)
#LIB_DIR_PROTOBUF = ../protobuf-2.5.0/lib/linux/$(PLATFORM)
#LIB_DIR_LOG4CXX = ../log4cxx/lib/linux/$(PLATFORM)
SEARCH_LIB_PATH = ".:..:/usr/lib:$(LIB_THRID_PATH)"

OUT_DIR_P1 = ../$(PLATFORM)
OUT_DIR_P2 = $(OUT_DIR_P1)/linux
OUT_DIR_P3 = $(OUT_DIR_P2)/$(BUILD_MODE)
OUT_DIR = $(OUT_DIR_P3)/server
CONFIG_DIR = serverconfig
NETCFG_FILE = netconfig.xml

LIB_S_OBJ_NETLIB = sgnet
LIB_S_OBJ_TINYXML = tinyxml
LIB_D_OBJ_MYSQL = mysqlclient
LIB_D_OBJ_PROTOBUF = protobuf
LIB_D_OBJ_LOG4CXX = log4cxx

AR_FLAGS = rcs
CC  = gcc
CPP = g++
LD  = ld
AR  = ar

CURRENT_CPPFILES = $(notdir $(wildcard $(SRC_DIR_CURRENT)/*.cpp))
SERVER_BASE_CPPFILES = $(notdir $(wildcard $(SRC_DIR_SERVER_BASE)/*.cpp))
MSG_CCFILES = $(notdir $(wildcard $(SRC_DIR_PROTO_MSG)/*.cc))
CURRENT_CPP_OBJS = $(CURRENT_CPPFILES:.cpp=.o)
SERVER_BASE_CPP_OBJS = $(SERVER_BASE_CPPFILES:.cpp=.o)
MSG_CC_OBJS = $(MSG_CCFILES:.cc=.o)

all:PRE_BUILD_EVENT NET_LIB $(OUT_FILE) PUBLISH 

display:
	echo $(CURRENT_CPP_OBJS)
	echo $(SERVER_BASE_CPP_OBJS)
	echo $(MSG_CC_OBJS)

PRE_BUILD_EVENT:
	if [ ! -d $(OUT_DIR_P1) ]; then sudo mkdir $(OUT_DIR_P1); fi
	if [ ! -d $(OUT_DIR_P2) ]; then sudo mkdir $(OUT_DIR_P2); fi
	if [ ! -d $(OUT_DIR_P3) ]; then sudo mkdir $(OUT_DIR_P3); fi
	if [ ! -d $(OUT_DIR) ]; then sudo mkdir $(OUT_DIR); fi

PUBLISH:
	sudo cp $(OUT_FILE) $(OUT_DIR)/
	if [ ! -d "$(OUT_DIR_P3)/$(CONFIG_DIR)" ]; then sudo mkdir "$(OUT_DIR_P3)/$(CONFIG_DIR)"; fi
	sudo cp ../$(CONFIG_DIR)/*.* "$(OUT_DIR_P3)/$(CONFIG_DIR)/"
	sudo cp "../$(NETCFG_FILE)" "$(OUT_DIR_P3)/"

NET_LIB:
	cd $(LIB_DIR_NETLIB) && make VER=$(VER)

$(OUT_FILE):$(CURRENT_CPP_OBJS) $(SERVER_BASE_CPP_OBJS) $(MSG_CC_OBJS)
	$(CPP) -o $@ $^ $(LD_FLAGS) -L$(LIB_DIR_NETLIB) -l$(LIB_S_OBJ_NETLIB) -L$(LIB_THRID_PATH) -l$(LIB_S_OBJ_TINYXML) -L$(LIB_THRID_PATH) -l$(LIB_D_OBJ_PROTOBUF) -L$(LIB_DIR_MYSQL) -l$(LIB_D_OBJ_MYSQL) -L$(LIB_THRID_PATH) -l$(LIB_D_OBJ_LOG4CXX) -Wl,-rpath=$(SEARCH_LIB_PATH)
	
$(CURRENT_CPP_OBJS):%.o:$(SRC_DIR_CURRENT)/%.cpp
	$(CPP) $(CPP_FLAGS) -I$(INCLUDE_SRC) -I$(INCLUDE_SERVER_BASE) -I$(INCLUDE_PROTO_ROOT) -I$(INCLUDE_PROTO_MSG) -I$(INCLUDE_MYSQL) -I$(INCLUDE_TINYXML) -I$(INCLUDE_NETLIB) -I$(INCLUDE_APR) -I$(INCLUDE_APR_UTIL) -I$(INCLUDE_LOG4CXX) -I$(INCLUDE_PROTOBUF) -I$(INCLUDE_PROTOBUF_STUBS) -c $<
	
$(SERVER_BASE_CPP_OBJS):%.o:$(SRC_DIR_SERVER_BASE)/%.cpp
	$(CPP) $(CPP_FLAGS) -I$(INCLUDE_SRC) -I$(INCLUDE_SERVER_BASE) -I$(INCLUDE_PROTO_ROOT) -I$(INCLUDE_PROTO_MSG) -I$(INCLUDE_MYSQL) -I$(INCLUDE_TINYXML) -I$(INCLUDE_NETLIB) -I$(INCLUDE_APR) -I$(INCLUDE_APR_UTIL) -I$(INCLUDE_LOG4CXX) -I$(INCLUDE_PROTOBUF) -I$(INCLUDE_PROTOBUF_STUBS) -c $<

$(MSG_CC_OBJS):%.o:$(SRC_DIR_PROTO_MSG)/%.cc
	$(CPP) $(CPP_FLAGS) -I$(INCLUDE_SRC) -I$(INCLUDE_SERVER_BASE) -I$(INCLUDE_PROTO_ROOT) -I$(INCLUDE_PROTO_MSG) -I$(INCLUDE_MYSQL) -I$(INCLUDE_TINYXML) -I$(INCLUDE_NETLIB) -I$(INCLUDE_APR) -I$(INCLUDE_APR_UTIL) -I$(INCLUDE_LOG4CXX) -I$(INCLUDE_PROTOBUF) -I$(INCLUDE_PROTOBUF_STUBS) -c $<

clean:
	rm -f $(CURRENT_CPP_OBJS) $(SERVER_BASE_CPP_OBJS) $(MSG_CC_OBJS) $(OUT_FILE)
	cd $(LIB_DIR_NETLIB) && make clean VER=$(VER)
	cd $(OUT_DIR) && rm -f $(OUT_FILE)
    

