#ifndef CSNAKE_SCREEN_H
#define CSNAKE_SCREEN_H

struct screen;
typedef struct screen Screen;

Screen *screen_create(void);
void screen_destroy(Screen *self);

#endif
