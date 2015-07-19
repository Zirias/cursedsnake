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
randomWalls(void)
{
    int i;

    int num = bsize.y * bsize.x / 230;
    int mvlen = bsize.y / 4;
    int dvlen = bsize.y / 6;
    int mhlen = bsize.x / 4;
    int dhlen = bsize.x / 6;

    for (i=0; i < num; ++i)
    {
	int y = randomNum(0, bsize.y-1);
	int x = randomNum(0, bsize.x-1);
	if (randomNum(0,1))
	{
	    int d, l, p;
	    if (x <= bsize.x/2) d=1;
	    else d=-1;
	    l = randomNum(mhlen-dhlen, mhlen+dhlen);
	    for (p = 0; p < l; ++p)
	    {
		board_set(board, y, x, WALL);
		x += d;
	    }
	}
	else
	{
	    int d, l, p;
	    if (y <= bsize.y/2) d=1;
	    else d=-1;
	    l = randomNum(mvlen-dvlen, mvlen+dvlen);
	    for (p = 0; p < l; ++p)
	    {
		board_set(board, y, x, WALL);
		y += d;
	    }
	}
    }
}

static void
createSnake(void)
{
    int y, x, i;

    int isclear = 0;
    while (!isclear)
    {
	y = randomNum(0, bsize.y-1);
	x = randomNum(0, bsize.x-13);

	isclear = 1;
	for (i=0; i<12; ++i)
	{
	    if (board_get(board, y, x+i) != EMPTY)
	    {
		isclear = 0;
		break;
	    }
	}
    }

    snake = snake_create(board, y, x);
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
    randomWalls();
    createSnake();
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
    maxFood = bsize.y * bsize.x / 50;
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
    int key, nfm;
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
		screen_showDialog(screen, "- P A U S E D -",
			"Cursed Snake -- (C) 07/2015 Felix Palmen"
			" <felix@palmen-it.de>.\n"
			" \n"
			"                 - press <SPACE> to resume -");
		do
		{
		    key = getch();
		} while (key != ' ');
		board_redraw(board);
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
			    "Press <Q> to quit, or <Enter> to restart.",
			    score);
		}
		else
		{
		    screen_showDialog(screen, "CONGRATULATIONS!",
			    "Your snake really got huge. Now it's time to\n"
			    "take some rest.\n"
			    "Your final score is: %lu.\n"
			    " \n"
			    "Press <Q> to quit, or <Enter> to play again.",
			    score);
		}
		timeout(-1);
		do
		{
		    key = getch();
		} while (key != 'q' && key != 'Q' && key != '\n');
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
	    nfm = (int)(4000.0 / maxFood * numFood / maxFood);
	    if (nfm < 1) nfm = 1;
	    nextFood = randomNum(nfm, nfm+200);
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

