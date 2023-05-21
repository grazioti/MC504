#include "../buffer/buffer.h"

typedef struct Person {
    char *body;
    char *mask;
    char value;
    int x, y;
} Person;

typedef struct Queue {
    int x, y;
    int max_size, n;
    Person *people;
} Queue;

typedef struct Cart {
    char *body;
    char *mask;
    char *empty_mask;
    char value;
    int x, y;
} Cart;

// Person Methods
void draw_person(Person p, PrintBuffer *buffer);

// Queue Methods
void init_queue(Queue *l, int x, int y, int max_size);
void add_person(Queue *l, int value, char *person_mask);
void remove_person(Queue *l, int value);
void draw_queue(Queue *l, PrintBuffer *buffer);

// Cart Methods
void init_cart(Cart *c, char *mask, char *empty_mask, int x, int y);
void set_value(Cart *c, int value);
void set_position(Cart *c, int x, int y);
void draw_cart(Cart *c, PrintBuffer *buffer);