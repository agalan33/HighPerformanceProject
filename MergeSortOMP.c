#include <omp.h>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
FILE * fp;
// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int *arr, int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 = r - m; 

	int *L = (int *)malloc(n1*sizeof(int));
	int *R = (int *)malloc(n2*sizeof(int)); 

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1+ j]; 

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2) 
	{ 
		if (L[i] <= R[j]) 
		{ 
			arr[k] = L[i]; 
			i++; 
		} 
		else
		{ 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	/* Copy the remaining elements of L[], if there 
	are any */
	while (i < n1) 
	{ 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 

	/* Copy the remaining elements of R[], if there 
	are any */
	while (j < n2) 
	{ 
		arr[k] = R[j]; 
		j++; 
		k++; 
	}

	free(R);
	free(L); 
} 

/* l is for left index and r is right index of the 
sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r) 
{ 
	if (l < r) 
	{ 
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		int m = l+(r-l)/2; 

		// // Sort first and second halves 
		#pragma omp parallel sections num_threads(48)
		{


		#pragma omp section
		{
				mergeSort(arr,l,m); //call 1
		}
		#pragma omp section
		{
				mergeSort(arr,m+1,r); //call 2
		} 
	}

		merge(arr, l, m, r);
	} 
} 

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size) 
{ 
	int i; 
	for (i=0; i < size; i++) 
		printf("%d ", A[i]); 
	printf("\n"); 
} 

void benchMark(int max){
	int *arr = (int *)malloc(max*sizeof(int));
	for(int i = 0; i < max; i++){
		arr[i] = rand()%1000;
	} 
	// int arr_size = sizeof(arr)/sizeof(arr[0]); 

	// printf("Given array is \n"); 
	// printArray(arr, max); 
	clock_t begin = clock();
	mergeSort(arr, 0, max - 1); 
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	// printf("\nSorted array is \n"); 
	// printArray(arr, max);
	fprintf (fp, "%d , %f\n", max, time_spent);
	// printf("Done with %d\n", max);
	free(arr);
} 

/* Driver program to test above functions */
int main(){
	int data = 1000;
   /* open the file for writing*/
   fp = fopen ("MergeSortOMP48Threads.txt","w"); 
   while(data <= 100000000){
   	benchMark(data);
   	data = data * 10;
   }
	/* close the file*/  
    fclose (fp);
    // printf("Done!\n");
	return 0; 
}