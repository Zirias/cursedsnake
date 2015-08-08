all: doscom/csnake.com

LIBDOSPATH:= externals/clang-libdos

include $(LIBDOSPATH)/mk/doscom.mk

csnake_TARGET:= csnake.com
csnake_SRCDIR:= src
csnake_TGTDIR:= doscom
csnake_ASMDIR:= doscom/asm
csnake_OBJDIR:= doscom/obj
csnake_MODULES:= board csnake food game screen snake utils \
	platform/dosreal/ticker
csnake_CFLAGS:= -DUSEMSCP

T:=csnake
$(eval $(DOSCOMRULES))

clean:
	rm -f $(CLEAN)

distclean: clean
	rm -fr doscom

.PHONY: all clean distclean
