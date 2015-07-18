P:= src
T:= csnake

csnake_SOURCES:= csnake.c utils.c game.c board.c snake.c screen.c
csnake_posix_LDFLAGS:= -lcurses

$(eval $(BINRULES))

