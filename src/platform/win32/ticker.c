#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

static long long interval;
static HANDLE timer = INVALID_HANDLE_VALUE;

static VOID CALLBACK
restart(LPVOID arg, DWORD timeLow, DWORD timeHigh)
{
    LARGE_INTEGER due;
    due.QuadPart = -interval;
    if (!SetWaitableTimer(timer, &due, 0, &restart, 0, 0))
    {
	fputs("SetWaitableTimer failed.\n", stderr);
	exit(1);
    }
}

void
ticker_init(void)
{
    timer = CreateWaitableTimer(0, 1, 0);
    if (!timerHandle)
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
ticker_start(unsigned int usec)
{
    interval = usec * 10LL;
    LARGE_INTEGER due;
    due.QuadPart = -interval;
    if (!SetWaitableTimer(timer, &due, 0, &restart, 0, 0))
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
}
