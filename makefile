CC = g++
CFLAGS  = -g -Wall

all: main

main: examples/main.cpp
	$(CC) $(CFLAGS) -o main src/*.cpp examples/main.cpp

test: examples/test.cpp
	$(CC) $(CFLAGS) -o main src/*.cpp examples/test.cpp

clean:
	$(RM) *.exe
