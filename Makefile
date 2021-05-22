CC = g++
CFLAGS = -Wall -g -fopenmp -std=c++11

all: lock

lock: main.o filterLock.o bakeryLock.o lamportLock.o boulangerieLock.o petersonLock.o
	$(CC) $(CFLAGS) -o lock main.o filterLock.o bakeryLock.o lamportLock.o boulangerieLock.o petersonLock.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

filterLock.o: locks/filterLock.hpp
	$(CC) $(CFLAGS) -c locks/filterLock.cpp

bakeryLock.o: locks/bakeryLock.hpp
	$(CC) $(CFLAGS) -c locks/bakeryLock.cpp

lamportLock.o: locks/lamportLock.hpp
	$(CC) $(CFLAGS) -c locks/lamportLock.cpp

boulangerieLock.o: locks/boulangerieLock.hpp
	$(CC) $(CFLAGS) -c locks/boulangerieLock.cpp

petersonLock.o: locks/petersonLock.hpp 
	$(CC) $(CFLAGS) -c locks/petersonLock.cpp	

clean:
	rm -rf *.o lock
