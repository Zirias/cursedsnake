#include <rtctimer.h>

static unsigned int usec = 0;

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
    usec = 1000 * msec;
    rtctset(usec);
}

void
ticker_stop(void)
{
    rtctstop();
    usec = 0;
}

void
ticker_wait(void)
{
    rtctwait();
    if (usec) rtctset(usec);
}

