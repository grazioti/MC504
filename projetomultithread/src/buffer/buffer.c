#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "buffer.h"

void init_buffer(PrintBuffer *buffer, int screen_width, int screen_height)
{
    int buffer_size = screen_height * screen_width + 1;

    buffer->screen_width = screen_width;
    buffer->screen_height = screen_height;
    buffer->buffer_size = buffer_size;
    buffer->buffer = malloc(sizeof(char) * buffer_size);
}

void fill_buffer(PrintBuffer *buffer, char c)
{
    for (int i = 0; i < buffer->buffer_size; i++)
    {
        buffer->buffer[i] = c;
    }
    buffer->buffer[buffer->buffer_size - 1] = '\0';
}

void draw_object(PrintBuffer *buffer, char *object, int x, int y)
{
    int nScreenWidth = buffer->screen_width;
    int nScreenHeight = buffer->screen_height;
    int _x = x;
    int _y = y;
    int listPos;

    for (int i = 0; object[i] != '\0'; i++)
    {
        listPos = _y * (nScreenWidth) + _x;
        if (object[i] == '\n')
        {
            _y++;
            _x = x;
            continue;
        }
        else if (listPos <= nScreenWidth * nScreenHeight)
        {
            buffer->buffer[listPos] = object[i];
        }

        _x++;
    }
}

void printc(PrintBuffer *buffer)
{
    printf("\033[0;0H");
    for (int i = 0; buffer->buffer[i] != '\0'; i++)
    {
        putchar(buffer->buffer[i]);
    }
    fflush(stdout);
}

void render_buffer(PrintBuffer *buffer, int sleepAfter)
{
    printc(buffer);
    fflush(stdout);
    fill_buffer(buffer, ' ');

    if (sleepAfter > 0)
        usleep(sleepAfter);
}
