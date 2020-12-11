CC = g++
CFLAGS  = -g -Wall

TARGET = main

all: $(TARGET)

main: examples/main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) src/*.cpp examples/main.cpp

clean:
	$(RM) $(TARGET).exe
