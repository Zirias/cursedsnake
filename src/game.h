#ifndef CSNAKE_GAME_H
#define CSNAKE_GAME_H

#include "common.h"

struct game;
typedef struct game Game;

Game *game_create(void);
void game_destroy(Game *self);

void game_run(Game *self);

#endif
