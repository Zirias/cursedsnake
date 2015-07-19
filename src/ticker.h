#ifndef CSNAKE_TICKER_H
#define CSNAKE_TICKER_H

void ticker_init(void);
void ticker_done(void);

void ticker_start(unsigned int usec);
void ticker_stop(void);
void ticker_wait(void);

#endif
