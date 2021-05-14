#include "filterLock.hpp"

FilterLock::FilterLock (int n) {
  level = new int[n];
  victim = new int[n];
  this -> n = n;
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