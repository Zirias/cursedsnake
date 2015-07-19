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
	screen_destroy(screen);
	fputs("Console too small, need at least "
		STR(MIN_COLS) "x" STR(MIN_ROWS) "\n", stderr);
	return 0;
    }
    --h;

    size = sizeof(Board) + (size_t)(w*h-1) * sizeof(Slot);
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
	if (self->slots[i].f) food_destroy(self->slots[i].f);
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
	screen_putItem(self->screen, y, x, item, 1);
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
    int y, x;
    for (y = 0; y < self->h; ++y) for (x = 0; x < self->w; ++x)
    {
	screen_putItem(self->screen, y, x, self->slots[y*self->w+x].it, 0);
    }
    screen_refresh(self->screen);
}

