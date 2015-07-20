#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

static HANDLE timer = INVALID_HANDLE_VALUE;

void
ticker_init(void)
{
    timer = CreateWaitableTimer(0, 0, 0);
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
}

void
ticker_start(int msec)
{
    LARGE_INTEGER due;

    timeBeginPeriod(1);
    due.QuadPart = -10000L * msec;
    if (!SetWaitableTimer(timer, &due, msec, 0, 0, 0))
    {
	fputs("SetWaitableTimer failed.\n", stderr);
	exit(1);
    }
}

void
ticker_stop(void)
{
    CancelWaitableTimer(timer);
    timeEndPeriod(1);
}

void
ticker_wait(void)
{
    WaitForSingleObject(timer, INFINITE);
}
