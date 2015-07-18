#ifndef CSNAKE_BOARD_H
#define CSNAKE_BOARD_H

#define MIN_ROWS 23
#define MIN_COLS 80

typedef enum item
{
    EMPTY,
    HEAD,
    TAIL,
    FOOD,
    WALL
} Item;

struct board;
typedef struct board Board;

Board *board_create(void);
void board_destroy(Board *self);

void board_clear(Board *self);

#endif

