#!/bin/bash

echo "Doing MergeSort Sequential"
gcc -o MergeSort MergeSort.c
./MergeSort
echo "Done With MergeSort Sequential"
echo "Doing MergeSort OpenMP with 48 Threads"
gcc -fopenmp -o MergeSortOMP MergeSortOMP.c
export OMP_NUM_THREADS=48
./MergeSortOMP
echo "Done with MergeSort OpenMP with 48 Threads"
echo "Doing MergeSort MPI with 32 Processes"
mpicc -o MergeSortMPI MergeSortMPI.c
mpirun -np 32 MergeSortMPI
echo "Finished Benchmark. Thanks for the wait :)"