#ifndef CSNAKE_COMMON_H
#define CSNAKE_COMMON_H

#include <stdlib.h>

/* macros */

#define STR_X(x) #x
#define STR(x) STR_X(x)


/* types */

typedef enum
{
    EMPTY,
    HEAD,
    TAIL,
    FOOD,
    FFOOD,
    WALL
} Item;

typedef enum
{
    NONE,
    LEFT,
    DOWN,
    RIGHT,
    UP
} Dir;

typedef struct
{
    int y;
    int x;
} Pos;


/* utils */

#define malloc xmalloc
void *xmalloc(size_t size);
int randomNum(int min, int max);
void sleepMs(int ms);

#endif
