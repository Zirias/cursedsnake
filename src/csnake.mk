P:= src
T:= csnake

csnake_SOURCES:= csnake.c board.c
csnake_PLATFORMSOURCES := screen.c

$(eval $(BINRULES))

