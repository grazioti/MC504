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

#define boolean int
#define true 1
#define false 0

#define N_PASSENGER 16
#define CAPACITY 5
#define N_RUNS 3

#define EMPTY -1

int WIDTH = 160;
int HEIGHT = 40;

char *PRINTBUFFER;

char *cabeca =
    "      _____  \n"
    "  II__|[ ]|  \n"
    " |        |_ \n"
    "< OO----OOO  \n";

char *corpo =
    "_____________\n"
    "|   [%03d]   |\n"
    "|___________|\n"
    "  OO----OO  \n";

char *corpo_vazio =
    "_____________\n"
    "|  [EMPTY]  |\n"
    "|___________|\n"
    "  OO----OO  \n";

char *person_mask =
    "   o   \n"
    " ----- \n"
    "/|%03d|\\\n"
    " |   | \n"
    " ----- \n"
    "/     \\\n";


typedef struct Person {
    char *body;
    char *mask;
    char value;
    int x, y;
} Person;

typedef struct Line {
    int x, y;
    int max_size, n;
    Person *people;
} Line;

typedef struct Vagao
{
    char *corpo;
    char pessoa;
    int x;
    int y;
} Vagao;

Line l;
Vagao listaVagoes[CAPACITY];

void init_line(Line *l, int x, int y, int max_size) {
    l->x = x;
    l->y = y;
    l->max_size = max_size;
    l->n = 0;
    l->people = malloc(sizeof(Person) * max_size);
}

void add_person(Line *l, int value) {
    int i = l->n;

    l->people[i].body = malloc(sizeof(char *) * strlen(person_mask));
    l->people[i].mask = malloc(sizeof(char *) * strlen(person_mask));
    l->people[i].value = value;
    l->people[i].x = l->x + 7*i;
    l->people[i].y = l->y;
    l->n++;

    strcpy(l->people[i].mask, person_mask);
}

void remove_person(Line *l, int value) {
    int person_pos;

    for (int i = 0; i < l->n; i++) {
        if (l->people[i].value == value) {
            person_pos = i;
            break;
        }
    }

    for (int i = person_pos; i < l->n; i++) {
        l->people[i].value = l->people[i+1].value;

        // FIX-ME: Atualizar a posicao para a do anterior
        // deveria funcionar mas pessoas sao renderizadas
        // no mesmo lugar
        // l->people[i].x = l->people[i+1].x;
        // l->people[i].y = l->people[i+1].y;

        // Recalcula posicao das pessoas
        // OBS: Nao deveria ser necessario
        // Ver acima
        l->people[i].x = l->x + 7*i;
        l->people[i].y = l->y;
    }
    l->n -= 1;
}

void render_person(Person p, char *buffer) {
    int nScreenWidth = WIDTH;
    int nScreenHeight = HEIGHT;
    int y = p.y;
    int x = p.x;
    int i = 0;

    sprintf(p.body, p.mask, p.value);
    while (p.body[i] != '\0') {
        int listPos = y * (nScreenWidth) + x;
        if (p.body[i] == '\n') {
            y++;
            x = p.x;
            i++;
            continue;
        }
        else if (listPos <= nScreenWidth * nScreenHeight) {
            buffer[listPos] = p.body[i];
        }
        x++;
        i++;
    }
}
 
void render_line(Line l) {
    for (int i = 0; i < l.n; i++) {
        render_person(l.people[i], PRINTBUFFER);
    }
}

Vagao gerar_vagao_padrao() {
    Vagao _vagao;
    _vagao.corpo = malloc(sizeof(char) * strlen(corpo));
    strcpy(_vagao.corpo, corpo);
    _vagao.x = 0;
    _vagao.y = 0;
    _vagao.pessoa = EMPTY;

    return _vagao;
}

void set_pessoa_vagao(Vagao vagao, int pessoa) {
    vagao.pessoa = pessoa;
    // TODO
}

void adicionar_vagao_no_buffer(Vagao vagao, char *buffer, int _x, int _y, int pessoa) {   
    if(pessoa == EMPTY)
        sprintf(vagao.corpo, corpo_vazio, pessoa);
    else
        sprintf(vagao.corpo, corpo, pessoa);

    int nScreenWidth = WIDTH;
    int nScreenHeight = HEIGHT;
    vagao.x = _x;
    vagao.y = _y;
    int y = vagao.y;
    int x = vagao.x;
    int i = 0;
    while (vagao.corpo[i] != '\0') {
        int listPos = y * (nScreenWidth) + x;
        if (vagao.corpo[i] == '\n') {
            y++;
            x = vagao.x;
            i++;
            continue;
        }
        else if (listPos <= nScreenWidth * nScreenHeight) {
            buffer[listPos] = vagao.corpo[i];
        }
        x++;
        i++;
    }
}

void print_on_screen(boolean sleepAfter) {
    system("clear");
    printf("%s", PRINTBUFFER);
    fflush(stdout);
    for (int i = 0; i < (sizeof(char) * WIDTH * HEIGHT); i++){
        PRINTBUFFER[i] = ' ';
    }
    if(sleepAfter)
        usleep(1000000);
}

