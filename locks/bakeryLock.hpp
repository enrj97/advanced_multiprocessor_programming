#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include <climits>

class BakeryLock {
  
  public:
  bool * flag;
  long long * label;
  int n;

  BakeryLock(int n);
  ~BakeryLock ();
  void lock();
  void unlock();

};