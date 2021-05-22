#include "petersonLock.hpp"

PetersonLock::PetersonLock (int n) {
	flag = new bool[2];
	for (int i = 0; i < 2; i ++) {
	  flag[i] = false;
	}
}

PetersonLock::~PetersonLock () {
	delete[] flag;
}

void PetersonLock::lock () {
	int i = omp_get_thread_num();
	int j = 1 - i;
	flag[i] = true;
	victim = i;
	while(flag[j] && victim == i) { /* wait */ }
}
	

void PetersonLock::unlock () {
	int i = omp_get_thread_num();
	flag[i] = false;
}
