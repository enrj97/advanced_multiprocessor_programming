CC = g++
CFLAGS = -Wall -g -fopenmp -std=c++11

all: lock

lock: main.o filterLock.o bakeryLock.o lamportLock.o
	$(CC) $(CFLAGS) -o lock main.o filterLock.o bakeryLock.o lamportLock.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

filterLock.o: locks/filterLock.hpp
	$(CC) $(CFLAGS) -c locks/filterLock.cpp

bakeryLock.o: locks/bakeryLock.hpp
	$(CC) $(CFLAGS) -c locks/bakeryLock.cpp

lamportLock.o: locks/lamportLock.hpp
	$(CC) $(CFLAGS) -c locks/lamportLock.cpp

clean:
	rm -rf *.o lock
