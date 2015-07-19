P:= src
T:= csnake

csnake_SOURCES:= csnake.c utils.c game.c board.c snake.c screen.c
csnake_PLATFORMSOURCES:= utils.c
csnake_posix_LDFLAGS:= -pthread -lcurses
csnake_win32_LDFLAGS:= -static-libgcc -Wl,-Bstatic -lpthread -lpdcurses

$(eval $(BINRULES))

