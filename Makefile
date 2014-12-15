CC=g++
BIN=./bin
INCLUDE=./include
CFLAGS= -g -std=c++0x -lboost_system -lboost_thread

all: testStack

testStack:
	$(CC) test/testStack.cpp src/WorkerPool.cpp $(CFLAGS) -o $(BIN)/testStack -I$(INCLUDE)

clean:
	rm -f $(BIN)/*
