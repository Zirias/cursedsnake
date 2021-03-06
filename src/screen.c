#define _POSIX_C_SOURCE 200112L
#include "screen.h"

#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#if (defined(WIN32) || defined(DOS))
#define USEMSCP
#endif

enum cpair
{
    CP_NONE,
    CP_WHITE,
    CP_RVRED,
    CP_YELLOW,
    CP_GREEN,
    CP_STATUS,
    CP_DIALOG
};

struct screen
{
    WINDOW *field;
    WINDOW *status;
    int w, h;
};

Screen *
screen_create()
{
    Screen *self = malloc(sizeof(Screen));

    if (!initscr())
    {
	free(self);
	return 0;
    }
    getmaxyx(stdscr, self->h, self->w);
    start_color();
    init_pair(CP_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(CP_RVRED, COLOR_BLACK, COLOR_RED);
    init_pair(CP_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CP_GREEN, COLOR_GREEN, COLOR_BLACK);
#ifdef USEMSCP
    init_pair(CP_STATUS, COLOR_BLACK, COLOR_CYAN);
#else
    init_pair(CP_STATUS, COLOR_CYAN, COLOR_BLACK);
#endif
    init_pair(CP_DIALOG, COLOR_WHITE, COLOR_BLUE);
    raw();
    nonl();
    noecho();
    keypad(stdscr, 1);
    timeout(0);
    curs_set(0);
    leaveok(stdscr, 1);
    self->status = newwin(1, 0, 0, 0);
    leaveok(self->status, 1);
    self->field = newwin(0, 0, 1, 0);
    leaveok(self->field, 1);
#ifdef USEMSCP
    wbkgd(self->status, COLOR_PAIR(CP_STATUS)|A_BLINK);
#else
    wbkgd(self->status, COLOR_PAIR(CP_STATUS)|A_REVERSE|A_BOLD);
#endif
    wbkgd(self->field, COLOR_PAIR(CP_WHITE));
    mvwaddstr(self->status, 0, 1, "cursed snake   <Q> quit  <SPACE> pause");
    mvwaddstr(self->status, 0, self->w - 25, "score:");
    wnoutrefresh(stdscr);
    wnoutrefresh(self->status);
    doupdate();
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

void
screen_clear(Screen *self)
{
    wclear(self->field);
    wrefresh(self->field);
}

void
screen_printScore(Screen *self, unsigned int score)
{
    mvwprintw(self->status, 0, self->w-18, "%08u", score);
    wrefresh(self->status);
}

void
screen_printTime(Screen *self, int minutes, int seconds)
{
    mvwprintw(self->status, 0, self->w-8, "[%02d:%02d]", minutes, seconds);
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
#ifdef USEMSCP
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
	case FFOOD:
	    mvwaddch(self->field, y, x, '#'|COLOR_PAIR(CP_GREEN));
	    break;
	case WALL:
	    mvwaddch(self->field, y, x, ' '|COLOR_PAIR(CP_RVRED));
	    break;
    }
}

void
screen_showDialog(Screen *self, const char *title, const char *fmt, ...)
{
    va_list ap;
    int w, h, y, x, i;
    size_t l;
    char *buf;
    char *lines[10];
    WINDOW *dlg;

    l = strlen(fmt)*2;
    buf = malloc(l);
    va_start(ap, fmt);
    vsnprintf(buf, l, fmt, ap);
    va_end(ap);

    lines[0] = strtok(buf, "\n");
    for (i = 1; i < 10; ++i)
    {
	lines[i] = strtok(0, "\n");
	if (!lines[i]) break;
    }
    h = i;

    w = (int)strlen(title);
    for (i = 0; i < h; ++i)
    {
	l = strlen(lines[i]);
	if (l > (size_t)w) w = (int)l;
    }

    h += 4;
    w += 4;
    y = (self->h - h) >> 1;
    x = (self->w - w) >> 1;

    dlg = subwin(self->field, h, w, y, x);
    leaveok(dlg, 1);
    werase(dlg);
    wbkgd(dlg, COLOR_PAIR(CP_DIALOG));
    box(dlg, 0, 0);

    mvwaddstr(dlg, 1, 2 + (w-4-(int)strlen(title))/2, title);
    for (i = 0; i < h-4; ++i)
    {
	mvwaddstr(dlg, i+3, 2, lines[i]);
    }

    free(buf);
    wrefresh(dlg);
    delwin(dlg);
}

void
screen_refresh(Screen *self)
{
    wrefresh(self->field);
}
