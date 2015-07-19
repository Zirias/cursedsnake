#include "game.h"

#include "screen.h"
#include "board.h"
#include "snake.h"
#include "ticker.h"

#include <curses.h>

static Screen *screen;
static Board *board;
static Snake *snake;
static unsigned int score;
static unsigned int scoreadd;
static int speed;
static int nextStep;
static int nextFood;

void
game_init(void)
{
    ticker_init();
    screen = screen_create();
    if (!screen) return;
    board = board_create(screen);
    if (!board)
    {
	screen_destroy(screen);
	return;
    }
    snake = snake_create(board, 2, 10);
    if (!snake)
    {
	board_destroy(board);
	screen_destroy(screen);
	return;
    }
    score = 0;
    scoreadd = 250;
    speed = 16;
    nextStep = 1;
    nextFood = randomNum(100,200);
    screen_printScore(screen, score);
}

void
game_done(void)
{
    snake_destroy(snake);
    board_destroy(board);
    screen_destroy(screen);
    ticker_done();
}

void
game_run(void)
{
    int key, x, y;
    Step step;
    Pos size;

    ticker_start(5000);
    board_size(board, &size);
    while (1)
    {
	ticker_wait();
	key = getch();
	if (key == 'q' || key == 'Q') break;
	switch (key)
	{
	    case KEY_LEFT:
		snake_setDir(snake, LEFT);
		break;

	    case KEY_DOWN:
		snake_setDir(snake, DOWN);
		break;

	    case KEY_UP:
		snake_setDir(snake, UP);
		break;

	    case KEY_RIGHT:
		snake_setDir(snake, RIGHT);
		break;
	}

	if (!--nextStep)
	{
	    step = snake_step(snake);
	    if (step == SST_HIT) break;
	    if (step == SST_FOOD)
	    {
		snake_grow(snake, randomNum(3,8));
		score += scoreadd;
		scoreadd = 250;
		screen_printScore(screen, score);
	    }
	    else
	    {
		if (scoreadd > 50) --scoreadd;
	    }
	    nextStep = speed;
	}

	if (!--nextFood)
	{
	    nextFood = randomNum(400, 1000);
	    do
	    {
		x = randomNum(0, size.x-1);
		y = randomNum(0, size.y-1);
	    } while (board_get(board, y, x) != EMPTY);
	    board_set(board, y, x, FOOD);
	}
    }
    ticker_stop();
}

