#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "omp.h"

#define ROOT_RESULT 666 
#define GROUP_EXCHANGE 667

FILE *fp;

void printArray(int *arr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void randomize(int *arr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		arr[i] = rand();
	}
}

int is_sorted(int *a, int length)
{
	int i;
	for (i = 1; i < length; i++)
		if (a[i - 1] > a[i])
			return 0;

	return 1;
}

int partition(int A[], int length)
{
	return (A[0] + A[length - 1]) / 2;
}

int divide(int A[], int length, int pivot)
{
	int divider = length / 2;
	while (divider < length && A[divider] <= pivot)
		divider++;
	while (divider > 0 && A[divider - 1] > pivot)
		divider--;

	return divider;
}

void merge(int m, int n, int A[], int B[], int C[])
{
	int i, j, k, p;
	i = j = k = 0;
	while (i < m && j < n) {
		if (A[i] <= B[j]) {
			C[k] = A[i];
			i++;
		}
		else {
			C[k] = B[j];
			j++;
		}
		k++;
	}
	if (i < m) {
		for (p = i; p < m; p++) {
			C[k] = A[p];
			k++;
		}
	}
	else {
		for (p = j; p < n; p++) {
			C[k] = B[p];
			k++;
		}
	}
}

int mpi_qsort(int *IN[], int length, MPI_Comm procs_comm)
{
	int number_of_processors, myrank, color, pivot, divider;
	int dest, offset, chunksize, recv_length, newlength;
	int *A, *B, *C;

	MPI_Comm group_comm;
	MPI_Status status;
	MPI_Request request;
	MPI_Comm_size(procs_comm, &number_of_processors);
	MPI_Comm_rank(procs_comm, &myrank);

	if (number_of_processors < 2)
		return length;

	A = *IN;

	pivot = partition(A, length);
	MPI_Bcast(&pivot, 1, MPI_INT, 0, procs_comm);

	divider = divide(A, length, pivot);

	color = myrank / (number_of_processors / 2);

	if (color) {
		dest = myrank - number_of_processors / 2;
		offset = 0;
		chunksize = divider;
	}
	else {
		dest = myrank + number_of_processors / 2;
		offset = divider;
		chunksize = length - divider;
	}

	MPI_Isend(&A[offset], chunksize, MPI_INT, dest, GROUP_EXCHANGE, procs_comm, &request);

	MPI_Probe(dest, GROUP_EXCHANGE, procs_comm, &status);
	MPI_Get_count(&status, MPI_INT, &recv_length);
	B = malloc(recv_length * sizeof(*B));
	MPI_Recv(B, recv_length, MPI_INT, dest, GROUP_EXCHANGE, procs_comm, &status);

	newlength = recv_length + length - chunksize;
	offset = offset ? 0 : divider;
	C = malloc(newlength * sizeof(*C));
	merge(length - chunksize, recv_length, &A[offset], B, C);
	
	MPI_Wait(&request, &status);

	free(A);
	free(B);
	*IN = C;

	MPI_Comm_split(procs_comm, color, myrank, &group_comm);
	return mpi_qsort(IN, newlength, group_comm);
}

int compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}

void quickSortMPI(int argc, char *argv[]) {
	int n = argc > 2 ? atoi(argv[2]) : 100;

	double time;

	int number_of_processors;

	int myrank;

	MPI_Status status;
	MPI_Comm procs;
	MPI_Request request;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_processors);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	int local_size = n / number_of_processors;

	int *local_array, *root_array = malloc(n * sizeof(*local_array));
	local_array = malloc(local_size * sizeof(*local_array));

	if (myrank == 0) {
		fprintf(fp, "MPI Quicksort: %d processes\nElements: %d\n", number_of_processors, n);
		randomize(root_array, n);
		time = MPI_Wtime();
	}

	MPI_Scatter(root_array, local_size, MPI_INT, local_array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

	qsort(local_array, local_size, sizeof(*local_array), compare);

	local_size = mpi_qsort(&local_array, local_size, MPI_COMM_WORLD);

	MPI_Isend(local_array, local_size, MPI_INT, 0, ROOT_RESULT, MPI_COMM_WORLD, &request);

	if (myrank == 0) {
		int recv_length, offset, p;
		offset = 0;
		for (p = 0; p < number_of_processors; p++) {
			MPI_Recv(&root_array[offset], n, MPI_INT, p, ROOT_RESULT, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &recv_length);
			offset += recv_length;
		}

		time = MPI_Wtime() - time;

		fprintf(fp, "Time elapsed: %f\n", time);
		if (is_sorted(root_array, n) == 1) {
			fprintf(fp, "Correct\n");
		}
		else {
			fprintf(fp, "Incorrect\n");
		}
	}

	MPI_Wait(&request, &status);

	MPI_Finalize();
}

