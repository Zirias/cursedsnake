#undef __STRICT_ANSI__
#include <time.h>
#include <dpmi.h>
#include <errno.h>

static uclock_t nextTick;
static uclock_t tickTime;
static int haveYield;

void
ticker_init(void)
{
    errno = 0;
    __dpmi_yield();
    haveYield = errno ? 0 : 1;
}

void
ticker_done(void)
{
}

void
ticker_start(int msec)
{
    tickTime = msec * UCLOCKS_PER_SEC / 1000;
    nextTick = uclock() + tickTime;
}

void
ticker_stop()
{
}

void
ticker_wait(void)
{
    if (haveYield)
    {
	while (uclock() < nextTick) __dpmi_yield();
    }
    else
    {
	while (uclock() < nextTick) __asm__ volatile ("hlt");
    }
    nextTick += tickTime;
}

