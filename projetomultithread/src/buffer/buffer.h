#ifndef BUFFER_H
#define BUFFER_H

#define clear_terminal printf("\033[H\033[J")
#define HIDE_CURSOR printf("\033[?25l")
#define SHOW_CURSOR printf("\033[?25h")
typedef struct PrintBuffer
{
    int screen_width, screen_height;
    int buffer_size;
    char *buffer;
} PrintBuffer;

void init_buffer(PrintBuffer *buffer, int screen_width, int screen_height);
void fill_buffer(PrintBuffer *buffer, char c);
void draw_object(PrintBuffer *buffer, char *object, int x, int y);
void render_buffer(PrintBuffer *buffer, int sleepAfter);
#endif
