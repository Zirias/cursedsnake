P:= src
T:= csnake

csnake_SOURCES:= csnake.c utils.c game.c board.c snake.c food.c screen.c
csnake_PLATFORMSOURCES:= ticker.c
csnake_LDFLAGS:= -lm
csnake_posix_LDFLAGS:= -lcurses
csnake_win32_LDFLAGS:= -static-libgcc -Wl,-Bstatic -lpdcurses \
	-Wl,-Bdynamic -lwinmm
csnake_win32_RES:= res$(PSEP)csnake.rc

$(eval $(BINRULES))

