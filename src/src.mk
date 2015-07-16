P:= src$(PSEP)
T:= csnake

csnake_DEPS:= src$(PSEP)src.mk
csnake_SOURCES:= $(P)csnake.c $(P)board.c

$(eval $(BINRULES))

