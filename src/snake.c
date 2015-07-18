#include "snake.h"

#include "board.h"

struct snake
{
    Board *b;
    int len;
    int maxlen;
    int grow;
    Dir dir;
    Pos *head;
    Pos *tail;
    Pos coord[1];
};

Snake *
snake_create(Board *b, int y, int x)
{
    Snake *self;
    Pos boardsize;
    int maxlen;

    board_size(b, &boardsize);
    maxlen = (int)(boardsize.x * boardsize.y * .6 + 1.0);

    self = malloc(sizeof(Snake) + (size_t)(maxlen) * sizeof(Pos));
    self->len = 0;
    self->maxlen = maxlen;
    self->grow = 8;
    self->dir = LEFT;
    self->head = &(self->coord[0]);
    self->tail = &(self->coord[0]);
    self->coord[0].y = y;
    self->coord[0].x = x;
    board_set(b, y, x, HEAD);
    return self;
}

void
snake_destroy(Snake *self)
{
    free(self);
}

void
snake_setDir(Snake *self, Dir dir)
{
    self->dir = dir;
}

void
snake_grow(Snake *self, int grow)
{
    self->grow += grow;
}

Step
snake_step(Snake *self)
{
    Pos *newHead;
    Item item;

    newHead = self->head + 1;
    if (newHead > &(self->coord[self->maxlen])) newHead = &(self->coord[0]);
    newHead->x = self->head->x;
    newHead->y = self->head->y;
    switch (self->dir)
    {
	case LEFT:
	    ++newHead->x;
	    break;
	case DOWN:
	    ++newHead->y;
	    break;
	case RIGHT:
	    --newHead->y;
	    break;
	case UP:
	    --newHead->x;
	    break;
    }
    item = board_get(self->b, newHead->y, newHead->x);
    if (item == HEAD || item == TAIL || item == WALL) return SST_HIT;
    board_set(self->b, self->head->y, self->head->x, TAIL);
    board_set(self->b, newHead->y, newHead->x, HEAD);
    self->head = newHead;

    if (self->grow)
    {
	--self->grow;
	++self->len;
	if (self->len == self->maxlen) return SST_MAXLEN;
    }
    else
    {
	board_set(self->b, self->tail->y, self->tail->x, EMPTY);
	++self->tail;
	if (self->tail > &(self->coord[self->maxlen]))
	{
	    self->tail = &(self->coord[0]);
	}
    }
    if (item == FOOD) return SST_FOOD;
    return SST_NORMAL;
}

