#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include "locks/filterLock.hpp"
#include "locks/bakeryLock.hpp"
#include "locks/lamportLock.hpp"

// 128K
std::stringstream writeBuffer;
std::ofstream dataCollector("locks.csv", std::ios::binary | std::ios::trunc);

int runLock(BaseLock *lock, int nthreads)
{
	int counter = 0;
	omp_set_num_threads(nthreads); // Use 4 threads for all consecutive parallel region

	std::cout << "Running " << lock->get_name() << " for " << nthreads << " threads" << std::endl;

	#pragma omp parallel shared(counter, writeBuffer)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
		int tid = omp_get_thread_num();

		start = std::chrono::high_resolution_clock::now();
		lock->lock();
		end = std::chrono::high_resolution_clock::now();

		writeBuffer
			<< lock->get_name() << ","
			<< nthreads << ","
			<< counter << ","
			<< tid << ","
			<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

		counter++;
		std::cout << tid << std::endl;
		lock->unlock();
	}

	dataCollector << writeBuffer.rdbuf() << std::flush;

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

	for (nthreads = 2; nthreads <= 8; nthreads += 2) {
		LamportLock lock(nthreads);
		if (runLock(&lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	dataCollector.close();

	return EXIT_SUCCESS;
}
