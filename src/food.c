#include "food.h"

#include "board.h"
#include "screen.h"

struct food
{
    Board *b;
    Screen *s;
    Food *next;
    Food *prev;
    int y;
    int x;
    int time;
};

static Food *all = 0;
static food_callback create = 0;
static food_callback destroy = 0;

Food *
food_create(Board *b, Screen *s, int y, int x)
{
    Food *self = malloc(sizeof(Food));
    self->b = b;
    self->s = s;
    self->y = y;
    self->x = x;
    self->time = randomNum(1500, 3000);
    if (all)
    {
	self->prev = all->prev;
	self->next = all;
	all->prev->next = self;
	all->prev = self;
    }
    else
    {
	all = self->prev = self->next = self;
    }
    if (create) create(self);
    return self;
}

void
food_destroy(Food *self)
{
    if (!self) return;
    if (destroy) destroy(self);
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (self == all)
    {
	all = self->next == all ? 0 : self->next;
    }
    free(self);
}

static void
tick(Food *self)
{
    if (--self->time == 300) food_draw(self);
    if (!self->time) board_set(self->b, self->y, self->x, EMPTY);
}

void
food_tick(void)
{
    Food *f, *n;

    if (!all) return;
    f = all;
    do { n = f->next; tick(f); f = n; } while (f != all);
}

void
food_draw(Food *self)
{
    screen_putItem(self->s, self->y, self->x,
	    self->time > 300 ? FOOD : FFOOD);
}

void
food_onCreate(food_callback cb)
{
    create = cb;
}

void
food_onDestroy(food_callback cb)
{
    destroy = cb;
}

