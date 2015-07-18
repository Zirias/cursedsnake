#include "../../screen.h"

#include <stdlib.h>
#include <curses.h>

struct screen
{
    WINDOW *field;
    WINDOW *status;
};

Screen *
screen_create(void)
{
    Screen *self = malloc(sizeof(Screen));
    return self;
}

void
screen_destroy(Screen *self)
{
    free(self);
}
