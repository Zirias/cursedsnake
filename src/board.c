#include "board.h"
#include "screen.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>

struct board
{
    Screen *screen;
    int w, h;
    Item items[1];
};

Board *
board_create(void)
{
    Board *self;
    Screen *screen;
    int w, h;

    screen = screen_create();
    if (!screen) return 0;
    w = screen_width(screen);
    h = screen_height(screen) - 1;
    if (w < MIN_COLS || h < MIN_ROWS+1)
    {
	screen_destroy(screen);
	fputs("Console too small, need at least "
		STR(MIN_COLS) "x" STR(MIN_ROWS+1),
		stderr);
	return 0;
    }

    self = malloc(sizeof(Board) + (size_t)(w*h-1) * sizeof(Item));
    self->w = w;
    self->h = h;
    self->screen = screen;
    board_clear(self);
    return self;
}


void
board_clear(Board *self)
{ 
    int i;
    for (i=0; i<self->w*self->h; ++i) self->items[i] = EMPTY;
}

void
board_destroy(Board *self)
{
    if (!self) return;
    screen_getch(self->screen);
    screen_destroy(self->screen);
    free(self);
}

