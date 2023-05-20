#include "../buffer/buffer.h"
#include "primitives.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Person Methods
void draw_person(Person p, PrintBuffer *buffer) {
    sprintf(p.body, p.mask, p.value);
    draw_object(buffer, p.body, p.x, p.y);
}

// Queue Methods
void init_queue(Queue *l, int x, int y, int max_size) {
    l->x = x;
    l->y = y;
    l->max_size = max_size;
    l->n = 0;
    l->people = malloc(sizeof(Person) * max_size);
}

void add_person(Queue *l, int value, char *person_mask) {
    int i = l->n;

    l->people[i].body = malloc(sizeof(char *) * strlen(person_mask));
    l->people[i].mask = malloc(sizeof(char *) * strlen(person_mask));
    l->people[i].value = value;
    l->people[i].x = l->x + 7*i;
    l->people[i].y = l->y;
    l->n++;

    strcpy(l->people[i].mask, person_mask);
}

void remove_person(Queue *l, int value) {
    int person_pos;

    for (int i = 0; i < l->n; i++) {
        if (l->people[i].value == value) {
            person_pos = i;
            break;
        }
    }

    for (int i = person_pos; i < l->n; i++) {
        l->people[i].value = l->people[i+1].value;
        l->people[i].x = l->x + 7*i;
        l->people[i].y = l->y;
    }
    l->n -= 1;
}

void draw_queue(Queue l, PrintBuffer *buffer) {
    for (int i = 0; i < l.n; i++) {
        draw_person(l.people[i], buffer);
    }
}

// Cart Methods
void init_cart(Cart *c, char *mask, char *empty_mask) {
    c->empty_mask = malloc(sizeof(char) * strlen(empty_mask));
    c->mask = malloc(sizeof(char) * strlen(mask));
    c->body = malloc(sizeof(char) * strlen(mask));
    c->x = 0;
    c->y = 0;
    c->value = -1; // EMPTY
    strcpy(c->empty_mask, empty_mask);
    strcpy(c->mask, mask);
}

void set_value(Cart *c, int value) {
    c->value = value;
}

void draw_cart(Cart *c, PrintBuffer *buffer) {
    if (c->value == -1)
        sprintf(c->body, c->empty_mask, c->value);
    else
        sprintf(c->body, c->mask, c->value);

    draw_object(buffer, c->body, c->x, c->y);
}
