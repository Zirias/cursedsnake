#undef __STRICT_ANSI__
#include <time.h>
#include <dpmi.h>
#include <errno.h>
#include <curses.h>

static uclock_t nextTick;
static uclock_t tickTime;
static unsigned int ring;
static unsigned int blocking = 0;

enum wmethod {
    WM_NONE,
    WM_YIELD,
    WM_IDLE,
    WM_HLT
};

static enum wmethod method;

static int
haveDosidle(void)
{
    __dpmi_regs regs;

    regs.x.ax = 0x1680;
    __dpmi_int(0x28, &regs);
    return regs.h.al ? 0 : 1;
}

static void
dosidle(void)
{
    __dpmi_regs regs;

    /* the DOS Idle call is documented to return immediately if no other
     * program is ready to run, therefore do one HLT if we can */
    if (ring == 0) __asm__ volatile ("hlt");

    regs.x.ax = 0x1680;
    __dpmi_int(0x28, &regs);
    if (regs.h.al) errno = ENOSYS;
}

void
ticker_init(void)
{
    /* determine protection ring */
    __asm__ volatile ("mov %%cs, %0\n\t"
		      "and $3, %0" : "=r" (ring));

    errno = 0;
    __dpmi_yield();

    if (errno)
    {
	if (haveDosidle())
	{
	    if (ring)
	    {
		method = WM_NONE;
	    }
	    else
	    {
		method = WM_HLT;
	    }
	}
	else
	{
	    method = WM_IDLE;
	}
    }
    else
    {
	method = WM_YIELD;
    }
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
    switch (method)
    {
	case WM_NONE:
	    while (uclock() < nextTick);
	    break;

	case WM_IDLE:
	    while (uclock() < nextTick) dosidle();
	    break;

	case WM_YIELD:
	    while (uclock() < nextTick) __dpmi_yield();
	    break;

	case WM_HLT:
	    while (uclock() < nextTick) __asm__ volatile ("hlt");
	    break;
    }
    nextTick += tickTime;
}

/* PDcurses for DOS uses yield and dos idle call, ignoring the results --
 * we can probably do better here: */

int
dos_getch()
{
    int c = getch();
    if (!blocking) return c;

    switch (method)
    {
	case WM_NONE:
	    while (c == ERR) c = getch();
	    break;

	case WM_IDLE:
	    while (c == ERR)
	    {
		dosidle();
		c = getch();
	    }
	    break;

	case WM_YIELD:
	    while (c == ERR)
	    {
		__dpmi_yield();
		c = getch();
	    }
	    break;

	case WM_HLT:
	    while (c == ERR)
	    {
		__asm__ volatile ("hlt");
		c = getch();
	    }
	    break;
    }
    return c;
}

void
dos_timeout(int delay)
{
    if (delay < 0) blocking = 1;
    else blocking = 0;
}

