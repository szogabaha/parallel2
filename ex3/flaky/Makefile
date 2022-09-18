CXX=g++
CC=gcc
CFLAGS=-std=c++11 -g -Wall -Wpedantic -Werror -Wfloat-equal
LDFLAGS=-lpthread -lm
INCLUDES=./include

BIN=main

SRC=$(BIN).cpp

SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))

all: $(BIN)

sanitize: CFLAGS += -fsanitize=thread
sanitize: $(BIN)

std: CFLAGS += -DSTD=1
std: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) -I$(INCLUDES)
	rm -rf *.o

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ -I$(INCLUDES)

check:
	cppcheck --enable=all  .

clean:
	rm -rf $(BIN)
	rm -rf *.o