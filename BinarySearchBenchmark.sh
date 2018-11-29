#!/bin/bash

echo "Doing Binary Search Sequential"
gcc -o BinarySearch BinarySearch.c
./BinarySearch
echo "Done With Binary Search Sequential"
echo "Doing Binary Search OpenMP with 48 Threads"
gcc -fopenmp -o BinarySearchOMP BinarySearchOMP.c
export OMP_NUM_THREADS=48
./BinarySearchOMP
echo "Done with Binary Search OpenMP with 48 Threads"
echo "Doing MergeSort MPI with 32 Processes"
echo "Finished Benchmark. Thanks for the wait :)"