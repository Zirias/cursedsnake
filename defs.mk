ifeq ($(OS),Windows_NT)
PLATFORM := win32
POSIXSHELL :=
ifdef LANG
POSIXSHELL := 1
endif
ifdef BASH
POSIXSHELL := 1
endif
else
PLATFORM := posix
POSIXSHELL := 1
endif

ifdef POSIXSHELL

SYSNAME := $(shell uname 2>/dev/null)
EXE :=
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

else

SYSNAME :=
EXE := .exe
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

endif

# vim: noet:si:ts=8:sts=8:sw=8
