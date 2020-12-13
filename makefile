CC = g++
CFLAGS  = -g -Wall -std=c++11

all: main

main: examples/main.cpp
	$(CC) $(CFLAGS) -o main src/*.cpp examples/main.cpp

test: examples/test.cpp
	$(CC) $(CFLAGS) -o test src/*.cpp examples/test.cpp

clean:
	$(RM) *.exe
