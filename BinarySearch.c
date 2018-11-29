#include <stdlib.h> 
#include <stdio.h>
#include <time.h>

FILE * fp; 

// A recursive binary search function. It returns 
// location of x in given array arr[l..r] is present, 
// otherwise -1 
int binarySearch(int *arr, int l, int r, int x){
     
    if (r >= l) 
    { 
        int mid = l + (r - l)/2; 
        
        // If the element is present at the middle 
        // itself 
        if (arr[mid] == x) 
            return mid; 
        
        // If element is smaller than mid, then 
        // it can only be present in left subarray 
        if (arr[mid] > x) 
            return binarySearch(arr, l, mid-1, x); 
        
        // Else the element can only be present 
        // in right subarray 
        return binarySearch(arr, mid+1, r, x); 
    } 
    
    // We reach here when element is not 
    // present in array 
    return -1; 
} 

void benchMark(int max){
    int *arr = (int *)malloc(max*sizeof(int));
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

int main(void) 
{ 
    int data = 10000;
    fp = fopen ("BinarySearch.txt","w"); 
    while(data <= 100000000){
    benchMark(data);
    data = data * 10;
   }
    /* close the file*/  
    fclose (fp);
    return 0; 
} 
