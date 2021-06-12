CC = g++
CFLAGS = -Wall -g -fopenmp -std=c++11

all: lock

lock: main.o filterLock.o bakeryLock.o lamportLock.o boulangerieLock.o petersonNode.o tas.o ttas.o petersonLock.o nativeOmpLock.o
	$(CC) $(CFLAGS) -o lock main.o filterLock.o bakeryLock.o lamportLock.o boulangerieLock.o petersonNode.o tas.o ttas.o petersonLock.o nativeOmpLock.o

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

petersonNode.o: locks/petersonNode.hpp 
	$(CC) $(CFLAGS) -c locks/petersonNode.cpp	

petersonLock.o: locks/petersonLock.hpp 
	$(CC) $(CFLAGS) -c locks/petersonLock.cpp	

tas.o: locks/tas.hpp 
	$(CC) $(CFLAGS) -c locks/tas.cpp	

ttas.o: locks/ttas.hpp 
	$(CC) $(CFLAGS) -c locks/ttas.cpp

nativeOmpLock: locks/nativeOmpLock.hpp	
	$(CC) $(CFLAGS) -c locks/nativeOmpLock.cpp

clean:
	rm -rf *.o lock
