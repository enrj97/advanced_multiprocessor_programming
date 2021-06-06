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

#define NUM_LOCK_OP 128
#define NUM_LOCK_CYCLES 128

#define THREAD_INIT 2
#define THREAD_MAX 64
#define THREAD_INCREMENT 2

int runLock(std::ofstream& dataCollector, BaseLock *lock, int iteration, int nthreads)
{
	int counter = 0;
	int data[NUM_LOCK_OP][2];
	omp_set_num_threads(nthreads);
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();

	#pragma omp parallel shared(counter, data, start, end)
	{
		int tid = omp_get_thread_num();

		while(1) {
			lock->lock();
			end = std::chrono::high_resolution_clock::now();

			if (counter >= NUM_LOCK_OP) {
				lock->unlock();
				break;
			}

			data[counter][0] = tid;
			data[counter][1] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			counter++;
			lock->unlock();
		}
	}

	for (int i = 0; i < NUM_LOCK_OP; i++) {
		dataCollector
			<< "\"" << lock->get_name() << "\","
			<< iteration << ","
			<< i << ","
			<< nthreads << ","
			<< data[i][0] << ","
			<< data[i][1] << std::endl << std::flush;
	}

	return EXIT_SUCCESS;
}

int runMultipleLock(std::ofstream& dataCollector, BaseLock *lock, int nthreads)
{
	std::cout << "Running " << lock->get_name() << " for " << nthreads << " threads";

	for (int i = 0; i < NUM_LOCK_CYCLES; i++) {
		std::cout << ".";
		if (runLock(dataCollector, lock, i, nthreads) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
	}

	std::cout << " Done!" << std::endl;

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
	if (runMultipleLock(dataCollector, &lock, 2) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		BoulangerieLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		BakeryLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		FilterLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		LamportLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		TestAndSetLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		TestAndTestAndSetLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	dataCollector.close();

	return EXIT_SUCCESS;
}
