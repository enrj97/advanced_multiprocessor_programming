#include "lamportLock.hpp"

/*		
 *		implementation by Lamport, resource:
 *		http://lamport.azurewebsites.net/pubs/bakery.pdf
 */

LamportLock::LamportLock (int num) {
	choosing = new bool[num];
	number = new int[num];
	for (int i = 0; i < num; i ++) {
	  choosing[i] = false;
	  number[i] = 0;
	}
	n = num;
}
LamportLock::~LamportLock () {
	delete[] choosing;
	delete[] number;
}

void LamportLock::lock () {
	int i = omp_get_thread_num();
	choosing[i] = true;
	// Find the max value and add 1 to get the next available ticket.
	number[i] = findMax() + 1;
	choosing[i] = false;

	for (int j = 0; j < n; j++) {
		// If the thread j is the current thread go the next thread.
		if (j == i)
			continue;
		
		// Wait if thread j is choosing right now.
		while (choosing[j]) { /* nothing */ }

		while (number[j] != 0 && (number[i] > number[j] || (number[i] == number[j] && i > j))) { /* nothing */ }		 
	} 
}

void LamportLock::unlock () {
	number[omp_get_thread_num()] = false;
}


int LamportLock::findMax() {
	int m = number[0];
	for (int i=1; i <n; ++i) {
		if (number[i] > m)
			m = number[i];
	}
	return m;
}