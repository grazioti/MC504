#ifndef ANIMATION_H
#define ANIMATION_H

#include "../buffer/buffer.h"
#include "../primitives/primitives.h"
#include "../rollercoaster_config.h"

#define CART_STARTX  10
#define CART_STARTY  04
#define CART_PADDING 12

#define QUEUE_STARTX 20
#define QUEUE_STARTY 29

// Animation Variables
extern int CART_HEADX, CART_HEADY;
extern PrintBuffer BUF;
extern Queue Q;
extern Cart CART_LIST[CAPACITY];

// Animation Functions
void add_background(PrintBuffer *buffer);
void draw_train(PrintBuffer *buffer, Cart *cart_list, int dx);
void move_train(PrintBuffer *buffer, Cart *cart_list);

#endif