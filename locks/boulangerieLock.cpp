/*
 *	Boulangerie lock from:
 *	https://doi.org/10.1016/j.tcs.2016.12.015
*/
#include "boulangerieLock.hpp"

BoulangerieLock::BoulangerieLock (int numThreads) {
	choosing = new bool[numThreads];
	number = new int[numThreads];
	num = new int[numThreads];
	for (int i = 0; i < numThreads; i ++) {
	  choosing[i] = false;
	  number[i] = 0;
	  num[i] = 0;
	}
	n = numThreads;
}

BoulangerieLock::~BoulangerieLock () {
	delete[] choosing;
	delete[] number;
	delete[] num;
}

void BoulangerieLock::lock () {
	
	bool tmp_c = false;
	int *prev_n = nullptr;
	int *tmp_n = nullptr;
	int limit = n;
	int i = omp_get_thread_num();

	choosing[i] = true;
	for(int j=0; j<n; j++){
		num[i]=number[j];
	}
	num[i] = findMax() + 1;
	number[i] = num[i];
	choosing[i] = false;

	// Optimizing for lower contention
	if(number[i]==1){
		limit = i;
	}
	else{
		limit = n;
	}

	for (int j = 0; j < limit; j++) {
		// If the thread j is the current thread go the next thread.
		if (j == i)
			continue;

		//taking advantage of inconsistent reads
		do{ 
			tmp_c = choosing[j];
		} 
		while(tmp_c);			

		tmp_n = nullptr;

		do{
			prev_n = tmp_n; 
			tmp_n = &number[j];
		}
		while (*tmp_n != 0 && (num[i] > *tmp_n || (num[i] == *tmp_n && i > j)) && (tmp_n == prev_n || prev_n == nullptr));	 
	} 
}

void BoulangerieLock::unlock () {
	int i = omp_get_thread_num();
	num[i] = false;
	number[i] = false;
}

int BoulangerieLock::findMax() {
	int m = num[0];
	for (int k=1; k<n; ++k) {
		if (num[k] > m)
			m = num[k];
	}
	return m;
}