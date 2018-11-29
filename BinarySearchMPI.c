
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void benchMark(int max){
    int *a = (int *)malloc(max*sizeof(int));
    for(int i = 0; i < max; i++){
        arr[i] = rand()%1000;
    } 
    clock_t begin = clock();
    for(int i = 0; i < 1000; i++){
        int result = binarySearch(arr, 0, max-1, i); 
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    fprintf (fp, "%d , %f\n", max, time_spent);
    free(arr);
}

int binarySearch(int *arr, int l, int r, int x){
     
    MPI_Scatter(&arr,5,MPI_INT,&b,5,MPI_INT,0,MPI_COMM_WORLD);
if(rank==0)
{
for(i=0;i<l;i++)
    {
        if(b[i]==search)
        {
            printf("\nNumber found!\t\t%d\t\t%d",rank,i);
            flag=1;
        }
        printf("\n%d\t\t%d",b[i],rank);
    }
}
if(rank==1)
{
    for(i=0;i<5;i++)
    {
        if(b[i]==search)
        {
            printf("\nNumber found!\t\t%d\t\t%d",rank,i);
            flag=1;
        }
        printf("\n%d\t\t%d",b[i],rank);
    }
} 
}

int main(){
    
clock_t tic = clock();
int *a = (int *)malloc(max*sizeof(int));
    for(int i = 0; i < max; i++){
        arr[i] = rand()%1000;
    }
int rank,size,search=6,flag=0;      
long int i;
MPI_Init(NULL,NULL);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);

MPI_Finalize();
clock_t toc=clock();
printf("\n\nElapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
}