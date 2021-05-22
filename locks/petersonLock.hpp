#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include <climits>
#include "baseLock.hpp"

class PetersonLock: public BaseLock {
  private:
    bool* flag;
    int victim;

  public:
    PetersonLock(int n);
    ~PetersonLock ();
    void lock();
    void unlock();

    const char *get_name() {
      return "PetersonLock";
    };

};
