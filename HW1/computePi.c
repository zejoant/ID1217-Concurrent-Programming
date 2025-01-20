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
#include <math.h>


double functionCalculator(double m){
  return sqrt(1-(m*m));
}

/* timer */
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time; /* start and end times */

struct arg_struct{
  double left;
  double right;
  double fl;
  double fr;
  double area;
  double result;
  int workers;
};

pthread_attr_t attr;
pthread_t workerId;
double e;
int np;

void *quad(void *arguments);

int main(int argc, char *argv[]) {

  np = (argc < 2) ? 10 : atoi(argv[1]);
  e = (argc > 2) ? atof(argv[2]) : 0.0001;
  
  struct arg_struct main_args = {
    .left = 0,
    .right = 1,
    .fl = 1,
    .fr = 0,
    .area = 1,
    .result = 0,
    .workers = 0,
  };
  
  /* read command line args if any */ 
 
  printf("The number of threads is: %d, and epsilon is: %.30f \n", np, e);


  /* do the parallel work: create the workers */
  start_time = read_timer();
  quad(&main_args);
  end_time = read_timer();

  printf("the PI approximation is: %f\n", 4*main_args.result);
  printf("The execution time is %g sec\n", end_time - start_time);

  pthread_exit(NULL);
}

void *quad(void *arguments) {
  struct arg_struct *args = arguments;
  double m = (args->left + args->right)/2;
  double fm = functionCalculator(m);
  double leftArea = ( args->fl + fm ) * (m - args->left)/2;
  double rightArea = ( fm + args->fr ) * (args->right - m)/2;

  if(fabs((leftArea+rightArea)-args->area)> e){
    struct arg_struct leftArgs = {.left=args->left, .right=m, .fl=args->fl, .fr=fm, .area=leftArea, .workers=args->workers+1};
    struct arg_struct rightArgs = {.left=m, .right=args->right, .fl=fm, .fr=args->fr, .area=rightArea, .workers=args->workers+1};
    
    if (args->workers < np){ //limits number of threads/workers
      pthread_t Id1, Id2;

      pthread_create(&Id1, NULL, quad, &leftArgs);
      pthread_create(&Id2, NULL, quad, &rightArgs);

      pthread_join(Id1, NULL);
      pthread_join(Id2, NULL);
    }
    else{ //if thread limit is reached
      quad(&leftArgs);
      quad(&rightArgs);
    }

    leftArea = leftArgs.result;
    rightArea = rightArgs.result;
  }
  args->result = leftArea + rightArea;
}


