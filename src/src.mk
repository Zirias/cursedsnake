P:= src$(PSEP)

csnake_OBJS := $(P)csnake.o $(P)board.o
csnake_SOURCES := $(csnake_OBJS:.o=.c)

SOURCES += $(csnake_SOURCES)

T := csnake
$(eval $(BINRULES))

