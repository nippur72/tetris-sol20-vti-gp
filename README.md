# tetris-sol20-vti-gp

This is my Tetris port for the following 8-bit computers:

- SOL-20 Terminal Computer by Processor Technologies
- (*) Altair 8800 or IMSAI 8080 with PolyMorphic Systems video terminal interface (VTI) board
- (*) Model T10 by General Processor (an Italian Z80 CP/M system)
- (*) TRS80 

(*) work in progress

The source code is based on my previous Tetris that I wrote for Laser 310/VZ200, 
Laser 500 and Apple1+TMS9918 video card. 

## Running on the SOL-20

From the `out` folder:
- `tetris_sol20.com` is the binary executable that can be loaded at address 0100h (it can be also exectued within CP/M as a normal .COM file)
- `tetris_sol20.ent` is a text file that can be typed-in from the SOL monitor. If you use the Solace emulator, load this file ad then type `EXEC 100`.












