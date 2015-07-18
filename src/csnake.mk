P:= src
T:= csnake

csnake_SOURCES:= csnake.c utils.c game.c board.c snake.c screen.c
csnake_PLATFORMSOURCES:= utils.c
csnake_LDFLAGS:= -pthread
csnake_posix_LDFLAGS:= -lcurses
csnake_win32_LDFLAGS:= -lpdcurses

$(eval $(BINRULES))
