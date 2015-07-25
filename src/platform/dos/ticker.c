#undef __STRICT_ANSI__
#include <time.h>

uclock_t tick;
uclock_t nextTick;
uclock_t tickTime;

void
ticker_init(void)
{
}

void
ticker_done(void)
{
}

void
ticker_start(int msec)
{
    tickTime = msec * UCLOCKS_PER_SEC / 1000;
    tick = uclock();
    nextTick = tick + tickTime;
}

void
ticker_stop()
{
}

void
ticker_wait(void)
{
    while ((tick = uclock()) < nextTick);
    nextTick = tick + tickTime;
}

