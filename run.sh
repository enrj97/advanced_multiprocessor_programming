#! /bin/bash

srun -p q_student --time=5:00 -N 1 -c 16 ./lock nebula.csv
