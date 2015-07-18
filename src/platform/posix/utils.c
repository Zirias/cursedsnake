#define _POSIX_C_SOURCE 199309L
#include <time.h>

void
sleepMs(int ms)
{
    struct timespec tv;
    tv.tv_sec = 0;
    tv.tv_nsec = 1000000 * ms;
    nanosleep(&tv, 0);
}

