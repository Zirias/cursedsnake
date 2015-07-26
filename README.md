# cursed snake

![csnake in urxvt](/../screenshots/csnake_0.1_urxvt.png?raw=true
"csnake running in urxvt")

A tiny snake game using curses.

Do you need a manual?

This is still work in progress. So far, it features a scoring system, clock
and randomly placed obstacles. Of course it handles any terminal size, as long
as it's not *smaller* than 80x25. Resizing the terminal while running is *not*
handled ... it's a silly idea trying to resize a board game in action. Just
don't do it :). Windows users: If you want bigger boards, create a link to the
executable. In the link properties, you can configure the window size (among
other things).

Colors are obviously a matter of being lucky with curses .. this screenshot
above looks ok, so how do you like this version of *good old* xterm?

![csnake in xterm](/../screenshots/csnake_0.1_xterm.png?raw=true
"csnake running in xterm")

Thanks to *PDcurses* (and a tiny bit of platform specific tweaking), it even
works fine on windows:

![csnake on windows](/../screenshots/csnake_0.1_cmd.png?raw=true
"csnake running on my windows dev box")

... as well as on DOS (this looks crazy on a 1920x1200 display):

![csnake on dos](/../screenshots/csnake_0.1_dos.png?raw=true
"csnake running in dosbox")

... here we can have this old *MS DOS smiley* as the snake head -- great! :)

## Planned

working on a highscore list. That's the only thing still missing from my idea
of a first release.

## Building from source

Just type `make`. You will end up with a binary in the `bin` subdir.

(If not, you're missing development files for `curses` -- on Linux, this is
normally `ncurses` -- or they are in a nonstandard location. Set your `CFLAGS`
and `LDFLAGS` environment variable so that `curses` headers and libs are
found.)

some more options to `make`:

 - `USELTO=0` (defaults to `1`): Enable or disable *link time optimizations*
 - `GCC32=1` (defaults to `0`): On x86_64, build an x86 binary
 - `DEBUG=1` (defaults to `0`): Build with debugging info (`-g3`),
                                disable optimizations (`-O0`)

### Building with llvm / clang

Try `LDFLAGS=-fuse-ld=gold make CC=clang`.

### Cross-compiling for DOS

You need a [`DJGPP`
toolchain](https://github.com/andrewwutw/build-djgpp/releases). Install it,
then compile the `PDcurses` lib with it. `pdcurses.a` goes in the `lib`
directory of your toolchain (e.g. `/usr/local/i586-pc-msdosdjgpp/lib`), named
`libpdcurses.a`. Place the header `curses.h` in the `include` directory of
your toolchain (e.g. `/usr/local/i586-pc-msdosdjgpp/include`). Then, the
following commands will do:

    make CC=i586-pc-msdosdjgpp-gcc PLATFORM=dos
    i586-pc-msdosdjgpp-strip --strip-all bin/csnake.exe

To make it run correctly under plain DOS, you need a DPMI server (Windows 9x
already provides one). DJGPP links the .exe with a stub that tries to run
CWSDPMI.EXE if no DPMI services are detected. This will not work because
cursed snake uses the privileged `HLT` instruction to save power when running
under plain DOS and CWSDPMI puts the code in `ring 3`. First, you have to
download the CWSDPMI binary distribution
[from here](http://homer.rice.edu/~sandmann/cwsdpmi/). Then there are two
possibilities to have cursed snake run in `ring 0`, so it can use `HLT`:

1. Modify the stub linked by DJGPP to load CWSDPR0.EXE instead:

   `/[...]/i586-pc-msdosdjgpp/bin/stubedit bin/csnake.exe dpmi=CWSDPR0.EXE`

   With this, the CWSDPR0.EXE must be in the same directory as csnake.exe to
   run it.

2. Replace the stub with CWSDSTR0.EXE:

    `/[...]/i586-pc-msdosdjgpp/bin/exe2coff bin/csnake.exe`

    `cat /path/to/CWSDSTR0.EXE bin/csnake >bin/csnake.exe`

   The resulting binary will run on its own even in plain DOS, because it has
   the CWS DPMI server embedded.

## Prerelease binaries

So you don't have a GNU toolchain on your machine but would like to try this
out just for fun? Here are some builds (redone from time to time):

 - [Windows 32bit](/../files/current/win32/csnake.exe?raw=true) (.exe)
 - [Linux 32bit](/../files/current/linux32/csnake?raw=true) (ELF binary)
 - [Linux 64bit](/../files/current/linux64/csnake?raw=true) (ELF binary)
 - [MS-DOS / Win 9x](/../files/current/dos/csnake.exe?raw=true) (.exe)

