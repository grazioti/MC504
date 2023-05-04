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


#define N_PASSENGER 16
#define CAPACITY 5
#define N_RUNS 3

sem_t mutex;
sem_t mutex2;
sem_t boardQueue;
sem_t unboardQueue;
sem_t allAboard;
sem_t allAshore;
int boarders = 0;
int unboarders = 0;


void* f_car(void *v) {
    int runs = 0;
    printf("The roller coaster has opened, let's go!\n");
    while(runs < N_RUNS) {
        printf("Boarding time!\n");
        for (int i = 0; i < CAPACITY; i++) {
            sem_post(&boardQueue);
        }
        sem_wait(&allAboard);

        printf("Start of the tour.\n");
        sleep(3);
        printf("End of the tour.\n");

        printf("Unboarding time!\n");
        for (int i = 0; i < CAPACITY; i++) {
            sem_post(&unboardQueue);
        }
        sem_wait(&allAshore);

        runs++;
    }
    printf("It was good, but it's over! Time to close the park.\n");
    return NULL;
}

void* f_passenger(void *v) {
    while (1) {
        int id = *(int*) v;
        int time = random() % 3;

        // printf("Passanger %d waiting for %d seconds\n", id, time);
        sleep(time);

        sem_wait(&boardQueue);

        // printf("Id before wait: %d\n",id);
        sem_wait(&mutex);
        boarders += 1;
        printf("Passenger %d boarded the car.\n", id);
        if (boarders == CAPACITY) {
            sem_post(&allAboard);
            printf("The car is complete with %d passengers.\n", CAPACITY);
            boarders = 0;
        }
        // printf("Id before post: %d\n",id);
        sem_post(&mutex);
        // printf("Id after post: %d\n",id);

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

    }

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

    pthread_join(thr_car, NULL);

    // for (i = 0; i < N_PASSENGER; i++) {
    //     pthread_join(thr_passenger[i], NULL);
    // }

    return 0;
}