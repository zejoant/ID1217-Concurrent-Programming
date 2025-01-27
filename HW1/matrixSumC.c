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

pthread_mutex_t lock;
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

struct val_and_pos{int val, x, y};

double start_time, end_time; /* start and end times */
int size, stripSize;         /* assume size is multiple of numWorkers */     
struct val_and_pos min = {.val = INT_MAX, .x = 0, .y = 0};
struct val_and_pos max = {.val = 0, .x = 0, .y = 0};
int sum = 0;
int matrix[MAXSIZE][MAXSIZE]; /* matrix */
int nextRow = 0;

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
    int i, j;
    long l; /* use long in case of a 64-bit system */
    pthread_attr_t attr;
    pthread_t workerid[MAXWORKERS];

    /*initialize random. Uncomment if desired*/
    //srand(time(NULL));

    /* set global thread attributes */
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    /* initialize mutex and condition variable */
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
    }
    for(l= 0; l< numWorkers; l++)
        pthread_join(workerid[l], NULL);

    end_time = read_timer();

    printf("The total is %d\n", sum);
    printf("The min is %d at position [%d, %d]\n", min.val, min.x, min.y);
    printf("The max is %d at position [%d, %d]\n", max.val, max.x, max.y);
    printf("The execution time is %g sec\n", end_time - start_time);

    pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg)
{
    long myid = (long)arg;
    int row, i, workerSum;
  

    #ifdef DEBUG
        printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
    #endif

    /* sum values in my strip */
    while(true){
        workerSum = 0;

        pthread_mutex_lock(&lock);
        row = nextRow;
        nextRow++;
        pthread_mutex_unlock(&lock);

        if(row >= size)
            break;
            
        //printf("worker %d did row: %d\n", myid, row);
        
        for(i = 0; i<size; i++){
            workerSum += matrix[row][i];
            if (matrix[row][i] < min.val){
                pthread_mutex_lock(&lock);
                if (matrix[row][i] < min.val){
                    min.val = matrix[row][i];
                    min.y = row;
                    min.x = i;
                }
                pthread_mutex_unlock(&lock);
            }
            if (matrix[row][i] > max.val){
                pthread_mutex_lock(&lock);
                if (matrix[row][i] > max.val){
                    max.val = matrix[row][i];
                    max.y = row;
                    max.x = i;
                }
                pthread_mutex_unlock(&lock);
            }
        }

        pthread_mutex_lock(&lock);
        sum += workerSum;
        pthread_mutex_unlock(&lock);
    }
}
