#include "filterLock.hpp"

FilterLock::FilterLock (int num) {
  level = new int[num];
  victim = new int[num];
  n = num;
}

FilterLock::~FilterLock () {
    delete[] level;
    delete[] victim;
}

void FilterLock::lock() {
  int me = omp_get_thread_num();
  for (int i = 1; i < n; i++) {
    level[me] = i;
    victim[i] = me;

    for( int j = 0; j < n; j++) {
      while ((j != me) && (level[j] >= i && victim[i] == me)) {

      }
    }
  }
}

void FilterLock::unlock() {
  int me = omp_get_thread_num();
  level[me] = 0;
}