void adicionar_char_no_buffer(char *origem, char *buffer, int _x, int _y, int valor) {
    char *_origem = malloc(sizeof(char) * strlen(origem));
    sprintf(_origem, origem, valor);
    int nScreenWidth = WIDTH;
    int nScreenHeight = HEIGHT;

    int y = _y;
    int x = _x;
    int i = 0;
    while (_origem[i] != '\0') {

        int listPos = y * (nScreenWidth) + x;
        if (_origem[i] == '\n') {
            y++;
            x = _x;
            i++;
            continue;
        }
        else if (listPos <= nScreenWidth * nScreenHeight) {

            buffer[listPos] = _origem[i];
        }
        x++;
        i++;
    }
}

void adicionar_trem_buffer(Vagao * v, int dx) {
    const int space = 12;
    const int startPos = 10;
    adicionar_char_no_buffer(cabeca, PRINTBUFFER, startPos + dx, 10, 0);
    for (int i = 0; i < CAPACITY; i++) {
        adicionar_vagao_no_buffer(v[i], PRINTBUFFER, startPos + space*(i+1) + dx, 10, v[i].pessoa);
    }
}

void move_train() {
    int dx = -5;
    for(int i = 0 ; i < WIDTH/abs(dx) ; i ++){
        adicionar_trem_buffer(listaVagoes,i*dx);
        print_on_screen(false);
        usleep(100000);
    }
}

void inicializar_vagoes(Vagao * v) {
    for(int i = 0 ; i < CAPACITY ; i++){
        v[i] = gerar_vagao_padrao();
    }
}

void print_s(char * string , char * buffer ,int d , int x, int y){
    adicionar_char_no_buffer(string,buffer,x,y,d);
}

sem_t mutex;
sem_t mutex2;
sem_t mutexPrint;
sem_t boardQueue;
sem_t unboardQueue;
sem_t allAboard;
sem_t allAshore;
int boarders = 0;
int unboarders = 0;


void* f_car(void *v) {
    int runs = 0;
    //printf("The roller coaster has opened, let's go!\n");
    while(runs < N_RUNS) {
        //printf("Boarding time!\n");
        for (int i = 0; i < CAPACITY; i++) {
            sem_post(&boardQueue);
        }
        sem_wait(&allAboard);

        sem_wait(&mutexPrint);
        //printf("Start of the tour.\n");
        move_train();
        //printf("End of the tour.\n");
        sem_post(&mutexPrint);

        //printf("Unboarding time!\n");
        for (int i = 0; i < CAPACITY; i++) {
            sem_post(&unboardQueue);
        }
        sem_wait(&allAshore);

        runs++;
    }
    //printf("It was good, but it's over! Time to close the park.\n");
    return NULL;
}

void* f_passenger(void *v) {
    while(1) {
        int id = *(int*) v;
        int time = random() % 3;

        sleep(time);

        sem_wait(&boardQueue);

        sem_wait(&mutex);
        int pos = boarders;
        listaVagoes[pos].pessoa = id;
        boarders += 1;

        sem_wait(&mutexPrint);
        //printf("Passenger %d boarded the car.\n", id);
        remove_person(&l,id);
        //entra no carro
        render_line(l);
        adicionar_trem_buffer(listaVagoes,0);
        print_on_screen(true);

        sem_post(&mutexPrint);

        if (boarders == CAPACITY) {
            sem_post(&allAboard);
            //printf("The car is complete with %d passengers.\n", CAPACITY);
            
            boarders = 0;
        }
        sem_post(&mutex);

        sem_wait(&unboardQueue);
        
        
        sem_wait(&mutexPrint);

        //printf("Passenger %d unboarded the car.\n", id);
        //Sai do carro
        listaVagoes[pos].pessoa = EMPTY;
        add_person(&l,id);
        render_line(l);
        adicionar_trem_buffer(listaVagoes,0);
        print_on_screen(true);

        sem_post(&mutexPrint);

        sem_wait(&mutex2);
        unboarders += 1;
        if (unboarders == CAPACITY) {
            sem_post(&allAshore);
            //printf("The %d passengers disembarked from the car.\n", CAPACITY);
            unboarders = 0;
        }
        sem_post(&mutex2);
    }
    return NULL;
}


int main() {
    system("clear");
    PRINTBUFFER = malloc((sizeof(char) * WIDTH * HEIGHT) + 1);
    for (int i = 0; i < (sizeof(char) * WIDTH * HEIGHT); i++)
    {
        PRINTBUFFER[i] = ' ';
    }
    PRINTBUFFER[sizeof(char) * WIDTH * HEIGHT] = '\0';

    //Vagao vagao = gerar_vagao_padrao();


    init_line(&l, 20, 20, N_PASSENGER);
    inicializar_vagoes(listaVagoes);

    int dx = 0;
     

    pthread_t thr_passenger[N_PASSENGER], thr_car;
    int i, id[N_PASSENGER];

    sem_init(&mutex, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&mutexPrint, 0, 1);
    sem_init(&boardQueue, 0, 0);
    sem_init(&unboardQueue, 0, 0);
    sem_init(&allAboard, 0, 0);
    sem_init(&allAshore, 0, 0);

    for (i = 0; i < N_PASSENGER; i++) {
        id[i] = i;
        pthread_create(&thr_passenger[i], NULL, f_passenger, (void*) &id[i]);
        add_person(&l, i);
    }

    pthread_create(&thr_car, NULL, f_car, NULL);

    pthread_join(thr_car, NULL);

    // for (i = 0; i < N_PASSENGER; i++) {
    //     pthread_join(thr_passenger[i], NULL);
    // }

    return 0;
}