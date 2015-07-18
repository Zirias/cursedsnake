#include "screen.h"

#include <stdlib.h>
#include <curses.h>

enum cpair
{
    CP_NONE,
    CP_WHITE,
    CP_RED
};

struct screen
{
    WINDOW *main;
    WINDOW *field;
    WINDOW *status;
    int w, h;
};

Screen *
screen_create()
{
    Screen *self = malloc(sizeof(Screen));
    self->main = initscr();
    if (!self->main)
    {
	free(self);
	return 0;
    }
    getmaxyx(self->main, self->h, self->w);
    start_color();
    init_pair(CP_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(CP_RED, COLOR_RED, COLOR_BLACK);
    bkgd(COLOR_PAIR(CP_WHITE));
    raw();
    noecho();
    return self;
}

void
screen_destroy(Screen *self)
{
    if (!self) return;
    endwin();
    free(self);
}

int
screen_width(const Screen *self)
{
    return self->w;
}

int
screen_height(const Screen *self)
{
    return self->h;
}

int
screen_getch(Screen *self)
{
    ((void)(self));

    return getch();
}
