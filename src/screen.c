#define _POSIX_C_SOURCE 200112L
#include "screen.h"

#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

enum cpair
{
    CP_NONE,
    CP_WHITE,
    CP_RED,
    CP_YELLOW,
    CP_GREEN,
    CP_STATUS,
    CP_DIALOG
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
    init_pair(CP_DIALOG, COLOR_WHITE, COLOR_BLUE);
    bkgd(COLOR_PAIR(CP_WHITE));
    raw();
    noecho();
    keypad(stdscr, 1);
    timeout(0);
    curs_set(0);
    self->status = newwin(1, 0, 0, 0);
    self->field = newwin(0, 0, 1, 0);
    wbkgd(self->status, COLOR_PAIR(CP_STATUS)|A_BLINK);
    tmp = malloc((size_t)self->w+1);
#ifdef WIN32
    memset(tmp, ' ', (size_t)self->w);
#else
    memset(tmp, ' ' + 0x80, (size_t)self->w);
#endif
    tmp[self->w] = 0;
    insertString(tmp, 1, "cursed snake   <Q> quit  <SPACE> pause");
    insertString(tmp, self->w - 16, "score:");
    waddstr(self->status, tmp);
    free(tmp);
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
    mvwprintw(self->status, 0, self->w-9, "%8u", score);
    wrefresh(self->status);
}

void
screen_pauseOn(Screen *self)
{
    mvwaddstr(self->status, 0, 16, "-- PAUSED --  <SPACE> continue");
    wrefresh(self->status);
}

void
screen_pauseOff(Screen *self)
{
    mvwaddstr(self->status, 0, 16, "<Q> quit  <SPACE> pause       ");
    wrefresh(self->status);
}

void
screen_putItem(Screen *self, int y, int x, Item item, int refresh)
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
	case FFOOD:
	    mvwaddch(self->field, y, x, '#'|COLOR_PAIR(CP_GREEN));
	    break;
	case WALL:
#ifdef WIN32
	    mvwaddch(self->field, y, x,
		    ' '|COLOR_PAIR(CP_RED)|A_REVERSE);
#else
	    mvwaddch(self->field, y, x,
		    (' '+0x80)|COLOR_PAIR(CP_RED)|A_REVERSE);
#endif
	    break;
    }
    if (refresh) wrefresh(self->field);
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

    dlg = newwin(h, w, y, x);
    wbkgd(dlg, COLOR_PAIR(CP_DIALOG));
    box(dlg, 0, 0);

    mvwaddstr(dlg, 1, 2 + (w-4-(int)strlen(title))/2, title);
    for (i = 0; i < h-3; ++i)
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
