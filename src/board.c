#include "board.h"
#include "screen.h"

#include <stdlib.h>

struct board
{
    Item items[BOARD_ROWS][BOARD_COLS];
    Screen *screen;
};

Board *
board_create(void)
{
    Board *self = malloc(sizeof(Board));
    board_clear(self);
    self->screen = screen_create();
    return self;
}


void
board_clear(Board *self)
{ 
    int i;
    for (i=0; i<BOARD_ROWS*BOARD_COLS; ++i) *(self->items[0]+i) = EMPTY;
}

void
board_destroy(Board *self)
{
    if (!self) return;
    screen_destroy(self->screen);
    free(self);
}

