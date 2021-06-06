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
	}
	n = numThreads;
}

BoulangerieLock::~BoulangerieLock () {
	delete[] choosing;
	delete[] number;
}

void BoulangerieLock::lock () {
	
	bool tmp_c = false;
	int prev_n = -1;
	int tmp_n = -1;
	int limit = n;
	int i = omp_get_thread_num();

	choosing[i] = true;
	number[i] = findMax() + 1;
	choosing[i] = false;

	// Optimizing for lower contention
	if(number[i]==1)
		limit = i;

	for (int j = 0; j < limit; j++) {
		// If the thread j is the current thread go the next thread.
		if (j == i)
			continue;

		//taking advantage of inconsistent reads
		do{ 
			tmp_c = choosing[j];
		} while(tmp_c);			

		tmp_n = -1;

		do{
			prev_n = tmp_n; 
			tmp_n = number[j];
		} while (tmp_n != 0 && (number[i] > tmp_n || (number[i] == tmp_n && i > j)) && (tmp_n == prev_n || prev_n == -1));	 
	} 
}

void BoulangerieLock::unlock () {
	number[omp_get_thread_num()] = false;
}

int BoulangerieLock::findMax() {
	int m = number[0];
	for (int k=1; k<n; ++k) {
		if (number[k] > m)
			m = number[k];
	}
	return m;
}