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

#define THREAD_INIT 2
#define THREAD_MAX 64
#define THREAD_INCREMENT 2

int runLock (
	std::ofstream& dataCollector,
	BaseLock *lock,
	int iteration,
	int nthreads,
	int lock_iterations,
	int cs_iterations
) {
	int counter = 0, duration;
	int thread_counter[THREAD_MAX] = {0};
	omp_set_num_threads(nthreads);

	#pragma omp parallel shared(counter, thread_counter, duration, lock_iterations)
	{
		int tid = omp_get_thread_num();
		std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

		#pragma omp barrier
		start = std::chrono::high_resolution_clock::now();

		while(counter <= lock_iterations) {
			lock->lock();
			counter++;

			if (counter == lock_iterations) {
				end = std::chrono::high_resolution_clock::now();
				duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			}

			if (counter >= lock_iterations) {
				lock->unlock();
				break;
			}

			for(int cs = 0; cs < cs_iterations; cs++) {/*wait*/}

			thread_counter[tid]++;

			lock->unlock();
		}
	}

	dataCollector
		<< "\"" << lock->get_name() << "\","
		<< iteration << ","
		<< nthreads << ","
		<< cs_iterations << ","
		<< duration;

	for (int i = 0; i < nthreads; i++) {
		dataCollector << "," << thread_counter[i];
	}

	dataCollector << std::endl << std::flush;

	return EXIT_SUCCESS;
}

int runMultipleLock (
	std::ofstream& dataCollector,
	BaseLock *lock,
	int nthreads,
	int loop_iterations,
	int lock_iterations,
	int cs_iterations
) {
	std::cout << "Running " << lock->get_name() << " for " << nthreads << " threads";

	for (int i = 0; i < loop_iterations; i++) {
		std::cout << ".";
		if (runLock(dataCollector, lock, i, nthreads, lock_iterations, cs_iterations) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
	}

	std::cout << " Done!" << std::endl;

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int nthreads, lock_iterations, loop_iterations, cs_iterations;

	if (argc != 5) {
		printf("Usage: lock [out filename] [outer iterations] [inner iterations] [cs iterations]\n");
		exit(EXIT_FAILURE);
	}

	// Argument parsing
	std::ofstream dataCollector(argv[1], std::ios::binary | std::ios::trunc);
	std::istringstream loop_ss( argv[2] );
	std::istringstream lock_ss( argv[3] );
	std::istringstream cs_ss( argv[4] );

	if (!(loop_ss >> loop_iterations) || !(lock_ss >> lock_iterations) ||  !(cs_ss >> cs_iterations)){
		printf("Usage: lock [out filename] [outer iterations] [inner iterations] [cs iterations]\n");
		exit(EXIT_FAILURE);
	}

	dataCollector << "\"LockName\", \"Iteration\", \"ThreadNum\", \"NumThreads\", \"Time\"";
	for (int i = 0; i < THREAD_MAX; i++) {
		dataCollector << ", \"Thread " << i << "\"";
	}

	dataCollector << std::endl << std::flush;

	std::cout << loop_iterations << " " << lock_iterations << " " << cs_iterations << std::endl;

	omp_set_dynamic(0);     // Explicitly disable dynamic teams

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads *= THREAD_INCREMENT) {
		PetersonLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads, loop_iterations, lock_iterations, cs_iterations) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		BoulangerieLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads, loop_iterations, lock_iterations, cs_iterations) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		BakeryLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads, loop_iterations, lock_iterations, cs_iterations) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		FilterLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads, loop_iterations, lock_iterations, cs_iterations) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		LamportLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads, loop_iterations, lock_iterations, cs_iterations) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		TestAndSetLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads, loop_iterations, lock_iterations, cs_iterations) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	for (nthreads = THREAD_INIT; nthreads <= THREAD_MAX; nthreads += THREAD_INCREMENT) {
		TestAndTestAndSetLock lock(nthreads);
		if (runMultipleLock(dataCollector, &lock, nthreads, loop_iterations, lock_iterations, cs_iterations) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	dataCollector.close();

	return EXIT_SUCCESS;
}
