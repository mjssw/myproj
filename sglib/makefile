VER = Debug
ifeq ($(VER), Debug)
CPP_FLAGS = -g -Wall -D_DEBUG
LD_FLAGS = -g -pthread -rdynamic
else
CPP_FLAGS = -Wall -O3
LD_FLAGS = -pthread -rdynamic
endif

OUT_FILE = libsgnet.a
SRC_INCLUDE = ./
MYSQL_INCLUDE = ../third_part/mysql/include
TINYXML_INCLUDE = ../third_part/tinyxml/include
SRC_DIR = ./
AR_FLAGS = rcs
CC  = gcc
CPP = g++
LD  = ld
AR  = ar

ALL_CPPFILES = $(wildcard *.cpp)
OBJS = $(ALL_CPPFILES:.cpp=.o)

all:$(OUT_FILE)

$(OUT_FILE):$(OBJS)
	$(AR) $(AR_FLAGS) $(OUT_FILE) $(OBJS) 
	
$(OBJS):%.o:%.cpp
	$(CPP) $(CPP_FLAGS) -I$(SRC_INCLUDE) -I$(MYSQL_INCLUDE) -I$(TINYXML_INCLUDE) -c $<

clean:
	rm -f $(OBJS) $(OUT_FILE)
