LDBP:= externals/clang-libdos
LDP:= $(LDBP)/src/libdos

BINARY:= bin/csnake.com

SOURCES:= src/board.c src/csnake.c src/food.c src/game.c src/screen.c \
	src/snake.c src/utils.c src/platform/dosreal/ticker.c

LIBDOS:= $(LDP)/core.c $(LDP)/stdlib.c $(LDP)/string.c $(LDP)/stdio.c \
	$(LDP)/time.c $(LDP)/conio.c $(LDP)/curses.c $(LDP)/rtctimer.c

CFLAGS:= -std=c99 -Oz -nostdlib -m16 -march=i386 -mregparm=3 \
	-ffreestanding -ffunction-sections -fdata-sections -I$(LDP) \
	-Wall -Wextra -pedantic -DUSEMSCP -DNOARGV

LDFLAGS:= -Wl,--omagic,--script=$(LDBP)/com.ld,--gc-sections

ALLSRC:= $(LIBDOS) $(SOURCES)

all: $(BINARY)

$(BINARY): $(ALLSRC:.c=.o) | bin $(LDBP)/com.ld
	clang -o$@ $(CFLAGS) $(LDFLAGS) $^

%.o: %.c


%.o: %.s
	echo \\t.code16 | cat - $< | as --32 -march=i586 -o$@

%.s: %.c
	clang -S -o$@ $(CFLAGS) $<

bin:
	mkdir bin

clean:
	rm -f $(ALLSRC:.c=.o)
	rm -f $(ALLSRC:.c=.s)

.PHONY: all $(BINARY)
