# Benchmarking Parallel Algorithms
In this project, we visited various algorithms that we are all familiar with and we tried to formulate parallel implementations for them using various tools to see which is better suited for each one. Here we will specifically be testing with OpenMP and MPI. These are two very different tools to apply parallelism techniques. OpenMP works with threads, while MPI handles processes. They both have their advantages and disadvantages. OpenMP is a good and efficient way to perform parallel tasks that don’t require much communication, while MPI is more resource intensive but provides an interface for communication between processes so that synchronisation is as cost effective as it can be. This makes it so that depending on what you are doing it may be better to go with one or the other. Sometimes it might not be better to even parallelize an algorithm at all because the overhead introduced by required communication or by the cost of creating threads might make the execution time worse than to simply apply it sequentially. The algorithms that were tested are the following:
QuickSort
Linear Search
MergeSort
Binary Search
All of these will be implemented using OpenMP and MPI and their execution times will be compared with each other and with the sequential algorithm.

