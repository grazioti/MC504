#include <stdio.h>
#include "buffer/buffer.h"

int main() {
    PrintBuffer buf;
    char *cabeca =
    "      _____  \n"
    "  II__|[ ]|  \n"
    " |        |_ \n"
    "< OO----OOO  \n";

    init_buffer(&buf, 140, 60);
    fill_buffer(&buf, ' ');
    draw_object(&buf, cabeca, 10, 10);
    render_buffer(&buf, 0);

    return 0;
}