CC = g++
CFLAGS = -Wall -g -fopenmp -std=c++11

all: locks

locks: main.o FilterLock.o BakeryLock.o
	$(CC) $(CFLAGS) -o locks  main.o filterLock.o bakeryLock.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

filterLock.o: locks/filterLock.hpp
	$(CC) $(CFLAGS) -c locks/filterLock.cpp

bakeryLock.o: locks/bakeryLock.hpp
	$(CC) $(CFLAGS) -c locks/bakeryLock.cpp

clean:
	rm -rf *.o locks
