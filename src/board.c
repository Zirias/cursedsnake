#include "board.h"

Board board;

void clearBoard(void)
{ 
    int i;
    for (i=0; i<BOARD_ROWS*BOARD_COLS; ++i) *(board[0]+i) = EMPTY;
}

