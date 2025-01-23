/* matrix summation using pthreads

   features: uses a barrier; the Worker[0] computes
             the total sum from partial sums computed by Workers
             and prints the total sum to the standard output

   usage under Linux:
     gcc matrixSum.c -lpthread
     a.out size numWorkers

*/
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000 /* maximum matrix size */
#define MAXWORKERS 10 /* maximum number of workers */

pthread_mutex_t barrier; /* mutex lock for the barrier */
pthread_mutex_t lock;
pthread_cond_t go;  /* condition variable for leaving */
int numWorkers;     /* number of workers */
int numArrived = 0; /* number who have arrived */


/* timer */
double read_timer()
{
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if (!initialized)
    {
        gettimeofday(&start, NULL);
        initialized = true;
    }
    gettimeofday(&end, NULL);
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time; /* start and end times */
int size, stripSize;         /* assume size is multiple of numWorkers */     
int min[] = {INT_MAX, 0, 0};
int max[] = {0, 0, 0};
int sum = 0;
int matrix[MAXSIZE][MAXSIZE]; /* matrix */
int nextRow = 0;

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
    int i, j;
    size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
    int sum = 0;
    int max[] = {0, 0, 0};
    int min[] = {INT_MAX, 0, 0};


    /* initialize the matrix */
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            matrix[i][j] = rand() % 99;
        }
    }

    /* print the matrix */
#ifdef DEBUG
    for (i = 0; i < size; i++)
    {
        printf("[ ");
        for (j = 0; j < size; j++)
        {
            printf(" %d", matrix[i][j]);
        }
        printf(" ]\n");
    }
#endif

    /* do the parallel work: create the workers */
    start_time = read_timer();
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            sum += matrix[i][j];
            if (matrix[i][j] > max[0]){
                max[0] = matrix[i][j];
                max[1] = i;
                max[2] = j;
            }
            if (matrix[i][j] < min[0]){
                min[0] = matrix[i][j];
                min[1] = i;
                min[2] = j;
            }
        }
    }
    end_time = read_timer();
    printf("The min is %d at position [%d, %d]\n", min[0], min[2], min[1]);
    printf("The max is %d at position [%d, %d]\n", max[0], max[2], max[1]);
    printf("The sum is %d\n", sum);
    printf("The execution time is %g sec\n", end_time - start_time);
    pthread_exit(NULL);
}

