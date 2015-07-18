P:= src
T:= csnake

csnake_SOURCES:= csnake.c board.c
csnake_PLATFORMSOURCES:= screen.c
csnake_posix_LDFLAGS:= -lcurses

$(eval $(BINRULES))

