<img width="806" height="425" alt="vik1_remake_snow" src="https://github.com/user-attachments/assets/cb32f0a1-900e-4eac-8496-c1d15318f3c0" />
# vik1 remake

A readable and extended version of **vik1**, Daniel Vik's car racing game that won
*Best X11 game* in the 2004 International Obfuscated C Code Contest (IOCCC).

The original fits a complete 3D racing game into about 60 lines of deliberately
unreadable C: hills, curves, mountains, a sun, night and winter modes, a computer
opponent, collisions and lap timing. This project started as an attempt to
understand how a game like that can be made at all, and grew into a cleaned-up
version with a few additions.
Compiles to 101k with uclibc static linked and 19k with libc 64bit.


## What's new compared to the original

- **Readable code.** The macros, nested ternaries and recursive calls to `main()`
  are rewritten as ordinary functions and loops, with comments.
- **Scalable window and full screen.** The game still draws at 400×200, but the
  picture is scaled to fill any window size, with black bars when the shape
  isn't 2:1. Press **F** or **F11** to toggle full screen.
- **End of race.** A race is two laps, followed by a finish screen showing total
  time, fastest lap and top speed. Press **R** for a new race.
- **Works on 64-bit systems.** The original used a `struct timeval` laid over an
  `int` array for its frame timer, which only works where `long` is 32 bits.
- **Other fixes.** Options can be given in any order, ESC quits, the window close
  button works, keys with large keysym values (e.g. media keys) no longer crash
  the game, and held keys no longer get stuck.

The original gameplay is kept: same tracks, same speed, same handling. The only
deliberate control change is that **Up** accelerates instead of the space bar.

## Building

You need a C compiler and the X11 development files
(`libx11-dev` on Debian/Ubuntu, `libX11-devel` on Fedora).

```sh
make
```

or directly:

```sh
gcc -O2 -Wall -o car_game car_game.c -lX11
```

The game runs on Linux and other systems with an X server. On Wayland desktops it
runs through XWayland. The scaling assumes a 24/32-bit colour display, which is
the normal case today.

## Playing

```sh
./car_game [options]
```

Options are letters written together as one word, for example `./car_game ad2`:

| Option | Effect |
|--------|--------|
| `a`    | Winter: snow and a slippery road |
| `d`    | Night |
| `1` `2` `3` | Track selection (without a digit you get the same track as `3`) |
| `f`    | Start in full screen |

| Key | Action |
|-----|--------|
| Up | Accelerate (release to slow down) |
| Left / Right | Steer |
| F or F11 | Toggle full screen |
| R | New race (on the finish screen) |
| ESC | Quit |

Stay on the road: driving onto the grass slows you down, and hitting the
opponent's car drops your speed sharply.

## How the original works

A few things that became clear while untangling the code:

- **One function draws everything.** Every recursive call `main(v)` sets a colour
  from `v`, draws two mirrored horizontal line segments on one screen row, and
  returns the current time. Sky, mountains, road, cars and snow are all built
  from these line pairs. In this version that function is `draw_mode_setcolors()`.
- **The track is generated at start-up.** The first 10660 iterations of the
  event loop fill an array with the curves and hills of the track. The second
  half of the track mirrors the first, so the road always closes into a loop.
- **The same array holds everything.** `C[]` stores the track curves, the track
  heights and, from offset 65793, the state of every key.
- **Constants do double duty.** 65793 is `0x010101`, so it is both the key-state
  offset and a grey-scale colour multiplier. 53300 is both the initial distance
  value and the green of the grass (`0x00D034`).
- **The first lap is a trick.** At start-up the car's position is set past the
  finish line, so the first frame "crosses" it and starts the lap clock.

## Repository layout

| File | Content |
|------|---------|
| `vik1.c` | The original 2004 entry, unchanged |
| `car_game.c` | The readable and extended version |
| `Makefile` | Builds `car_game` |

The Git history shows the de-obfuscation step by step, from the original (tag
`original`) to the current version. `git log -p car_game.c` shows each step as a
diff.

## License

The original vik1 is published by the IOCCC under the
[Creative Commons Attribution-ShareAlike 4.0 International](https://creativecommons.org/licenses/by-sa/4.0/)
license (CC BY-SA 4.0). This remake is a modified version of it and is released
under the same license. See [LICENSE](LICENSE).

## Thanks

- **Daniel Vik** for writing vik1 and proving that a full racing game fits in a
  few dozen lines of C.
- **The IOCCC** and its judges for running the contest and keeping the winning
  entries available: <https://www.ioccc.org/2004/vik1/>
- **Master-Hash** for the GitHub mirror of the IOCCC entries this work started
  from: <https://github.com/Master-Hash/ioccc-src/tree/master/2004/vik1>
