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
#include "locks/boulangerieLock.hpp"
#include "locks/petersonLock.hpp"
#include "locks/tas.hpp"
#include "locks/ttas.hpp"

#define NUM_ITERATIONS 1024

#define THREAD_INIT 2
#define THREAD_MAX 64
#define THREAD_INCREMENT 2

int runLock(std::ofstream& dataCollector, BaseLock *lock, int nthreads)
{
	int counter = 0;
	int data[NUM_ITERATIONS][3];
	omp_set_num_threads(nthreads);

	std::cout << "Running " << lock->get_name() << " for " << nthreads << " threads" << std::endl;

	#pragma omp parallel shared(counter, data)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
		int tid = omp_get_thread_num();

		start = std::chrono::high_resolution_clock::now();
		while(1) {
			lock->lock();
			end = std::chrono::high_resolution_clock::now();

			if (counter >= NUM_ITERATIONS) {
				lock->unlock();
				break;
			}

			data[counter][0] = nthreads;
			data[counter][1] = tid;
			data[counter][2] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			counter++;
			lock->unlock();
		}
	}

	for (int i = 0; i < NUM_ITERATIONS; i++) {
		dataCollector
			<< "\"" << lock->get_name() << "\","
			<< i << ","
			<< data[i][0] << ","
			<< data[i][1] << ","
			<< data[i][2] << std::endl << std::flush;
	}

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int nthreads;

	if (argc != 2) {
		printf("Usage: lock [out filename]\n");
		exit(EXIT_FAILURE);
	}

	std::ofstream dataCollector(argv[1], std::ios::binary | std::ios::trunc);

	omp_set_dynamic(0);     // Explicitly disable dynamic teams


	PetersonLock lock(2);
	if (runLock(dataCollector, &lock, 2) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		BoulangerieLock lock(nthreads);
		if (runLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		BakeryLock lock(nthreads);
		if (runLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		FilterLock lock(nthreads);
		if (runLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		LamportLock lock(nthreads);
		if (runLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		TestAndSetLock lock(nthreads);
		if (runLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		TestAndTestAndSetLock lock(nthreads);
		if (runLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	dataCollector.close();

	return EXIT_SUCCESS;
}
