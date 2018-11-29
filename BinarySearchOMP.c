//Author: Chittampally Vasanth Raja
//The following program is parallized version of search algorithm
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#define MAXTHREADS 10

FILE * fp; 

void binarySearch(int key, int *a, int size){
    int found = 0;
    int i;
    for(int j=1;j<=5;j++){
        omp_set_num_threads(j);
        found=0;
        #pragma omp parallel private(i)
        {
 
            int start,noofsteps;
            #pragma omp single
            if(found==0)
            {
 
                start=(omp_get_thread_num())*(size/omp_get_num_threads());
                noofsteps=start+(size/omp_get_num_threads());
                if(size%j!=0)
                    noofsteps+=(size%j);
                for(i=start;i<noofsteps;i++)             
                    if(key==a[i]) {
                        found=1;
                        break;
                    }
 
            }
        }
    }
}

void benchMark(int size){
    int *arr = (int *)malloc(size*sizeof(int));
    for(int i = 0; i < size; i++){
        arr[i] = rand() % 1000;
    }
    clock_t begin = clock();
    for(int p = 0; p < 1000; p++){
        binarySearch(p, arr, size);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    fprintf (fp, "%d , %f\n", size, time_spent);
    free(arr);
}
int main(void){
    int data = 10;
    fp = fopen ("BinarySearchOMP.txt","w"); 
    while(data <= 1000000){
    benchMark(data);
    data = data * 10;
   }
    /* close the file*/  
    fclose (fp);
    printf("Done!\n"); 
    return 0;
}

