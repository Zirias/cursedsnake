#include "game.h"

#include "screen.h"
#include "board.h"
#include "snake.h"
#include "ticker.h"
#include "food.h"

#include <curses.h>

/* accelerate every 2 minutes */
#define NEXT_SPEED_TICKS 12000

static Screen *screen = 0;
static Board *board = 0;
static Snake *snake = 0;
static unsigned int score;
static unsigned int scoreadd;
static int speed;
static int hspeed;
static int vspeed;
static int nextStep;
static int nextFood;
static int nextSpeed;
static int maxFood;
static int numFood;
static Pos bsize;

static void
foodCreated(Food *f)
{
    (void)(f);

    ++numFood;
}

static void
foodDestroyed(Food *f)
{
    (void)(f);

    --numFood;
}

static void
addFood(void)
{
    int x, y;

    if (numFood < maxFood)
    {
	do
	{
	    x = randomNum(0, bsize.x-1);
	    y = randomNum(0, bsize.y-1);
	} while (board_get(board, y, x) != EMPTY);
	board_set(board, y, x, FOOD);
    }
}

static void
newgame(void)
{
    int i;
    
    snake_destroy(snake);
    score = 0;
    scoreadd = 250;
    speed = 8;
    hspeed = 10;
    vspeed = 15;
    nextStep = 1;
    nextSpeed = NEXT_SPEED_TICKS;
    nextFood = randomNum(50,100);
    snake = snake_create(board, 2, 10);
    if (!snake)
    {
	board_destroy(board);
	screen_destroy(screen);
	exit(1);
    }

    for (i = 0; i < maxFood / 3; ++i) addFood();
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
    board_size(board, &bsize);
    maxFood = bsize.y * bsize.x / 60;
    numFood = 0;
    food_onCreate(&foodCreated);
    food_onDestroy(&foodDestroyed);
    newgame();
}

void
game_done(void)
{
    food_onCreate(0);
    food_onDestroy(0);
    snake_destroy(snake);
    board_destroy(board);
    screen_destroy(screen);
    ticker_done();
}

void
game_run(void)
{
    int key;
    Step step;

    ticker_start(10000);
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
	    ticker_start(10000);
	    goto cont;
	}
	switch (key)
	{
	    case KEY_LEFT:
		snake_setDir(snake, LEFT);
		speed = hspeed;
		break;

	    case KEY_DOWN:
		snake_setDir(snake, DOWN);
		speed = vspeed;
		break;

	    case KEY_UP:
		snake_setDir(snake, UP);
		speed = vspeed;
		break;

	    case KEY_RIGHT:
		snake_setDir(snake, RIGHT);
		speed = hspeed;
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
		ticker_start(10000);
		break;
	}

	food_tick();

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
		ticker_start(10000);
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
	    nextFood = randomNum(200, 500);
	    addFood();
	}

	if (!--nextSpeed)
	{
	    nextSpeed = NEXT_SPEED_TICKS;
	    if (vspeed > 4)
	    {
		if (speed == vspeed)
		{
		    vspeed -= 3;
		    speed -= 3;
		    hspeed -= 2;
		}
		else
		{
		    vspeed -= 3;
		    speed -= 2;
		    hspeed -= 2;
		}
	    }
	}
cont:;
    }
    ticker_stop();
}

