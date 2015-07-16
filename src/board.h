#ifndef CSNAKE_BOARD_H
#define CSNAKE_BOARD_H

#define BOARD_ROWS 24
#define BOARD_COLS 80

typedef enum item
{
    EMPTY,
    HEAD,
    TAIL,
    FOOD,
    WALL
} Item;

typedef Item Board[BOARD_ROWS][BOARD_COLS];

extern Board board;

void clearBoard(void);

#endif

