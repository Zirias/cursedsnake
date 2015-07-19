#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

static LARGE_INTEGER interval;
static HANDLE timer = INVALID_HANDLE_VALUE;

void
ticker_init(void)
{
    timeBeginPeriod(1);
    timer = CreateWaitableTimer(0, 1, 0);
    if (!timer)
    {
	fputs("CreateWaitableTimer failed.\n", stderr);
	exit(1);
    }
}

void
ticker_done(void)
{
    CloseHandle(timer);
    timeEndPeriod(1);
}

void
ticker_start(unsigned int usec)
{
    interval.QuadPart = usec;
    interval.QuadPart *= -10L;
    if (!SetWaitableTimer(timer, &interval, 0, 0, 0, 0))
    {
	fputs("SetWaitableTimer failed.\n", stderr);
	exit(1);
    }
}

void
ticker_stop(void)
{
    CancelWaitableTimer(timer);
}

void
ticker_wait(void)
{
    WaitForSingleObject(timer, INFINITE);
    if (!SetWaitableTimer(timer, &interval, 0, 0, 0, 0))
    {
	fputs("SetWaitableTimer failed.\n", stderr);
	exit(1);
    }
}
