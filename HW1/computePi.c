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
  };

  double e;
  pthread_attr_t attr;
  pthread_t workerId;

  /* set global thread attributes */
  
/* read command line, initialize, and create threads */

void *quad(void *arguments);

int main(int argc, char *argv[]) {

  struct arg_struct main_args = {
  .left = 0,
  .right = 1,
  .fl = 1,
  .fr = 0,
  .area = 1,
  .result = 0,
  };
  

  /* initialize mutex and condition variable */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* read command line args if any */ 
    e = (atof(argv[1])<0.0001)? 0.0001 : atof(argv[1]);
    int np = (argc<1)? 1 : argc;
    printf("e is %f \n", e);
 

  /* do the parallel work: create the workers */
  start_time = read_timer();
  double *result;
  //pthread_create(&workerId, NULL, quad, &args);
  //pthread_join(workerId, NULL);
  quad(&main_args);
  end_time = read_timer();
  printf("%f", 4*main_args.result);
  pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *quad(void *arguments) {

  struct arg_struct *args = arguments;
  double m = (args->left + args->right)/2;
  double fm = functionCalculator(m);
  double leftArea = ( args->fl + fm ) * (m - args->left)/2;
  double rightArea = ( fm + args->fr ) * (args->right - m)/2;
  //printf("debugger: rightArea: %f \n ", rightArea);
  if(fabs((leftArea+rightArea)-args->area)> e){
    struct arg_struct leftArgs = {.left=args->left, .right=m, .fl=args->fl, .fr=fm, .area=leftArea };
    struct arg_struct rightArgs = {.left=m, .right=args->right, .fl=fm, .fr=args->fr, .area=rightArea };
    pthread_t Id1, Id2;

    pthread_create(&Id1, NULL, quad, &leftArgs);

    pthread_create(&Id2, NULL, quad, &rightArgs);

    pthread_join(Id1, NULL);
    pthread_join(Id2, NULL);

    //printf("left result : %f right result: %f \n", leftArgs.result, rightArgs.result);

    leftArea = leftArgs.result;
    rightArea = rightArgs.result;
  }
  args->result = leftArea + rightArea;
}


