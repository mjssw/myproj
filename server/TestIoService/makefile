VER = Debug

PLATFORM = x64
OUT_FILE = testioservice

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
INCLUDE_NETLIB = ../../sglib

SRC_DIR_CURRENT = $(INCLUDE_SRC)

LIB_DIR_NETLIB = $(INCLUDE_NETLIB)
SEARCH_LIB_PATH = ".:..:/usr/lib"

OUT_DIR_P1 = ../$(PLATFORM)
OUT_DIR_P2 = $(OUT_DIR_P1)/linux
OUT_DIR_P3 = $(OUT_DIR_P2)/$(BUILD_MODE)
OUT_DIR = $(OUT_DIR_P3)/server

LIB_S_OBJ_NETLIB = sgnet

AR_FLAGS = rcs
CC  = gcc
CPP = g++
LD  = ld
AR  = ar

CURRENT_CPPFILES = $(notdir $(wildcard $(SRC_DIR_CURRENT)/*.cpp))
CURRENT_CPP_OBJS = $(CURRENT_CPPFILES:.cpp=.o)

all:PRE_BUILD_EVENT NET_LIB $(OUT_FILE) PUBLISH 

PRE_BUILD_EVENT:
	if [ ! -d $(OUT_DIR_P1) ]; then sudo mkdir $(OUT_DIR_P1); fi
	if [ ! -d $(OUT_DIR_P2) ]; then sudo mkdir $(OUT_DIR_P2); fi
	if [ ! -d $(OUT_DIR_P3) ]; then sudo mkdir $(OUT_DIR_P3); fi
	if [ ! -d $(OUT_DIR) ]; then sudo mkdir $(OUT_DIR); fi

PUBLISH:
	sudo cp $(OUT_FILE) $(OUT_DIR)/

NET_LIB:
	cd $(LIB_DIR_NETLIB) && make VER=$(VER)

$(OUT_FILE):$(CURRENT_CPP_OBJS)
	$(CPP) -o $@ $^ $(LD_FLAGS) -L$(LIB_DIR_NETLIB) -l$(LIB_S_OBJ_NETLIB) -Wl,-rpath=$(SEARCH_LIB_PATH)
	
$(CURRENT_CPP_OBJS):%.o:$(SRC_DIR_CURRENT)/%.cpp
	$(CPP) $(CPP_FLAGS) -I$(INCLUDE_SRC) -I$(INCLUDE_NETLIB) -c $<
	
clean:
	rm -f $(CURRENT_CPP_OBJS) $(OUT_FILE)
	cd $(LIB_DIR_NETLIB) && make clean VER=$(VER)
	cd $(OUT_DIR) && rm -f $(OUT_FILE)
    

