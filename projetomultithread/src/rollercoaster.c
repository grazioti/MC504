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

#define WIDTH 200
#define HEIGHT 40

#define CART_STARTX 10
#define CART_STARTY 04
#define CART_PADDING 12

#define QUEUE_STARTX 20
#define QUEUE_STARTY 29

#define SLEEP_AFTER 2000000
#define TRAIN_CD 20000
#define SLEEP_BOARD 500000
// Title Screens
char *OPENING =
    " _______  _______  _        _        _______  _______    _______  _______  _______  _______ _________ _______  _______ \n"
    "(  ____ )(  ___  )( \\      ( \\      (  ____ \\(  ____ )  (  ____ \\(  ___  )(  ___  )(  ____ \\__   __/(  ____ \\(  ____ )\n"
    "| (    )|| (   ) || (      | (      | (    \\/| (    )|  | (    \\/| (   ) || (   ) || (    \\/   ) (   | (    \\/| (    )|\n"
    "| (____)|| |   | || |      | |      | (__    | (____)|  | |      | |   | || (___) || (_____    | |   | (__    | (____)|\n"
    "|     __)| |   | || |      | |      |  __)   |     __)  | |      | |   | ||  ___  |(_____  )   | |   |  __)   |     __)\n"
    "| (\\ (   | |   | || |      | |      | (      | (\\ (     | |      | |   | || (   ) |      ) |   | |   | (      | (\\ (   \n"
    "| ) \\ \\__| (___) || (____/\\| (____/\\| (____/\\| ) \\ \\__  | (____/\\| (___) || )   ( |/\\____) |   | |   | (____/\\| ) \\ \\__\n"
    "|/   \\__/(_______)(_______/(_______/(_______/|/   \\__/  (_______/(_______)|/     \\|\\_______)   )_(   (_______/|/   \\__/\n"
    "                             _________ _______    _______  _______  _______  _                                         \n"
    "                             \\__   __/(  ____ \\  (  ___  )(  ____ )(  ____ \\( (    /|                               \n"
    "                                ) (   | (    \\/  | (   ) || (    )|| (    \\/|  \\  ( |                               \n"
    "                                | |   | (_____   | |   | || (____)|| (__    |   \\ | |                                 \n"
    "                                | |   (_____  )  | |   | ||  _____)|  __)   | (\\ \\) |                                \n"
    "                                | |         ) |  | |   | || (      | (      | | \\   |                                 \n"
    "                             ___) (___/\\____) |  | (___) || )      | (____/\\| )  \\  |                               \n"
    "                             \\_______/\\_______)  (_______)|/       (_______/|/    )_)                                \n";

char *ENDING =
    " _______  _______  _        _        _______  _______    _______  _______  _______  _______ _________ _______  _______ \n"
    "(  ____ )(  ___  )( \\      ( \\      (  ____ \\(  ____ )  (  ____ \\(  ___  )(  ___  )(  ____ \\__   __/(  ____ \\(  ____ )\n"
    "| (    )|| (   ) || (      | (      | (    \\/| (    )|  | (    \\/| (   ) || (   ) || (    \\/   ) (   | (    \\/| (    )|\n"
    "| (____)|| |   | || |      | |      | (__    | (____)|  | |      | |   | || (___) || (_____    | |   | (__    | (____)|\n"
    "|     __)| |   | || |      | |      |  __)   |     __)  | |      | |   | ||  ___  |(_____  )   | |   |  __)   |     __)\n"
    "| (\\ (   | |   | || |      | |      | (      | (\\ (     | |      | |   | || (   ) |      ) |   | |   | (      | (\\ (   \n"
    "| ) \\ \\__| (___) || (____/\\| (____/\\| (____/\\| ) \\ \\__  | (____/\\| (___) || )   ( |/\\____) |   | |   | (____/\\| ) \\ \\__\n"
    "|/   \\__/(_______)(_______/(_______/(_______/|/   \\__/  (_______/(_______)|/     \\|\\_______)   )_(   (_______/|/   \\__/\n"
    "                          _________ _______    _______  _        _______  _______  _______  ______                     \n"
    "                          \\__   __/(  ____ \\  (  ____ \\( \\      (  ___  )(  ____ \\(  ____ \\(  __  \\                    \n"
    "                             ) (   | (    \\/  | (    \\/| (      | (   ) || (    \\/| (    \\/| (  \\  )                   \n"
    "                             | |   | (_____   | |      | |      | |   | || (_____ | (__    | |   ) |                   \n"
    "                             | |   (_____  )  | |      | |      | |   | |(_____  )|  __)   | |   | |                   \n"
    "                             | |         ) |  | |      | |      | |   | |      ) || (      | |   ) |                   \n"
    "                          ___) (___/\\____) |  | (____/\\| (____/\\| (___) |/\\____) || (____/\\| (__/  )                   \n"
    "                          \\_______/\\_______)  (_______/(_______/(_______)\\_______)(_______/(______/                    \n";

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

char *THREE =
    "   @@@@  \n"
    "  @@||@@ \n"
    " @@@||@@@\n"
    " @@@||@@@\n"
    "    ||   \n";

char *FLOOR =
    "##########\n"
    "oooooooooo\n"
    "oooooooooo\n"
    "oooooooooo\n"
    "oooooooooo\n";

char *ENTRANCE =
    "---------------\n"
    "|   ROLLER    |\n"
    "|   COASTER   |\n"
    "---------------\n"
    "||           ||\n"
    "||           ||\n"
    "||           ||\n"
    "||           ||\n"
    "||           ||\n"
    "||           ||\n";

char *EXIT =
    "---------------\n"
    "|    EXIT     |\n"
    "|             |\n"
    "---------------\n"
    "||           ||\n"
    "||           ||\n"
    "||           ||\n"
    "||           ||\n"
    "||           ||\n"
    "||           ||\n";

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
void add_background(PrintBuffer *buffer)
{
    int start_pos = 10;
    int floor_start_pos = HEIGHT - 5;

    for (int i = 0; i < WIDTH / 40; i++)
        draw_object(buffer, COASTER_FLOOR, i * 40, start_pos);

    for (int i = 0; i < WIDTH / 10; i++)
        draw_object(buffer, FLOOR, i * 10, floor_start_pos);

    draw_object(buffer, ENTRANCE, 1, HEIGHT - 15);
    draw_object(buffer, EXIT, WIDTH - 16, HEIGHT - 15);
}

void draw_train(PrintBuffer *buffer, Cart *cart_list, int dx)
{
    const int space = 12;
    const int start_pos = 10;

    CART_HEADX += dx;
    CART_HEADY += 0;

    draw_queue(&Q, buffer);
    add_background(buffer);
    draw_object(buffer, CART_HEAD, CART_HEADX, CART_HEADY);
    for (int i = 0; i < CAPACITY; i++)
    {
        cart_list[i].x = cart_list[i].x + dx;
        draw_cart(&cart_list[i], buffer);
    }
}

void move_train(PrintBuffer *buffer, Cart *cart_list)
{
    int dx = -1;

    for (int i = 0; i < WIDTH / abs(dx); i++)
    {
        draw_train(buffer, cart_list, dx);
        render_buffer(buffer, TRAIN_CD);
    }

    CART_HEADX = CART_STARTX;
    CART_HEADY = CART_STARTY;
    for (int i = 0; i < CAPACITY; i++)
    {
        set_position(&CART_LIST[i], CART_STARTX + (i + 1) * CART_PADDING, CART_STARTY);
    }
}

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
    // Sets up animation variables
    HIDE_CURSOR;
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
    SHOW_CURSOR;
    return 0;
}