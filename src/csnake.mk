P:= src
T:= csnake

csnake_SOURCES:= csnake.c board.c screen.c
csnake_posix_LDFLAGS:= -lcurses

$(eval $(BINRULES))

