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

![csnake screenshot 2015-07-19](/../screenshots/csnake_0.1_cmd.png?raw=true
"csnake running on my windows dev box")

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

## Prerelease win32 binary

So you don't have a GNU toolchain on your windows box but would like to try
this out just for fun? well, here's the
[latest test build](/../files/current/csnake.exe?raw=true) (i686 win32).

