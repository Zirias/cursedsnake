#include "game.h"

#include "screen.h"
#include "board.h"
#include "snake.h"

struct game
{
    Screen *screen;
    Board *board;
    Snake *snake;
    unsigned int score;
    int speed;
    int nextFood;
};

Game *
game_create(void)
{
    Game *self = malloc(sizeof(Game));
    self->screen = screen_create();
    if (!self->screen)
    {
	free(self);
	return 0;
    }
    self->board = board_create(self->screen);
    if (!self->board)
    {
	screen_destroy(self->screen);
	free(self);
	return 0;
    }
    self->snake = snake_create(self->board, 2, 10);
    if (!self->snake)
    {
	board_destroy(self->board);
	screen_destroy(self->screen);
	free(self);
	return 0;
    }
    self->score = 0;
    self->speed = 20;
    self->nextFood = randomNum(20,40);
    screen_printScore(self->screen, self->score);
    return self;
}

void
game_destroy(Game *self)
{
    if (!self) return;
    snake_destroy(self->snake);
    board_destroy(self->board);
    screen_destroy(self->screen);
    free(self);
}

void
game_run(Game *self)
{
    screen_putItem(self->screen, 10, 10, HEAD);
    screen_putItem(self->screen, 10, 11, TAIL);
    screen_putItem(self->screen, 10, 12, TAIL);
    screen_putItem(self->screen, 10, 13, TAIL);
    screen_putItem(self->screen, 14, 40, FOOD);
    screen_putItem(self->screen, 20, 10, WALL);
    screen_getch(self->screen);
}

