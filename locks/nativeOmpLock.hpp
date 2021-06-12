#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include <climits>

class NativeOmpLock: public BaseLock  {
  private:
    omp_lock_t lck;

  public:
    NativeOmpLock();
    ~NativeOmpLock();
    void lock();
    void unlock();

    const char *get_name() {
      return "NativeOmpLock";
    };
};
