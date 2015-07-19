#ifndef CSNAKE_FOOD_H
#define CSNAKE_FOOD_H

#include "common.h"

struct board;
struct screen;

struct food;
typedef struct food Food;

typedef void (*food_callback)(Food *f);

Food *food_create(struct board *b, struct screen *s, int y, int x);
void food_destroy(Food *self);

void food_draw(Food *self, int refresh);

void food_tick(void);
void food_onCreate(food_callback cb);
void food_onDestroy(food_callback cb);

#endif
