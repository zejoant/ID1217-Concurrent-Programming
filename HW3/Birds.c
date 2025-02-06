#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>


#define DEFAULTBIRDS 10
int worms;
sem_t empty;
sem_t notEmpty;

void *babyBird(void *arg){

    int babyBirdId = (int)arg;

    while(true){
        sem_wait(&notEmpty);
        if(worms > 0){
            worms--;
            printf("baby bird %d eats worm, worms left: %d\n", babyBirdId, worms);
            printf("baby bird %d sleeps\n", babyBirdId);
            sem_post(&notEmpty);
            sleep(rand()%3+1);
        }
        else{
            printf("BABY BIRD %d ALERTS PARENT \n", babyBirdId);
            sem_post(&empty);
            sleep(2);
        }
    }

 }

 void parentBird(){

    while(true){
        sem_wait(&empty);
        printf("parent bird looks for worms\n");
        worms = rand()%10+5;
        sleep(2);
        printf("parent bird found %d worms\n", worms);
        sem_post(&notEmpty);
    }
 }


int main(int argc, char *argv[]){
    int babyBirds = (argc > 1) ? atoi(argv[1]) : DEFAULTBIRDS;

    pthread_attr_t attr;
    pthread_t babyBirdId[babyBirds];

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    worms = rand()%10+5;
    sem_init(&notEmpty, true, 1);	
    sem_init(&empty, true, 0); 	

    for (int l = 0; l < babyBirds; l++){
        pthread_create(&babyBirdId[l], NULL, babyBird, (void *)l);
    }
    
    parentBird();

    return 0;
}


