#include "hiscore.h"
#include "screen.h"

struct hiscoreEntry
{
    unsigned int score;
    int min;
    int sec;
    int len;
    char name[40];
};

struct hiscore
{
    Screen *s;
    int h;
    int w;
    HiscoreEntry entries[10];
};

Hiscore *
hiscore_create(Screen *s, int h, int w)
{
    Hiscore *self = malloc(sizeof(Hiscore));
    self->s = s;
    self->h = h;
    self->w = w;
    return self;
}

void
hiscore_destroy(Hiscore *self)
{
    free(self);
}

void
hiscore_show(Hiscore *self)
{
}

void
hiscore_enter(Hiscore *self, unsigned int score, int min, int sec, int len)
{
}

