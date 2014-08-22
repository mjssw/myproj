OUT_FILE = client 
SRC_INCLUDE = ../../sglib
SRC_DIR = ../../sglib
CPP_FLAGS = -g -Wall -D_DEBUG
LD_FLAGS = -g -pthread -rdynamic
AR_FLAGS = -r
CC  = gcc
CPP = g++
LD  = ld
AR  = ar

INCLUDE_PROTO_MSG = ../../protomsg/msg
INCLUDE_PROTO_ROOT = ../../third_part/protobuf/include
INCLUDE_PROTOBUF = ../../third_part/protobuf/include/google/protobuf
INCLUDE_PROTOBUF_STUBS = ../../third_part/protobuf/include/google/protobuf/stubs

SRC_DIR_PROTO_MSG = $(INCLUDE_PROTO_MSG)

LIB_THRID_PATH = /usr/local/serverdeps
SEARCH_LIB_PATH = ".:..:/usr/lib:$(LIB_THRID_PATH)"

ALL_CPPFILES = $(wildcard *.cpp)
OBJS = $(ALL_CPPFILES:.cpp=.o)

all:$(OUT_FILE)

$(OUT_FILE):$(OBJS)
	$(CPP) -o $(OUT_FILE) $(OBJS) $(LD_FLAGS) -L ../../sglib/ -lsgnet 
	
$(OBJS):%.o:%.cpp
	$(CPP) $(CPP_FLAGS) -I$(SRC_INCLUDE) -c $<

clean:
	rm -f $(OBJS) $(OUT_FILE)
