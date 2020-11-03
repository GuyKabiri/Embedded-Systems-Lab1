# Embedded Systems Lab1
A lab as a part of the 'Embedded Systems' course.
Using switches to control LEDs and generate sound.
The program includes 3 modes, a binnary counter, a shifter and a fan, each one controled by a specific switch.
The code is written for Basys MX3 board.

## Mode 1 - Binnary Counter
Turn on SW0, the counter will initiate with `0` and will binnary count up to `255`.
```
Stage     0: ---- ----
Stage     1: ---- ---+
Stage     2: ---- --+-
Stage     3: ---- --++
.
.
.
Stage    37: --+- -+-+
.
.
.
Stage   255: ++++ ++++
```

## Mode 2 - Shifter
Turn on SW1, the first LED will turn on and will shift one LED each time.
```
Stage   1: -------+
Stage   2: ------+-
Stage   3: -----+--
Stage   4: ----+---
Stage   5: ---+----
Stage   6: --+-----
Stage   7: -+------
Stage   8: +-------
```

## Mode 3 - Fan
Turn on SW2, the two middle LEDs will turn on and will shift to the sides.
```
Stage   1: ---++---
Stage   2: --+--+--
Stage   3: -+----+-
Stage   4: +------+
```

## Other Actions

### Change Direction
Turn on SW3 to change the direction of the action performed.

### Change Speed
Turn on SW4 to change the speed of the action performed.

### Pause
Turn on SW5 to pause any action that is performed.

### Generate Sound
Turn on SW6 to generate a sound by the speaker on the board.

### Exit
Turn on SW7 to break from the loop and end the program.
