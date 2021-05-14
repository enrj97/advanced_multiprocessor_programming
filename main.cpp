#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <omp.h>
#include "locks/filterLock.hpp"


FilterLock lock(4);

int main(int argc, char *argv[]){
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	int counter=0, tid, nthreads;

	omp_set_dynamic(0);     // Explicitly disable dynamic teams
	omp_set_num_threads(4); // Use 4 threads for all consecutive parallel region

	#pragma omp parallel private(tid) shared(counter, start, end)
	{
		tid = omp_get_thread_num();

		lock.lock();
		std::cout << tid << std::endl;
		sleep(1);
		lock.unlock();
	}


	return 0;
}