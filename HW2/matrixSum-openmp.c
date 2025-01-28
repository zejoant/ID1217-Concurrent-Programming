/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>

double start_time, end_time;

#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10000 /* maximum matrix size */
#define MAXWORKERS 8  /* maximum number of workers */

struct val_and_pos{
   int val, x, y;
};

double times[5];

int numWorkers;
int size;
int matrix[MAXSIZE][MAXSIZE];

int comparator(const void *a, const void *b){
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
}

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
    int i, j, total = 0;
    struct val_and_pos min = {.val = INT_MAX, .x = 0, .y = 0};
    struct val_and_pos max = {.val = 0, .x = 0, .y = 0};

    /* read command line args if any */
    size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
    numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
    if (size > MAXSIZE)
        size = MAXSIZE;
    if (numWorkers > MAXWORKERS)
        numWorkers = MAXWORKERS;

    omp_set_num_threads(numWorkers);

    /* initialize the matrix */
    for (i = 0; i < size; i++)
    {
        //  printf("[ ");
        for (j = 0; j < size; j++)
        {
            matrix[i][j] = rand() % 99;
            //	  printf(" %d", matrix[i][j]);
        }
        //	  printf(" ]\n");
    }

    for (int k = 0; k <= 4; k++){
        total = 0;
        start_time = omp_get_wtime();
        #pragma omp parallel for reduction(+ : total) private(j)
            for (i = 0; i < size; i++){
                for (j = 0; j < size; j++){
                    total += matrix[i][j];
                    if (matrix[i][j] > max.val)
                    {
                        max.val = matrix[i][j];
                        max.y = i;
                        max.x = j;
                    }
                    if (matrix[i][j] < min.val)
                    {
                        min.val = matrix[i][j];
                        min.y = i;
                        min.x = j;
                    }
                }
            } // implicit barrier
        end_time = omp_get_wtime();
        times[k] = end_time - start_time;
    }

    double size = sizeof(times) / sizeof(times[0]); 
    qsort(times, size, sizeof(double), comparator); 
    double median = (times[2]+times[3] )/ 2;

    /*printf("the total is %d\n", total);
    printf("the max is %d at position [%d,%d] \nthe min is %d at position [%d,%d]\n", max.val, max.x, max.y, min.val, min.x, min.y);
    printf("it took %g seconds\n", end_time - start_time);
    */
    printf("the median is %g\n", median);
}