void swap(int l, int r, int *arr) {
	int temp = arr[l];
	arr[l] = arr[r];
	arr[r] = temp;
}

int partitionOMP(int left, int right, int *arr) {
	int pivot = left;
	int l = left;
	int r = right;

	while (l < r) {
		while (arr[l] <= arr[pivot] && l <= r) l++;
		while (arr[r] > arr[pivot] && l <= r) r--;

		if (l < r) {
			swap(l, r, arr);
		}
	}

	int temp = arr[pivot];
	arr[pivot] = arr[r];
	arr[r] = temp;
	return r;
}

void quickSort(int left, int right, int *arr) {
	if (left < right) {
		int pivot = partitionOMP(left, right, arr);
		quickSort(left, pivot - 1, arr);
		quickSort(pivot + 1, right, arr);
	}
}

void quickSortOMP2(int left, int right, int *arr, int threshhold) {
	if (left < right) {
		int pivot = partitionOMP(left, right, arr);

		if ((right - left) < threshhold) {
			quickSort(left, pivot - 1, arr);
			quickSort(pivot + 1, right, arr);
		}
		else {
				#pragma omp task default(none) firstprivate(arr, left, pivot, threshhold)
				quickSortOMP2(left, pivot - 1, arr, threshhold);

				#pragma omp task default(none) firstprivate(arr, right, pivot, threshhold)
				quickSortOMP2(pivot + 1, right, arr, threshhold);
				
		}
	}
}

void quickSortOMP(int *arr, int size) {
	#pragma omp parallel default(none) shared(arr, size)
	{
		#pragma omp single nowait
		quickSortOMP2(0, size - 1, arr, 10);
	}
}

int main(int argc, char *argv[])
{
	if (argc <= 2)return -1;
	int oper = atoi(argv[1]);

	fp = fopen("QuickSort.txt", "a");
	if(oper == 2)
		quickSortMPI(argc, argv);
	else if (oper == 1) {
		int size = atoi(argv[2]);
		int *arr = (int *)malloc(size * sizeof(int));
		double start, end;
		randomize(arr, size);
		start = omp_get_wtime();
		quickSortOMP(arr, size);
		end = omp_get_wtime();
		fprintf(fp, "OpenMP Quicksort: %d threads\nElements: %d\n", omp_get_max_threads(), size);
		fprintf(fp, "Time elapsed: %f\n", end - start);
		if (is_sorted(arr, size) == 1) {
			fprintf(fp, "Correct\n");
		}
		else {
			fprintf(fp, "Incorrect\n");
		}
		free(arr);
	}
	else if (oper == 0) {
		int size = atoi(argv[2]);
		int *arr = (int *)malloc(size * sizeof(int));
		double start, end;
		randomize(arr, size);
		start = omp_get_wtime();
		quickSort(0, size - 1, arr);
		end = omp_get_wtime();
		fprintf(fp, "Sequential Quicksort\nElements: %d\n", size);
		fprintf(fp, "Time elapsed: %f\n", end - start);
		if (is_sorted(arr, size) == 1) {
			fprintf(fp, "Correct\n");
		}
		else {
			fprintf(fp, "Incorrect\n");
		}
		free(arr);
	}
	return 0;
}