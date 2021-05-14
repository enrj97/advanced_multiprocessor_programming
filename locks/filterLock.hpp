#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>

class FilterLock {
  
  public:
  int* level;
  int* victim;
  int n;

  FilterLock(int n);

  void lock();

  void unlock();

};