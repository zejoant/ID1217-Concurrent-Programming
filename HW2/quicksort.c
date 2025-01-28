#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

#define MAXARRAYSIZE 100000000
#define MAXWORKERS 10

int size;
int numWorkers;


void quickSort(int arr[], int first, int last){

    //partition part
    int pivot = arr[first+(last-first)/2];
    int i, toBeSwapped=first-1;

    for(i = first; i<=last; i++){
        if(arr[i] < pivot){
            toBeSwapped++;
            swap(&arr[i], &arr[toBeSwapped]);
        }
    }

    toBeSwapped++;
    swap(&arr[i], &arr[toBeSwapped]);

    //task-giver part

    #pragma omp task firstprivate(arr, first, toBeSwapped) {
        quickSort(arr, first, toBeSwapped);
    }

    #pragma omp task firstprivate(arr, toBeSwapped, last) {
        quickSort(arr, toBeSwapped, last);
    }
    
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[]){

    size = (argc > 1) ? atoi(argv[1]) : MAXARRAYSIZE;
    numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;

    if (size > MAXARRAYSIZE)
        size = MAXARRAYSIZE;

    if (numWorkers > MAXWORKERS)
        numWorkers = MAXWORKERS;

    result[size];
    int array[size];

    omp_set_num_threads(numWorkers);

    srand(time(NULL));

    for(int i = 0; i<size; i++){
        array[i] = rand() % 101;
    }

    #pragma omp parallel {
        #pragma omp single nowait
        quickSort(array, 0, size-1);
    }

    return 0;
}
