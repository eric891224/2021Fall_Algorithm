
.PHONY: all clean run
CC = g++
CFLAGS= -c -std=c++11 
LDFLAGS=-lm -Wall -O3
TARGET = bin/cb

all: $(TARGET)

bin/cb : bin/cb.o 
	$(CC) bin/cb.o -o bin/cb -O3

bin/cb.o : src/cb.cpp 
	$(CC) src/cb.cpp -o bin/cb.o $(CFLAGS) $(LDFLAGS) -Iinc 

clean:
	rm -f cb *.o