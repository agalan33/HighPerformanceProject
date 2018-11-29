#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int *, int *, int, int, int);
void mergeSort(int *, int *, int, int);
FILE * fp;

/********** Merge Function **********/
void merge(int *a, int *b, int l, int m, int r) {
	
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;
	
	while((h <= m) && (j <= r)) {
		
		if(a[h] <= a[j]) {
			
			b[i] = a[h];
			h++;
			
			}
			
		else {
			
			b[i] = a[j];
			j++;
			
			}
			
		i++;
		
		}
		
	if(m < h) {
		
		for(k = j; k <= r; k++) {
			
			b[i] = a[k];
			i++;
			
			}
			
		}
		
	else {
		
		for(k = h; k <= m; k++) {
			
			b[i] = a[k];
			i++;
			
			}
			
		}
		
	for(k = l; k <= r; k++) {
		
		a[k] = b[k];
		
		}
		
	}

/********** Recursive Merge Function **********/
void mergeSort(int *a, int *b, int l, int r) {
	
	int m;
	
	if(l < r) {
		
		m = (l + r)/2;
		
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
		
		}
		
	}

void benchMark(int max){
	/********** Create and populate the array **********/
	int n = max;
	int *original_array = malloc(n * sizeof(int));
	
	int c;
	srand(time(NULL));
	for(c = 0; c < n; c++) {
		original_array[c] = rand() % n;
		}
	
	clock_t begin = clock();
	/********** Initialize MPI **********/
	int world_rank;
	int world_size;
	
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		
	/********** Divide the array in equal-sized chunks **********/
	int size = n/world_size;
	
	/********** Send each subarray to each process **********/
	int *sub_array = malloc(size * sizeof(int));
	MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	/********** Perform the mergesort on each process **********/
	int *tmp_array = malloc(size * sizeof(int));
	mergeSort(sub_array, tmp_array, 0, (size - 1));
	
	/********** Gather the sorted subarrays into one **********/
	int *sorted = NULL;
	if(world_rank == 0) {
		
		sorted = malloc(n * sizeof(int));
		
		}
	
	MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	/********** Make the final mergeSort call **********/
	if(world_rank == 0) {
		
		int *other_array = malloc(n * sizeof(int));
		mergeSort(sorted, other_array, 0, (n - 1));
		
			
		/********** Clean up root **********/
		free(sorted);
		free(other_array);
			
		}
	
	/********** Clean up rest **********/
	free(original_array);
	free(sub_array);
	free(tmp_array);
	
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	fprintf (fp, "%d , %f\n", max, time_spent*0.10);
	// printf("Done with %d\n", max);

}

int main() {
	int data = 10000;
   /* open the file for writing*/
   fp = fopen ("MergeSortMPI.txt","w");
   MPI_Init(NULL,NULL); 
   while(data <= 100000000){
   	benchMark(data);
   	data = data * 10;
   }
   /********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	/* close the file*/  
    fclose (fp);
    // printf("Done!\n");
	return 0;
	
	
}

