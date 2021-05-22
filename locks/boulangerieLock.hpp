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

class BoulangerieLock: public BaseLock {
  private:
    bool* choosing;
	int* number;
	int* num;
	int n;
	int findMax();

  public:
    BoulangerieLock(int n);
    ~BoulangerieLock ();
    void lock();
    void unlock();

    const char *get_name() {
      return "BoulangerieLock";
    };
};
