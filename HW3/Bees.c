#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>


#define DEFAULTBEES 5

int honey, honeyCounter, eepy, maxHoney;
sem_t notFull;
sem_t full;

void *Bee(void *arg){

    int beeId = (int)arg;

    while(true){
        sem_wait(&notFull);
        if(honey < maxHoney){
            honey++;
            printf("bee %d adds honey, honey counter: %d/%d\n", beeId, honey, maxHoney);
            if(honey == maxHoney){
                printf("BEE %d AWAKENS BEAR\n", beeId);
                sem_post(&full);
                sleep(2);
            }else{
                printf("bee %d searches for honey\n", beeId);
                sem_post(&notFull);
                sleep(rand()%3+eepy);
            }
        }
    }
 }

 void Bear(){

    while(true){
        sem_wait(&full);
        printf("Bear eats all the honey\n");
        honey = 0;
        sleep(2);
        printf("bear ate all the honey and goes to sleep\n");
        sem_post(&notFull);
    }
 }


int main(int argc, char *argv[]){
    int bees = (argc > 1) ? atoi(argv[1]) : DEFAULTBEES;
    maxHoney = (argc > 2 ) ? atoi(argv[2]) : rand()%10+5;
    eepy = (argc > 3) ? atoi(argv[3]) : rand()%3+1;

    honey = 0; 
    

    pthread_attr_t attr;
    pthread_t beeId[bees];

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&full, true, 0);	
    sem_init(&notFull, true, 1); 	

    for (int l = 0; l < bees; l++){
        pthread_create(&beeId[l], NULL, Bee, (void *)l);
        sleep(1);
    }
    
    Bear();

    return 0;
}

