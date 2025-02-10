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

int numWorkers;
int size;
int matrix[MAXSIZE][MAXSIZE];

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
        for (j = 0; j < size; j++)
        {
            matrix[i][j] = rand() % 99;
        }
    }

    //print the matrix
    #ifdef DEBUG
    for (i = 0; i < size; i++) {
        printf("[ ");
        for (j = 0; j < size; j++) {
        printf(" %d", matrix[i][j]);
        }
        printf(" ]\n");
    }
    #endif

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

    printf("The total is %d\n", total);
    printf("The max is %d at position [%d,%d] \nThe min is %d at position [%d,%d]\n", max.val, max.x, max.y, min.val, min.x, min.y);
    printf("It took %g seconds\n", end_time - start_time);
    
    //printf("the median is %g\n", median);
}
