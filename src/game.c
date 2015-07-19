#include "game.h"

#include "screen.h"
#include "board.h"
#include "snake.h"
#include "ticker.h"

#include <curses.h>

static Screen *screen = 0;
static Board *board = 0;
static Snake *snake = 0;
static unsigned int score;
static unsigned int scoreadd;
static int speed;
static int nextStep;
static int nextFood;

static void
newgame(void)
{
    snake_destroy(snake);
    score = 0;
    scoreadd = 250;
    speed = 16;
    nextStep = 1;
    nextFood = randomNum(100,200);
    snake = snake_create(board, 2, 10);
    if (!snake)
    {
	board_destroy(board);
	screen_destroy(screen);
	exit(1);
    }
    screen_printScore(screen, score);
}

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
    newgame();
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
	if (key == 'q' || key == 'Q')
	{
	    ticker_stop();
	    screen_showDialog(screen, "QUIT",
		    "Are you sure you want to abandon your game?\n"
		    " \n"
		    "Hit <Q> again to quit, any other key to resume.");
	    timeout(-1);
	    key = getch();
	    if (key == 'q' || key == 'Q') break;
	    board_redraw(board);
	    timeout(0);
	    ticker_start(5000);
	    goto cont;
	}
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

	    case ' ':
		ticker_stop();
		timeout(-1);
		screen_pauseOn(screen);
		do
		{
		    key = getch();
		} while (key != ' ');
		screen_pauseOff(screen);
		timeout(0);
		ticker_start(5000);
		break;
	}

	if (!--nextStep)
	{
	    step = snake_step(snake);
	    if (step == SST_HIT || step == SST_MAXLEN)
	    {
		ticker_stop();
		flash();
		beep();
		if (step == SST_HIT)
		{
		    screen_showDialog(screen, "OUCH!! -- Game over",
			    "Your poor snake hit something and has "
			    "a headache.\n"
			    "Your final score is %lu.\n"
			    " \n"
			    "Press <Q> to quit, any other key to restart.",
			    score);
		}
		else
		{
		    screen_showDialog(screen, "CONGRATULATIONS!",
			    "Your snake really got huge. Now it's time to\n"
			    "take some rest.\n"
			    "Your final score is: %lu.\n"
			    " \n"
			    "Press <Q> to quit, any other key to play again.",
			    score);
		}
		timeout(-1);
		key = getch();
		if (key == 'q' || key == 'Q') break;
		board_clear(board);
		screen_clear(screen);
		newgame();
		timeout(0);
		ticker_start(5000);
		goto cont;
	    }
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
cont:;
    }
    ticker_stop();
}

