#ifndef CSNAKE_SNAKE_H
#define CSNAKE_SNAKE_H

#include "common.h"

struct board;

typedef enum
{
    SST_NORMAL,
    SST_FOOD,
    SST_HIT,
    SST_MAXLEN
} Step;

struct snake;
typedef struct snake Snake;

Snake *snake_create(struct board *board, int y, int x);
void snake_destroy(Snake *self);

void snake_setDir(Snake *self, Dir dir);
void snake_grow(Snake *self, int grow);
int snake_len(const Snake *self);
Step snake_step(Snake *self);

#endif

