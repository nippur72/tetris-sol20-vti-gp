#ifndef _DRAW_H_
#define _DRAW_H_

#include "types.h"

#define ESC_REVERSE_OFF "\x1b"
#define ESC_REVERSE_ON  "\x1c"

INLINE void draw_tile(byte x, byte y, byte ch, byte color) {
   byte *dest = (byte *) (SCREEN_ADDRESS + ((word)y*SCREEN_WIDTH) + (word)x);   
   *dest = ch|color;
}

void draw_string(byte x, byte y, char *s, byte color) {
   byte c;
   while(c=*s++) {
      draw_tile(x++, y, c, color);
   }
}

// print string with ESC + color support
void print_string(byte x, byte y, char *s, byte color) {
   byte c;
   while(c=*s++) {
           if(c==0x1b) color = REVERSE_OFF;
      else if(c==0x1c) color = REVERSE_ON;
      else {
         draw_tile(x++, y, c, color);
      }
   }
}

void cls() {
   memset((byte *)SCREEN_ADDRESS, CH_SPACE, SCREEN_HEIGHT*SCREEN_WIDTH);
}

void wait_interrupt() {
   //
}

#endif