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
    WALL
} Item;

typedef enum
{
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
void insertString(char *dest, int pos, const char *s);
int randomNum(int min, int max);
void sleepMs(int ms);

#endif
