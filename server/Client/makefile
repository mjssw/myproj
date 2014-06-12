OUT_FILE = client 
SRC_INCLUDE = ../NetLib
SRC_DIR = ../NetLib
CPP_FLAGS = -g -Wall -D_DEBUG
LD_FLAGS = -g -pthread -rdynamic
AR_FLAGS = -r
CC  = gcc
CPP = g++
LD  = ld
AR  = ar

ALL_CPPFILES = $(wildcard *.cpp)
OBJS = $(ALL_CPPFILES:.cpp=.o)

all:$(OUT_FILE)

$(OUT_FILE):$(OBJS)
	$(CPP) -o $(OUT_FILE) $(OBJS) $(LD_FLAGS) -L ../NetLib/ -lsgnet 
	
$(OBJS):%.o:%.cpp
	$(CPP) $(CPP_FLAGS) -I$(SRC_INCLUDE) -c $<

clean:
	rm -f $(OBJS) $(OUT_FILE)
