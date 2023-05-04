/*
 * Problema da Montanha Russa.
 *
 * ALUNOS:
 * César Devens Grazioti - RA: 195641
 * João Miguel De Oliveira Guimarães - RA: 174358
 * Otavio Anovazzi - RA: 186331
 * Renan Luis Moraes De Sousa - RA: 243792
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_PASSENGER 15
#define CAPACITY 5

sem_t mutex;
sem_t mutex2;
sem_t boardQueue;
sem_t unboardQueue;
sem_t allAboard;
sem_t allAshore;
int boarders = 0;
int unboarders = 0;

/*
1 load() 
2 boardQueue.signal(C) 
3 allAboard.wait() 
4 
5 run() 
6 
7 unload() 
8 unboardQueue.signal(C) 
9 allAshore.wait()
*/

void* f_car(void *v) {
    while(1) {
        printf("Boarding time!\n");
        sem_post(&boardQueue);
        sem_wait(&allAboard);

        printf("Start of the tour.\n");
        sleep(3);
        printf("End of the tour.\n");

        printf("Unboarding time!\n");
        sem_post(&unboardQueue);
        sem_wait(&allAshore);
    }
    return NULL;
}

/*
1 boardQueue.wait() 
2 board() 
3 
4 mutex.wait() 
5   boarders+=1 
6   if boarders==C: 
7       allAboard.signal() 
8       boarders=0 
9 mutex.signal() 
10 
11 unboardQueue.wait() 
12 unboard() 
13 
14 mutex2.wait() 
15  unboarders+=1 
16  if unboarders==C: 
17      allAshore.signal() 
18      unboarders=0 
19 mutex2.signal()
*/
void* f_passenger(void *v) {
    int id = *(int*) v;

    sleep(random() % 3);

    sem_wait(&boardQueue);
    printf("Passenger %d boarded the car.\n", id);

    sem_wait(&mutex);
    boarders += 1;
    if (boarders == CAPACITY) {
        sem_post(&allAboard);
        printf("The car is complete with %d passengers.\n", CAPACITY);
        boarders = 0;
    }
    sem_post(&mutex);

    sem_wait(&unboardQueue);
    printf("Passenger %d unboarded the car.\n", id);

    sem_wait(&mutex2);
    unboarders += 1;
    if (unboarders == CAPACITY) {
        sem_post(&allAshore);
        printf("The %d passengers disembarked from the car.\n", CAPACITY);
        unboarders = 0;
    }
    sem_post(&mutex2);

    return NULL;
}

int main() {
    pthread_t thr_passenger[N_PASSENGER], thr_car;
    int i, id[N_PASSENGER];

    sem_init(&mutex, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&boardQueue, 0, 0);
    sem_init(&unboardQueue, 0, 0);
    sem_init(&allAboard, 0, 0);
    sem_init(&allAshore, 0, 0);

    for (i = 0; i < N_PASSENGER; i++) {
        id[i] = i;
        pthread_create(&thr_passenger[i], NULL, f_passenger, (void*) &id[i]);
    }

    pthread_create(&thr_car, NULL, f_car, NULL);

    for (i = 0; i < N_PASSENGER; i++) {
        pthread_join(thr_passenger[i], NULL);
    }

    return 0;
}