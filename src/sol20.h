#include "types.h"
#include "keyboard.h"

#define SCREEN_WIDTH   64
#define SCREEN_HEIGHT  16
#define SCREEN_ADDRESS 0xCC00

#define REVERSE_OFF 0
#define REVERSE_ON  128

#define FRAME_VERT      ' '
#define FRAME_HORIZ     ' '
#define FRAME_NW_CORNER ' '
#define FRAME_NE_CORNER ' '
#define FRAME_SW_CORNER ' '
#define FRAME_SE_CORNER ' '

#define FRAME_BOARD_VERT 127

#define CH_BACKGROUND 127
#define CH_SPACE      32

#define CRUNCH_CHAR_1   127
#define CRUNCH_COLOR_1  REVERSE_OFF

#define CRUNCH_CHAR_2   32
#define CRUNCH_COLOR_2  REVERSE_OFF

byte test_key(byte key);
byte read_keyboard();
byte player_input();
byte read_sol20_keyboard_port();
byte read_sol20_keyboard_status_port();

// test a specific key
byte test_key(byte key) {
   if(read_sol20_keyboard_status_port()==0) return 0;
   return read_sol20_keyboard_port() == key ? 1 : 0;
}

// reads the SOL20 keyboard status port
byte _key_status = 0;
INLINE byte read_sol20_keyboard_status_port() {   
   __asm   
   in a,(0xFA)   
   xor a, 0xff
   and 0x01   
   ld (__key_status),a      
   __endasm;
   return _key_status;
}

// reads the SOL20 keyboard port
byte _key = 0;
INLINE byte read_sol20_keyboard_port() {
   __asm
   in a,(0xfc)   
   ld (__key),a   
   __endasm;
   return _key;
}

byte player_input() {
   if(read_sol20_keyboard_status_port()==0) return 0;

   byte key = read_sol20_keyboard_port();   

        if(key=='I' || key=='i') return KEY_ROTATE;
   else if(key=='J' || key=='j') return KEY_LEFT;
   else if(key=='K' || key=='k') return KEY_DOWN;
   else if(key=='L' || key=='l') return KEY_RIGHT;
   else if(key==' ') return KEY_DROP;
   else if(key==13) return 13;  // TODO handle differently
   else return 0;
}

