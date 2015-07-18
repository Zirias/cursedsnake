#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void *
xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
	perror("malloc");
	exit(1);
    }
    return ptr;
}

void
insertString(char *dest, int pos, const char *s)
{
    dest += pos;
    while (*s) *dest++ = *s++;
}

int
randomNum(int min, int max)
{
    static int seeded = 0;
    if (!seeded)
    {
	seeded = 1;
	srand((unsigned int)time(0));
    }
    return (rand() % (max-min+1)) + min;
}
