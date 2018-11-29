#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "mpi.h"

FILE *fp;

void printArray(int *arr, int size);
int linearSearch(int x, int *arr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == x)
			return i;
	}

	return -1;
}

int linearSearchOMP(int x, int *arr, int size) {
	int i, itr, minIndex, threads = omp_get_num_threads();
	int *found = (int *)malloc(threads * sizeof(int));

	#pragma omp for private(i) 
	for (i = 0; i < threads; i++) {
		found[i] = -1;
	}

	#pragma omp for private(i)
	for (i = 0; i < size; i++) {
		if (arr[i] == x) {
			if(found[omp_get_thread_num()] == -1)
				found[omp_get_thread_num()] = i;
		}
	}
	
	minIndex = size;
	for (i = 0; i < threads; i++) {
		if (found[i] != -1) {
			if (found[i] < minIndex) minIndex = found[i];
		}
	}
	free(found);
	if(minIndex == size)
		return -1;
	else return minIndex;
}

int linearSearchMPI(int argc, char *argv[], int x, int *arr, int size) {
	int rank, np, localsize, i, minIndex;
	int *localdata, *found, *results;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	localsize = size / np;
	found = (int *)malloc(1 * sizeof(int));
	results = (int *)malloc(np * sizeof(int));

	found[0] = -1;
	for (i = rank * localsize; i < (rank + 1)*localsize; i++) {
		if (arr[i] == x) {
			found[0] = i;
			break;
		}
	}

	MPI_Gather(found, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);
	/*
	localsize = size / np;
	localdata = (int *)malloc(localsize * sizeof(int));
	found = (int *)malloc(1 * sizeof(int));
	results = (int *)malloc(np * sizeof(int));

	MPI_Scatter(arr, localsize, MPI_INT, localdata, localsize, MPI_INT, 0, MPI_COMM_WORLD);

	found[0] = -1;
	for (i = 0; i < localsize; i++) {
		if (localdata[i] == x) {
			found[0] = i;
			break;
		}
	}

	MPI_Gather(found, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if(rank == 0)
		printArray(results, np);*/
	if (rank == 0) {
		minIndex = size;
		for (i = 0; i < np; i++) {
			if (results[i] != -1) {
				if (results[i] < minIndex) minIndex = results[i];
			}
		}
		if (minIndex == size)minIndex = -1;
	}
	else minIndex = -2;
	free(found);
	free(results);
	MPI_Finalize();
	return minIndex;
}

void randomize(int *arr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		arr[i] = rand();
	}
}

int pickRandElem(int *arr, int size) {
	int elem = size/2 + (rand()%(size/2));
	elem = arr[elem];
	return elem;
}

void printArray(int *arr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int is_correct(int elem, int index, int *arr) {
	if (arr[index] == elem)return 1;
	else printf("Returned this element: %d\n", arr[index]);
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		//printf("Must specify size of array.\n");
		return 0;
	}

	fp = fopen("LinearSearch.txt", "a");
	int size = atoi(argv[2]), oper = atoi(argv[1]), randElem, index;
	int *arr = (int *)malloc(size * sizeof(int));
	double start, end;
	
	randomize(arr, size);
	randElem = pickRandElem(arr, size);

	if (oper == 2) {
		start = omp_get_wtime();
		index = linearSearchMPI(argc, argv, randElem, arr, size);
		if (index == -2) {
			free(arr);
			return 0;
		}
		end = omp_get_wtime();
		fprintf(fp, "MPI Linear Search\n");
	} 
	else if (oper == 1) {
		start = omp_get_wtime();
		index = linearSearchOMP(randElem, arr, size);
		end = omp_get_wtime();
		fprintf(fp, "OpenMP Linear Search\n");
	}
	else if (oper == 0) {
		start = omp_get_wtime();
		index = linearSearch(randElem, arr, size);
		end = omp_get_wtime();
		fprintf(fp, "Sequential Linear Search\n");
	}

	fprintf(fp, "Element: %d -> Index: %d\n", randElem, index);
	fprintf(fp, "Time elapsed: %f\n", (end - start));
	if (is_correct(randElem, index, arr))fprintf(fp, "Correct\n");
	else fprintf(fp, "Incorrect\n");

	free(arr);
	return 0;
}