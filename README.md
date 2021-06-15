# advanced_multiprocessor_programming
Advanced Multiprocessor Programming
Project 1 - Register Locks

by

Enrico Coluccia, 12005483
Aron Wussler, 11717702

Executing a make command generates the ./lock executable.

To execute the program please run:
```
./lock [out_filename] [outer_iterations] [inner_iterations] [cs_iterations]
```
Where:
-`out_filename`: the name of the output file
-`outer_iterations`: outer iterations to calculate the time
-`inner_iterations`: how many times each inner loop takes the lock
-`cs_iterations`: length of the critical section

Example:
```
./lock nebula.csv 128 1024 16
```

or with `srun`:
```
srun -p q_student --time=5:00 -N 1 -c 64 ./lock nebula.csv 128 1024 16
```

The program runs each lock implementation with variable number of threads, from 2 to 64 with a step of 2.
This configuration is defined by the following rows:
```
#define THREAD_INIT 2
#define THREAD_MAX 64
#define THREAD_INCREMENT 2
```
