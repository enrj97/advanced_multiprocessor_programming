#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include "baseLock.hpp"

class FilterLock : public BaseLock {
  private:
    int* level;
    int* victim;
    int n;

  public:
    FilterLock(int n);
    ~FilterLock ();
    void lock();
    void unlock();

    const char *get_name() {
      return "FilterLock";
    };
};
