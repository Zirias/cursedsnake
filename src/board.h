#ifndef CSNAKE_BOARD_H
#define CSNAKE_BOARD_H

#include "common.h"

#define MIN_ROWS 24
#define MIN_COLS 80

struct screen;

struct board;
typedef struct board Board;

Board *board_create(struct screen *screen);
void board_destroy(Board *self);

void board_clear(Board *self);
void board_size(const Board *self, Pos *pos);
void board_set(Board *self, int y, int x, Item item);
Item board_get(const Board *self, int y, int x);

#endif

