#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

#define MAXARRAYSIZE 1000000
#define MAXWORKERS 10

int size;
int numWorkers;
double start_time, end_time;
int array[MAXARRAYSIZE];

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(int arr[], int first, int last){

    //partition part
    int pivot = arr[last];
    int i, toBeSwapped=first-1;

    if(!(first > last)){
        for(i = first; i<last; i++){
            if(arr[i] <= pivot){
                toBeSwapped++;
                swap(&arr[i], &arr[toBeSwapped]);
            }
        }

        toBeSwapped++;
        swap(&arr[last], &arr[toBeSwapped]);

        //task-giver part
        //if more than 1 threads it runs parallel
        if(omp_get_num_threads() > 1){
            #pragma omp task firstprivate(arr, first, toBeSwapped)
                quickSort(arr, first, toBeSwapped-1);

                quickSort(arr, toBeSwapped+1, last);

        //sequential for when only 1 thread
        }else{ 
            quickSort(arr, first, toBeSwapped-1);
            quickSort(arr, toBeSwapped+1, last);
        }
    }
}

int main(int argc, char *argv[]){

    size = (argc > 1) ? atoi(argv[1]) : MAXARRAYSIZE;
    numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
    //int arraySize = sizeof(array)/sizeof(array[0]);
    
    if (size > MAXARRAYSIZE){
        size = MAXARRAYSIZE;
    }
    
    if (numWorkers > MAXWORKERS){
        numWorkers = MAXWORKERS;
    }

    //int array[size];

    omp_set_num_threads(numWorkers);

    srand(time(NULL));

    for(int i = 0; i<size; i++){
        array[i] = rand() % 101;
    }

    //print the unsorted matrix
    #ifdef DEBUG
    printf("Unsorted list: [");
    for (int i = 0; i < size; i++) {
        printf("%d, ", array[i]);
    }
    printf("]\n\n");
    #endif

    start_time = omp_get_wtime();
    #pragma omp parallel 
    {
        #pragma omp single nowait
        quickSort(array, 0, size-1);
    }
    end_time = omp_get_wtime();

    //print the sorted matrix
    #ifdef DEBUG
    printf("Sorted list: [");
    for (int i = 0; i < size; i++) {
        printf("%d, ", array[i]);
    }
    printf("]\n\n");
    #endif

    printf("It took %g seconds to sort the array", end_time-start_time);

    return 0;
}
