#include "board.h"
#include "screen.h"
#include "food.h"

#include <stdio.h>
#include <string.h>

typedef struct
{
    Item it;
    Food *f;
} Slot;

struct board
{
    Screen *screen;
    int w, h;
    Slot slots[1];
};

Board *
board_create(Screen *screen)
{
    Board *self;
    size_t size;
    int w, h;

    w = screen_width(screen);
    h = screen_height(screen);
    if (w < MIN_COLS || h < MIN_ROWS)
    {
	fputs("Console too small, need at least "
		STR(MIN_COLS) "x" STR(MIN_ROWS) "\n", stderr);
	return 0;
    }
    --h;

#ifdef DOSREAL
    size = sizeof(Board);
    for (int i = 0; i < w*h-1; ++i) size += sizeof(Slot);
#else
    size = sizeof(Board) + (size_t)(w*h-1) * sizeof(Slot);
#endif
    self = malloc(size);
    memset(self, 0, size);
    self->w = w;
    self->h = h;
    self->screen = screen;
    return self;
}


void
board_clear(Board *self)
{ 
    int i;
    for (i=0; i<self->w*self->h; ++i)
    {
	self->slots[i].it = EMPTY;
	if (self->slots[i].f)
	{
	    food_destroy(self->slots[i].f);
	    self->slots[i].f = 0;
	}
    }
}

void
board_destroy(Board *self)
{
    int i;
    if (!self) return;
    for (i=0; i<self->w*self->h; ++i)
    {
	if (self->slots[i].f) food_destroy(self->slots[i].f);
    }
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
    int idx;
    if (y<0 || y>=self->h || x<0 || x>=self->w) return;

    idx = y*self->w+x;

    self->slots[idx].it = item;
    if (item == FOOD)
    {
	self->slots[idx].f = food_create(self, self->screen, y, x);
	food_draw(self->slots[idx].f);
    }
    else
    {
	food_destroy(self->slots[idx].f);
	self->slots[idx].f = 0;
	screen_putItem(self->screen, y, x, item);
    }
}

Item
board_get(const Board *self, int y, int x)
{
    if (y<0 || y>=self->h || x<0 || x>=self->w) return WALL;
    return self->slots[y*self->w+x].it;
}

void board_redraw(const Board *self)
{
    int idx, y, x;
    for (y = 0; y < self->h; ++y) for (x = 0; x < self->w; ++x)
    {
	idx = y*self->w+x;
	if (self->slots[idx].it == FOOD)
	{
	    food_draw(self->slots[idx].f);
	}
	else
	{
	    screen_putItem(self->screen, y, x, self->slots[idx].it);
	}
    }
}

