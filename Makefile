include defs.mk

# default configuration
CC := gcc
DEBUG := 0
GCC32 := 0
USELTO := 1

# read local configuration
-include defaults.mk

CFLAGS += -std=c89 -Wall -Wextra -pedantic -Winit-self -Wshadow \
	  -Wbad-function-cast -Wwrite-strings -Wconversion

ifeq ($(PLATFORM),dos)
EXE := .exe
CFLAGS += -DDOS
else
CFLAGS += -fvisibility=hidden
endif

BINDIR := bin

VTAGS :=
V := 0

prefix := /usr/local
bindir := $(prefix)/bin
docbasedir := $(prefix)/share/doc
docdir := $(docbasedir)/cwo

INSTALL := install

ifneq ($(GCC32),0)
CC := gcc -m32
CFLAGS += -DGCC32BIT
VTAGS += [32bit]
endif

ifeq ($(DEBUG), 0)
VTAGS += [release]
CFLAGS += -g0 -O3
ifneq ($(USELTO),0)
CFLAGS += -flto
LDFLAGS += -flto
endif
else
CFLAGS += -Wformat=2 -DDEBUG -g3 -O0
VTAGS += [debug]
endif

CCDEP := $(CC) -MM

ifeq ($(V),1)
VCC :=
VDEP :=
VLD :=
VCCLD :=
VRES :=
VGEN :=
VGENT :=
VR :=
else
VCC = @echo $(EQT)   [CC]   $@$(EQT)
VDEP = @echo $(EQT)   [DEP]  $@$(EQT)
VLD = @echo $(EQT)   [LD]   $@$(EQT)
VCCLD = @echo $(EQT)   [CCLD] $@$(EQT)
VRES = @echo $(EQT)   [RES]  $@$(EQT)
VGEN = @echo $(EQT)   [GEN]  $@$(EQT)
VGENT = @echo $(EQT)   [GEN]  $@: $(VTAGS)$(EQT)
VR := @
endif

INCLUDES := -Iinclude

all: bins

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
conf.mk:
	$(VGENT)
	$(VR)echo $(EQT)C_CC := $(CC)$(EQT) >conf.mk
	$(VR)echo $(EQT)C_DEBUG := $(DEBUG)$(EQT) >>conf.mk
	$(VR)echo $(EQT)C_GCC32 := $(GCC32)$(EQT) >>conf.mk
	$(VR)echo $(EQT)C_USELTO := $(USELTO)$(EQT) >>conf.mk
	$(VR)echo $(EQT)C_CROSS_COMPILE := $(CROSS_COMPILE)$(EQT) >> conf.mk

-include conf.mk

ifneq ($(strip $(C_CC))_$(strip $(C_DEBUG))_$(strip $(C_GCC32))_$(strip $(C_USELTO))_$(strip $(C_CROSS_COMPILE)),$(strip $(CC))_$(strip $(DEBUG))_$(strip $(GCC32))_$(strip $(USELTO))_$(strip $(CROSS_COMPILE)))
.PHONY: conf.mk
endif
endif
endif

include mk$(PSEP)bin.mk
include src$(PSEP)src.mk

bins: $(BINARIES)

clean:
ifneq ($(strip $(SOURCES)),)
	$(RMF) $(SOURCES:.c=.o)
	$(RMF) $(SOURCES:.c=.d)
endif
ifneq ($(strip $(CLEAN)),)
	$(RMF) $(CLEAN)
endif

distclean: clean
	$(RMF) conf.mk
	$(RMFR) $(BINDIR) $(CMDQUIET)

strip: all
ifneq ($(strip $(BINARIES)),)
	$(CROSS_COMPILE)strip --strip-all $(BINARIES)
endif

ifeq ($(PLATFORM),posix)
install: strip
ifneq ($(strip $(BINARIES)),)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) $(BINARIES) $(DESTDIR)$(bindir)
endif
endif

$(BINDIR):
	$(VR)$(MDP) $(BINDIR) $(CMDQUIET)

bindir: $(BINDIR)

.PHONY: all bins bindir strip clean distclean install
.SUFFIXES:

# vim: noet:si:ts=8:sts=8:sw=8
