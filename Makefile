CC = g++
CFLAGS = -Wall -g -fopenmp -std=c++11

all: main

main: main.o FilterLock.o
	$(CC) $(CFLAGS) -o main  main.o FilterLock.o

main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp

FilterLock.o: locks/filterLock.hpp
	$(CC) $(CFLAGS) -c locks/filterLock.cpp

clean: 
	rm -rf *.o main