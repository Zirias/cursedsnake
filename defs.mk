ifeq ($(OS),Windows_NT)

CMDSEP := &
PSEP := \\
CPF := copy /y
RMF := del /f /q
RMFR := -rd /s /q
MDP := -md
XIF := if exist
XTHEN := (
XFI := )
CATIN := copy /b
CATADD := +
CATOUT :=
EQT :=
CMDQUIET := >nul 2>nul & verify >nul

CFLAGS += -DWIN32
platform_LDFLAGS :=
platform_LIBS :=
lib_CFLAGS :=
lib_LIBS :=

PLATFORM := win32

else

SYSNAME := $(shell uname)

CMDSEP := ;
PSEP := /
CPF := cp -f
RMF := rm -f
RMFR := rm -fr
MDP := mkdir -p
XIF := if [ -x
XTHEN := ]; then
XFI := ; fi
CATIN := cat
CATADD := 
CATOUT := >
EQT := "
#" make vim syntax highlight happy
CMDQUIET := >/dev/null 2>&1

platform_LDFLAGS :=
platform_LIBS :=
lib_CFLAGS := -fPIC
lib_LIBS :=

PLATFORM := posix

endif

# vim: noet:si:ts=8:sts=8:sw=8
