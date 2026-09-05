# tetris-sol20-vti-gp

This is my Tetris port for the following 8-bit computers:

- SOL-20 Terminal Computer by Processor Technology corp.
- Altair 8800 with boards: VDM-1 (video) and Cromemco D+7A + JS-1 joystick
- Model T08 by General Processor (an Italian Z80 CP/M system)
- TRS80 

The source code is based on my previous Tetris that I wrote for Laser 310/VZ200, 
Laser 500 and Apple1+TMS9918 video card. 

## Running on the SOL-20

From the `out` folder:
- `tetris_sol20.com` is the binary executable that can be loaded at address 0100h (it can be also exectued within CP/M as a normal .COM file)
- `tetris_sol20.ent` is a text file that can be typed-in from the SOL monitor. If you use the Solace emulator, load this file ad then type `EXEC 100`.

## Running on the TRS-80

On the Real machine:
- convert .cas to .wav with cas2wav
- reset the machine, hold BREAK key and go into BASIC
- type SYSTEM
- at the "*?" prompt type "." (file name first letter) and ENTER
- two asterisks should flash top right corner of the screen
- at the "*?" prompt type "/" to execute

## Running on the Altair 8800

Requires a VDM-1 Video card at address CC00h and Cromemco D+7A I/O board with a JS-1 joystick,
at I/O addresses 18h-1Ah (for joystick input). No keyboard is used.

From the `out` folder: `tetris_altair.com` is a normal CP/M executable.

Joystick controls:
- stick left/right: move the piece (autorepeat while held)
- stick down: soft drop
- button D0 (B1): rotate
- button D1 (B2): instant drop, and press it at the intro screen
  or after GAME OVER to start/restart

