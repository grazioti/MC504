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
#include <string.h>

#include "sprites/sprites.h"
#include "animation/animation.h"
#include "animation/animation_config.h"
#include "buffer/buffer.h"
#include "primitives/primitives.h"

// Thread Control Variables
sem_t mutex;
sem_t mutex2;
sem_t mutexPrint;
sem_t boardQueue;
sem_t unboardQueue;
sem_t allAboard;
sem_t allAshore;
int boarders = 0;
int unboarders = 0;

void load()
{
    return;
}

void unload()
{
    return;
}

void run()
{
    move_train(&BUF, CART_LIST);
}

void board(int id, int pos)
{
    sem_wait(&mutexPrint);

    set_value(&CART_LIST[pos], id);
    remove_person(&Q, id);
    draw_queue(&Q, &BUF);
    draw_train(&BUF, CART_LIST, 0);
    render_buffer(&BUF, SLEEP_BOARD);

    sem_post(&mutexPrint);
}

void unboard(int id, int pos)
{
    sem_wait(&mutexPrint);

    set_value(&CART_LIST[pos], -1);
    add_person(&Q, id, PERSON_MASK);
    draw_queue(&Q, &BUF);
    draw_train(&BUF, CART_LIST, 0);
    render_buffer(&BUF, SLEEP_BOARD);

    sem_post(&mutexPrint);
}

// Thread Functions
void *f_car(void *v)
{
    int runs = 0;

    while (runs < N_RUNS)
    {
        load();
        for (int i = 0; i < CAPACITY; i++)
        {
            sem_post(&boardQueue);
        }
        sem_wait(&allAboard);

        sem_wait(&mutexPrint);
        run();
        sem_post(&mutexPrint);

        unload();
        for (int i = 0; i < CAPACITY; i++)
        {
            sem_post(&unboardQueue);
        }
        sem_wait(&allAshore);

        runs++;
    }
    return NULL;
}

void *f_passenger(void *v)
{
    int id, stime, pos;

    while (1)
    {
        id = *(int *)v;
        stime = random() % 3;

        sleep(stime);

        // Waits to board
        sem_wait(&boardQueue);
        sem_wait(&mutex);

        pos = boarders;
        board(id, pos);

        boarders += 1;
        if (boarders == CAPACITY)
        {
            sem_post(&allAboard);
            boarders = 0;
        }
        sem_post(&mutex);

        // Waits to Unboard
        sem_wait(&unboardQueue);
        unboard(id, pos);

        sem_wait(&mutex2);
        unboarders += 1;
        if (unboarders == CAPACITY)
        {
            sem_post(&allAshore);
            unboarders = 0;
        }
        sem_post(&mutex2);
    }
    return NULL;
}

int main()
{
    pthread_t thr_passenger[N_PASSENGER], thr_car;
    int id[N_PASSENGER];

    // Sets up terminal for rendering
    HIDE_CURSOR;
    CLEAR_TERMINAL;

    // Sets up animation variables
    init_buffer(&BUF, WIDTH, HEIGHT);
    fill_buffer(&BUF, ' ');
    init_queue(&Q, QUEUE_STARTX, HEIGHT - 11, N_PASSENGER);

    CART_HEADX = CART_STARTX;
    CART_HEADY = CART_STARTY;
    for (int i = 0; i < CAPACITY; i++)
    {
        init_cart(&CART_LIST[i], CART_BODY, CART_EMPTY, CART_STARTX + (i + 1) * CART_PADDING, CART_STARTY);
    }

    fill_buffer(&BUF, ' ');
    draw_object(&BUF, OPENING, WIDTH / 2 - 60, HEIGHT / 2 - 17);
    render_buffer(&BUF, SLEEP_AFTER);
    fill_buffer(&BUF, ' ');

    // Sets up threads
    sem_init(&mutex, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&mutexPrint, 0, 1);
    sem_init(&boardQueue, 0, 0);
    sem_init(&unboardQueue, 0, 0);
    sem_init(&allAboard, 0, 0);
    sem_init(&allAshore, 0, 0);

    for (int i = 0; i < N_PASSENGER; i++)
    {
        id[i] = i;
        pthread_create(&thr_passenger[i], NULL, f_passenger, (void *)&id[i]);
        add_person(&Q, i, PERSON_MASK);
    }

    pthread_create(&thr_car, NULL, f_car, NULL);
    pthread_join(thr_car, NULL);

    fill_buffer(&BUF, ' ');
    draw_object(&BUF, ENDING, WIDTH / 2 - 60, HEIGHT / 2 - 17);
    render_buffer(&BUF, SLEEP_AFTER);
    fill_buffer(&BUF, ' ');

    // Tears down rendering config
    SHOW_CURSOR;

    return 0;
}