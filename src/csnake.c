#include "board.h"

int main(void)
{
    Board *board = board_create();
    board_destroy(board);
    return 0;
}

