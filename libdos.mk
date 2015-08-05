LDBP:= externals/clang-libdos
LDP:= $(LDBP)/src/libdos

BINARY:= bin/csnake.com

SOURCES:= src/board.c src/csnake.c src/food.c src/game.c src/screen.c \
	src/snake.c src/utils.c src/platform/dosreal/ticker.c

LIBDOS:= $(LDP)/core.c $(LDP)/stdlib.c $(LDP)/string.c $(LDP)/stdio.c \
	$(LDP)/time.c $(LDP)/conio.c $(LDP)/curses.c $(LDP)/rtctimer.c

CFLAGS:= -std=c99 -Oz -nostdlib -m16 -march=i386 -ffreestanding -I$(LDP) \
	-Wall -Wextra -pedantic -DUSEMSCP -DNOARGV -DDOSREAL \
	-fomit-frame-pointer

LDFLAGS:= -Wl,--nmagic,--script=$(LDBP)/com.ld

ALLSRC:= $(LIBDOS) $(SOURCES)

all: $(BINARY)

$(BINARY): $(ALLSRC) | bin $(LDBP)/com.ld
	clang -o$@ $(CFLAGS) $(LDFLAGS) $^

bin:
	mkdir bin

.PHONY: all $(BINARY)
