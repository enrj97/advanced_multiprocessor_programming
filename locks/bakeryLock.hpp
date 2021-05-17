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

class BakeryLock : public BaseLock {
  private:
    bool * flag;
    long long * label;
    int n;
  public:
    BakeryLock(int n);
    ~BakeryLock ();
    void lock();
    void unlock();

    const char *get_name() {
      return "BakeryLock";
    };
};
