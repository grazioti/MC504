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

#include "buffer/buffer.h"
#include "primitives/primitives.h"

#define N_PASSENGER 16
#define CAPACITY 5
#define N_RUNS 3

#define WIDTH 160
#define HEIGHT 40

#define CART_STARTX 10
#define CART_STARTY 04
#define CART_PADDING 12

#define QUEUE_STARTX 20
#define QUEUE_STARTY 30

// Animation Masks
char *CART_HEAD =
    "             \n"
    "             \n"
    "             \n"
    "      _____  \n"
    "  II__|[ ]|  \n"
    " |        |_ \n"
    "< OO----OOO  \n";

char *CART_BODY =
    "      o      \n"
    "    -----    \n"
    "   /|%03d|\\   \n"
    "_____________\n"
    "|           |\n"
    "|___________|\n"
    "  OO----OO  \n";

char *CART_EMPTY =
    "             \n"
    "             \n"
    "             \n"
    "_____________\n"
    "|  [EMPTY]  |\n"
    "|___________|\n"
    "  OO----OO  \n";

char *PERSON_MASK =
    "   o   \n"
    " ----- \n"
    "/|%03d|\\\n"
    " |   | \n"
    " ----- \n"
    "/     \\\n";

char *COASTER_FLOOR =
    "========================================\n"
    "|     H*    H   *H         H*    H   *H|\n"
    "|     H *   H  * H         H *   H  * H|\n"
    "|     H  *  H *  H         H  *  H *  H|\n"
    "|     H   * **   H         H   ***    H|\n"
    "|     H  *  H *  H         H  *  H *  H|\n"
    "|     H *   H  * H         H *   H  * H|\n"
    "|     H*    H   *H         H*    H   *H|\n"
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n";

// Animation Variables
int CART_HEADX, CART_HEADY;
PrintBuffer BUF;
Queue Q;
Cart CART_LIST[CAPACITY];

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

// Auxiliary Function
void add_background(PrintBuffer *buffer) {
    int start_pos = 10;

    for (int i = 0; i < WIDTH / 40; i++)
        draw_object(buffer, COASTER_FLOOR, i * 40, start_pos);
}

void draw_train(PrintBuffer *buffer, Cart *cart_list, int dx) {
    const int space = 12;
    const int start_pos = 10;

    CART_HEADX += dx;
    CART_HEADY += 0;

    draw_queue(&Q, buffer);
    add_background(buffer);
    draw_object(buffer, CART_HEAD, CART_HEADX, CART_HEADY);
    for (int i = 0; i < CAPACITY; i++) {
        cart_list[i].x = cart_list[i].x + dx;
        draw_cart(&cart_list[i], buffer);
    }
}

void move_train(PrintBuffer *buffer, Cart *cart_list) {
    int dx = -10;

    for(int i = 0; i < 6; i++){
        draw_train(buffer, cart_list, i * dx);
        render_buffer(buffer, 1000000);
    }

    CART_HEADX = CART_STARTX;
    CART_HEADY = CART_STARTY;
    for (int i = 0; i < CAPACITY; i++) {
        set_position(&CART_LIST[i], CART_STARTX + (i + 1) * CART_PADDING, CART_STARTY);
    }
}

// Thread Functions
void* f_car(void *v) {
    int runs = 0;

    while(runs < N_RUNS) {
        for (int i = 0; i < CAPACITY; i++) {
            sem_post(&boardQueue);
        }
        sem_wait(&allAboard);

        sem_wait(&mutexPrint);
        move_train(&BUF, CART_LIST);
        sem_post(&mutexPrint);

        for (int i = 0; i < CAPACITY; i++) {
            sem_post(&unboardQueue);
        }
        sem_wait(&allAshore);

        runs++;
    }
    return NULL;
}

void* f_passenger(void *v) {
    int id, stime, pos;

    while(1) {
        id = *(int*) v;
        stime = random() % 3;

        sleep(stime);

        sem_wait(&boardQueue);

        sem_wait(&mutex);
        pos = boarders;
        set_value(&CART_LIST[pos], id);
        boarders += 1;

        sem_wait(&mutexPrint);

        remove_person(&Q, id);
        draw_queue(&Q, &BUF);

        draw_train(&BUF, CART_LIST, 0);
        render_buffer(&BUF, 1000000);

        sem_post(&mutexPrint);

        if (boarders == CAPACITY) {
            sem_post(&allAboard);
            boarders = 0;
        }
        sem_post(&mutex);

        sem_wait(&unboardQueue);
        
        
        sem_wait(&mutexPrint);

        //Sai do carro
        set_value(&CART_LIST[pos], -1);
        add_person(&Q, id, PERSON_MASK);
        draw_queue(&Q, &BUF);

        draw_train(&BUF, CART_LIST, 0);
        render_buffer(&BUF, 1000000);

        sem_post(&mutexPrint);

        sem_wait(&mutex2);
        unboarders += 1;
        if (unboarders == CAPACITY) {
            sem_post(&allAshore);
            unboarders = 0;
        }
        sem_post(&mutex2);
    }
    return NULL;
}

int main() {
    pthread_t thr_passenger[N_PASSENGER], thr_car;
    int id[N_PASSENGER];

    // Sets up animation variables
    init_buffer(&BUF, WIDTH, HEIGHT);
    fill_buffer(&BUF, ' ');
    init_queue(&Q, QUEUE_STARTX, QUEUE_STARTY, N_PASSENGER);

    CART_HEADX = CART_STARTX;
    CART_HEADY = CART_STARTY;
    for (int i = 0; i < CAPACITY; i++) {
        init_cart(&CART_LIST[i], CART_BODY, CART_EMPTY, CART_STARTX + (i + 1) * CART_PADDING, CART_STARTY);
    }

    // Sets up threads
    sem_init(&mutex, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&mutexPrint, 0, 1);
    sem_init(&boardQueue, 0, 0);
    sem_init(&unboardQueue, 0, 0);
    sem_init(&allAboard, 0, 0);
    sem_init(&allAshore, 0, 0);

    for (int i = 0; i < N_PASSENGER; i++) {
        id[i] = i;
        pthread_create(&thr_passenger[i], NULL, f_passenger, (void*) &id[i]);
        add_person(&Q, i, PERSON_MASK);
    }

    pthread_create(&thr_car, NULL, f_car, NULL);
    pthread_join(thr_car, NULL);

    return 0;
}