#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include "baseLock.hpp"

class LamportLock: public BaseLock {
  
  private:
	bool* choosing;
	int* number;
	int n;
	int findMax();
  public:
	LamportLock(int n);
	~LamportLock ();
	void lock();
	void unlock();

  const char *get_name() {
      return "LamportLock";
   };

};