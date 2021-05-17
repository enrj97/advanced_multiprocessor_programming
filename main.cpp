#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include "locks/filterLock.hpp"
#include "locks/bakeryLock.hpp"


int runLock(BaseLock *lock, int nthreads)
{
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	int counter=0, tid;

	omp_set_num_threads(nthreads); // Use 4 threads for all consecutive parallel region

	std::cout << "Running " << lock->get_name() << " for " << nthreads << " threads" << std::endl;

	#pragma omp parallel private(tid) shared(counter, start, end)
	{
		tid = omp_get_thread_num();

		lock->lock();
		std::cout << tid << std::endl;
		sleep(1);
		lock->unlock();
	}

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int nthreads;
	omp_set_dynamic(0);     // Explicitly disable dynamic teams

	for (nthreads = 2; nthreads <= 8; nthreads += 2) {
		BakeryLock lock(nthreads);
		if (runLock(&lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = 2; nthreads <= 8; nthreads += 2) {
		FilterLock lock(nthreads);
		if (runLock(&lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
