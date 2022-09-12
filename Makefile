CXX=g++
CC=gcc
CFLAGS=-std=c11 -g -Wall -Wpedantic -Werror
LDFLAGS=-lpthread -lm

BIN=numerical_integration

SRC=$(BIN).c

all: $(BIN)

sanitize: CFLAGS += -fsanitize=thread
sanitize: $(BIN)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(BIN)