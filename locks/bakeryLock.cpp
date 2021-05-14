#include "bakeryLock.hpp"

BakeryLock::BakeryLock (int num) {
	flag = new bool[num];
	label = new long long[num];
	for (int i = 0; i < num; i ++) {
	  flag[i] = false;
	  label[i] = 0;
	}
	n = num;
}
BakeryLock::~BakeryLock () {
	delete[] flag;
	delete[] label;
}

void BakeryLock::lock () {
	int i = omp_get_thread_num();
	flag[i] = true;
	long long max = label[0];
	for (int j = 1; j < n; j ++) {
	  if (label[j] > max) {
	    max = label[j];
	  }
	}
	if (max == LLONG_MAX) {
	  std::cout << "ERROR: Label Value Overflow" << std::endl;
	  exit (-1);
	}
	label[i] = max + 1;

	for (int j = 0; j < n; j++) {
	  if (i == j) {
	    continue;
	  } else {
	    while (flag[j] && ((label[j] < label[i]) ||( label[i] == label[j] && j < i))) {};
	  } 
	}
}

void BakeryLock::unlock () {
	flag[omp_get_thread_num()] = false;
}

