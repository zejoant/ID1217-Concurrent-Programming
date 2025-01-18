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

/* a reusable counter barrier */
void Barrier()
{
    pthread_mutex_lock(&barrier);
    numArrived++;
    if (numArrived == numWorkers)
    {
        numArrived = 0;
        pthread_cond_broadcast(&go);
    }
    else
        pthread_cond_wait(&go, &barrier);
    pthread_mutex_unlock(&barrier);
}

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

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
    int i, j;
    long l; /* use long in case of a 64-bit system */
    pthread_attr_t attr;
    pthread_t workerid[MAXWORKERS];

    /* set global thread attributes */
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    /* initialize mutex and condition variable */
    pthread_mutex_init(&barrier, NULL);
    pthread_cond_init(&go, NULL);
    pthread_mutex_init(&lock, NULL);

    /* read command line args if any */
    size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
    numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
    if (size > MAXSIZE)
        size = MAXSIZE;
    if (numWorkers > MAXWORKERS)
        numWorkers = MAXWORKERS;
    stripSize = size / numWorkers;

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
    for (l = 0; l < numWorkers; l++){
        pthread_create(&workerid[l], &attr, Worker, (void *)l);
        pthread_join(workerid[l], NULL);
    }
    end_time = read_timer();
    printf("The min is %d at position %d,%d\n", min[0], min[2], min[1]);
    printf("The max is %d at position %d,%d\n", max[0], max[2], max[1]);
    printf("The sum is %d\n", sum);
    printf("The execution time is %g sec\n", end_time - start_time);
    pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg)
{
    long myid = (long)arg;
    int total, i, j, first, last;
  

#ifdef DEBUG
    printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
#endif

    /* determine first and last rows of my strip */
    first = myid * stripSize;
    last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);

    /* sum values in my strip */
    for (i = first; i <= last; i++)
        for (j = 0; j < size; j++)
        {
            //pthread_mutex_lock(&lock);
            sum += matrix[i][j];
            //pthread_mutex_unlock(&lock);
            if (matrix[i][j] < min[0])
            {
                if (matrix[i][j] < min[0])
                {
                    pthread_mutex_lock(&lock);
                    min[0] = matrix[i][j];
                    min[1] = i;
                    min[2] = j;
                    pthread_mutex_unlock(&lock);
                }
            }
            if (matrix[i][j] > max[0])
            {
                if (matrix[i][j] > max[0])
                {
                    pthread_mutex_lock(&lock);
                    max[0] = matrix[i][j];
                    max[1] = i;
                    max[2] = j;
                    pthread_mutex_unlock(&lock);
                }
            }
        }
}
