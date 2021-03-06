#define _POSIX_SOURCE
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static sigset_t alarm;
static struct itimerval itv;
static struct sigaction sasave;

/* unused signal handler, SIG_DFL would terminate, SIG_IGN causes
 * sigwait() to wait forever -- therefore this dummy */
static void
sighdl(int signum)
{
    (void)(signum);
}

void
ticker_init(void)
{
    struct sigaction sa;

    sigemptyset(&alarm);
    sigaddset(&alarm, SIGALRM);
    sa.sa_handler = &sighdl;
    sigemptyset(&(sa.sa_mask));
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, &sasave);
}

void
ticker_done(void)
{
    sigaction(SIGALRM, &sasave, 0);
}

void
ticker_start(int msec)
{
    itv.it_interval.tv_sec = msec / 1000;
    itv.it_interval.tv_usec = (1000*msec) % 1000000;
    itv.it_value.tv_sec = msec / 1000;
    itv.it_value.tv_usec = (1000*msec) % 1000000;
    if (setitimer(ITIMER_REAL, &itv, 0) < 0)
    {
	perror("setitimer");
	exit(1);
    }
}

void
ticker_stop(void)
{
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &itv, 0) < 0)
    {
	perror("setitimer");
	exit(1);
    }
}

void
ticker_wait(void)
{
    int sig;
    sigwait(&alarm, &sig);
}
