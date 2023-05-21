#include <stdlib.h>

#include "../buffer/buffer.h"
#include "../primitives/primitives.h"
#include "../sprites/sprites.h"
#include "animation.h"
#include "animation_config.h"

#define TRAIN_CD 1000000 / TRAIN_MOVE_FPS

int CART_HEADX, CART_HEADY;
PrintBuffer BUF;
Queue Q;
Cart CART_LIST[CAPACITY];

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
    float dx = -WIDTH / TRAIN_MOVE_FPS;

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