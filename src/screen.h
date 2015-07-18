#ifndef CSNAKE_SCREEN_H
#define CSNAKE_SCREEN_H

struct screen;
typedef struct screen Screen;

Screen *screen_create(void);
void screen_destroy(Screen *self);

int screen_width(const Screen *self);
int screen_height(const Screen *self);
int screen_getch(Screen *self);

#endif
