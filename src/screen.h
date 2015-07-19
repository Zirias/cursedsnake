#ifndef CSNAKE_SCREEN_H
#define CSNAKE_SCREEN_H

#include "common.h"

struct screen;
typedef struct screen Screen;

Screen *screen_create(void);
void screen_destroy(Screen *self);

int screen_width(const Screen *self);
int screen_height(const Screen *self);
void screen_clear(Screen *self);
void screen_printScore(Screen *self, unsigned int score);
void screen_putItem(Screen *self, int y, int x, Item item, int refresh);
void screen_showDialog(Screen *self, const char *title, const char *fmt, ...);
void screen_refresh(Screen *self);

#endif
