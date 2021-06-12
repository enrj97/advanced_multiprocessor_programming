#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include "baseLock.hpp"

class NativeOmpLock: public BaseLock  {
  private:
    omp_lock_t lck;

  public:
    NativeOmpLock(int n);
    ~NativeOmpLock();
    void lock();
    void unlock();

    const char *get_name() {
      return "NativeOmpLock";
    };
};
