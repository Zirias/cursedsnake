#include "board.h"
#include "screen.h"

#include <stdio.h>

struct board
{
    Screen *screen;
    int w, h;
    Item items[1];
};

Board *
board_create(Screen *screen)
{
    Board *self;
    int w, h;

    w = screen_width(screen);
    h = screen_height(screen);
    if (w < MIN_COLS || h < MIN_ROWS)
    {
	screen_destroy(screen);
	fputs("Console too small, need at least "
		STR(MIN_COLS) "x" STR(MIN_ROWS) "\n", stderr);
	return 0;
    }
    --h;

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
    free(self);
}

void
board_size(const Board *self, Pos *pos)
{
    pos->y = self->h;
    pos->x = self->w;
}

void
board_set(Board *self, int y, int x, Item item)
{
    if (y<0 || y>=self->h || x<0 || x>=self->w) return;
    self->items[y*self->w+x] = item;
    screen_putItem(self->screen, y, x, item, 1);
}

Item
board_get(const Board *self, int y, int x)
{
    if (y<0 || y>=self->h || x<0 || x>=self->w) return WALL;
    return self->items[y*self->w+x];
}

void board_redraw(const Board *self)
{
    int y, x;
    for (y = 0; y < self->h; ++y) for (x = 0; x < self->w; ++x)
    {
	screen_putItem(self->screen, y, x, self->items[y*self->w+x], 0);
    }
    screen_refresh(self->screen);
}

