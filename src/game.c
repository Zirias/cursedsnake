#include "game.h"

#include "screen.h"
#include "board.h"
#include "snake.h"

#include <pthread.h>
#include <semaphore.h>
#include <curses.h>

struct game
{
    Screen *screen;
    Board *board;
    Snake *snake;
    pthread_t timer;
    sem_t tick;
    sem_t endtimer;
    unsigned int score;
    unsigned int scoreadd;
    int speed;
    int nextStep;
    int nextFood;
};

static void *
timerloop(void *data)
{
    Game *self = data;

    while (sem_trywait(&(self->endtimer))<0)
    {
	sleepMs(25);
	sem_post(&(self->tick));
    }
    return 0;
}

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
    self->scoreadd = 250;
    self->speed = 3;
    self->nextStep = 1;
    self->nextFood = randomNum(20,40);
    sem_init(&self->tick, 0, 1);
    sem_init(&self->endtimer, 0, 0);
    screen_printScore(self->screen, self->score);
    return self;
}

void
game_destroy(Game *self)
{
    if (!self) return;
    sem_destroy(&(self->endtimer));
    sem_destroy(&(self->tick));
    snake_destroy(self->snake);
    board_destroy(self->board);
    screen_destroy(self->screen);
    free(self);
}

void
game_run(Game *self)
{
    int key, x, y;
    Step step;
    Pos size;

    board_size(self->board, &size);
    pthread_create(&(self->timer), 0, &timerloop, self);
    while (sem_wait(&(self->tick))==0)
    {
	key = getch();
	if (key == 'q' || key == 'Q') break;
	switch (key)
	{
	    case KEY_LEFT:
		snake_setDir(self->snake, LEFT);
		break;

	    case KEY_DOWN:
		snake_setDir(self->snake, DOWN);
		break;

	    case KEY_UP:
		snake_setDir(self->snake, UP);
		break;

	    case KEY_RIGHT:
		snake_setDir(self->snake, RIGHT);
		break;
	}

	if (!--self->nextStep)
	{
	    step = snake_step(self->snake);
	    if (step == SST_HIT) break;
	    if (step == SST_FOOD)
	    {
		snake_grow(self->snake, randomNum(3,8));
		self->score += self->scoreadd;
		self->scoreadd = 250;
		screen_printScore(self->screen, self->score);
	    }
	    else
	    {
		if (self->scoreadd > 50) --self->scoreadd;
	    }
	    self->nextStep = self->speed;
	}

	if (!--self->nextFood)
	{
	    self->nextFood = randomNum(80, 200);
	    do
	    {
		x = randomNum(0, size.x-1);
		y = randomNum(0, size.y-1);
	    } while (board_get(self->board, y, x) != EMPTY);
	    board_set(self->board, y, x, FOOD);
	}
    }
    sem_post(&(self->endtimer));
    pthread_join(self->timer, 0);
}

