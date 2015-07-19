#ifndef CSNAKE_HISCORE_H
#define CSNAKE_HISCORE_H

#include "common.h"

struct hiscoreEntry;
typedef struct hiscoreEntry HiscoreEntry;

struct hiscore;
typedef struct hiscore Hiscore;

struct screen;

Hiscore *hiscore_create(struct screen *s, int h, int w);
void hiscore_destroy(Hiscore *self);

void hiscore_show(Hiscore *self);
void hiscore_enter(Hiscore *self, unsigned int score,
	int min, int sec, int len);

#endif
