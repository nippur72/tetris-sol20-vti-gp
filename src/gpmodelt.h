// General Processor Model T10

#include "types.h"
#include "keyboard.h"

#define SCREEN_WIDTH   64
#define SCREEN_HEIGHT  16
#define SCREEN_ADDRESS 0xC000

#define REVERSE_OFF 0
#define REVERSE_ON  128

#define FRAME_VERT      (131-128)
#define FRAME_HORIZ     (132-128)
#define FRAME_NW_CORNER (129-128)
#define FRAME_NE_CORNER (130-128)
#define FRAME_SW_CORNER (134-128)
#define FRAME_SE_CORNER (135-128)

#define FRAME_BOARD_VERT (132-128)

#define CH_BACKGROUND 25
#define CH_SPACE      32

#define CRUNCH_CHAR_1   25
#define CRUNCH_COLOR_1  REVERSE_OFF

#define CRUNCH_CHAR_2   32
#define CRUNCH_COLOR_2  REVERSE_OFF

byte test_key(byte key);
byte read_keyboard();
byte player_input();
byte read_gp_keyboard_port();

#define SCANCODE_RETN 0x0d

// test a specific key
byte test_key(byte key) {
   return read_gp_keyboard_port() == key ? 1 : 0;
}

// reads the GP keyboard port
byte _key = 0;
INLINE byte read_gp_keyboard_port() {
   __asm
   in a,(0xd8)
   xor a, 0xff
   ld (__key),a
   __endasm;
   return _key;
}

byte player_input() {
   byte key = read_gp_keyboard_port();

   if((key & 128) == 0) return 0; // key not pressed

   // wait for the key to be released
   while(1) {
      byte strobe = read_gp_keyboard_port() & 128;
      if(strobe != 0) break;
   }

   // strip strobe bit
   key = key & 0x7f;

        if(key=='I' || key=='i') return KEY_ROTATE;
   else if(key=='J' || key=='j') return KEY_LEFT;
   else if(key=='K' || key=='k') return KEY_DOWN;
   else if(key=='L' || key=='l') return KEY_RIGHT;
   else if(key==' ') return KEY_DROP;
   else if(key==13) return 13;  // TODO handle differently
   else return 0;
}
