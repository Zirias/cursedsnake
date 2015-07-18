#include "screen.h"

#include <curses.h>
#include <string.h>

enum cpair
{
    CP_NONE,
    CP_WHITE,
    CP_RED,
    CP_YELLOW,
    CP_GREEN,
    CP_STATUS
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
    char *tmp;

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
    init_pair(CP_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CP_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(CP_STATUS, COLOR_BLACK, COLOR_CYAN);
    bkgd(COLOR_PAIR(CP_WHITE));
    raw();
    noecho();
    curs_set(0);
    self->status = newwin(1, 0, 0, 0);
    self->field = newwin(0, 0, 1, 0);
    wbkgd(self->status, COLOR_PAIR(CP_STATUS)|A_BLINK);
    tmp = malloc((size_t)self->w+1);
    memset(tmp, ' ' + 0x80, (size_t)self->w);
    tmp[self->w] = 0;
    insertString(tmp, 1, "cursed snake");
    insertString(tmp, self->w - 16, "score:");
    waddstr(self->status, tmp);
    free(tmp);
    refresh();
    wrefresh(self->status);
    return self;
}

void
screen_destroy(Screen *self)
{
    if (!self) return;
    delwin(self->status);
    delwin(self->field);
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

void
screen_printScore(Screen *self, unsigned int score)
{
    mvwprintw(self->status, 0, self->w-9, "%8u", score);
    wrefresh(self->status);
}

void
screen_putItem(Screen *self, int y, int x, Item item)
{
    switch (item)
    {
	case EMPTY:
	    mvwaddch(self->field, y, x, ' '|COLOR_PAIR(CP_WHITE));
	    break;
	case HEAD:
#ifdef WIN32
	    mvwaddch(self->field, y, x,
		    1|COLOR_PAIR(CP_YELLOW)|A_BOLD|A_ALTCHARSET);
#else
	    mvwaddch(self->field, y, x, '@'|COLOR_PAIR(CP_YELLOW)|A_BOLD);
#endif
	    break;
	case TAIL:
	    mvwaddch(self->field, y, x, ACS_CKBOARD|COLOR_PAIR(CP_YELLOW));
	    break;
	case FOOD:
	    mvwaddch(self->field, y, x, '#'|COLOR_PAIR(CP_GREEN)|A_BOLD);
	    break;
	case WALL:
	    mvwaddch(self->field, y, x,
		    (' '+0x80)|COLOR_PAIR(CP_RED)|A_REVERSE);
	    break;
    }
    wrefresh(self->field);
}

