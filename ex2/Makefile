CXX=g++
CFLAGS=-std=c++11 -g -Wall -Wpedantic -Wfloat-equal
LDFLAGS=-lpthread -lm

BIN=sieve_of_eratosthenes_parallel

SRC=$(BIN).cpp

all: $(BIN)

%: %.cpp
	$(CXX) $(CFLAGS) -o $@ $< $(LDFLAGS)

check:
	cppcheck --enable=all  .

clean:
	rm -rf $(BIN)
