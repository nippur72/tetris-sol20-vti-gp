#include "types.h"
#include "keyboard.h"

#define SCREEN_WIDTH   64
#define SCREEN_HEIGHT  16
#define SCREEN_ADDRESS 0x3C00

#define REVERSE_OFF 0
#define REVERSE_ON  0

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

#define KEY_REPEAT_COUNTER_MAX 400     /* key autorepeat timer value */

#define FILLED_GR_CHAR  191
#define FILLED_GR_COLOR   0

byte test_key(word scancode);
byte read_keyboard();
byte player_input();

/*   1   2   4   8  16  32  64  128 
   +---+---+---+---+---+---+---+---+  +--+---+---+---+---+---+---+---+---+
  1| @ | A | B | C | D | E | F | G |  |0 | ` | a | b | c | d | e | f | g |
   +---+---+---+---+---+---+---+---+  |  +---+---+---+---+---+---+---+---+
  2| H | I | J | K | L | M | N | O |  |1 | h | i | j | k | l | m | n | o |
   +---+---+---+---+---+---+---+---+  |  +---+---+---+---+---+---+---+---+
  4| P | Q | R | S | T | U | V | W |  |2 | p | q | r | s | t | u | v | w |
   +---+---+---+---+---+---+---+---+  |  +---+---+---+---+---+---+---+---+
  8| X | Y | Z | [ | \ | ] | ^ | _ |  |3 | x | y | z | { | | | } | ~ |   |
   +---+---+---+---+---+---+---+---+  |  +---+---+---+---+---+---+---+---+
 16| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |  |4 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
   +---+---+---+---+---+---+---+---+  |  +---+---+---+---+---+---+---+---+
 32| 8 | 9 | : | ; | , | - | . | / |  |5 | 8 | 9 | * | + | < | = | > | ? |
   +---+---+---+---+---+---+---+---+  |  +---+---+---+---+---+---+---+---+
 64|ENT|CLR|BRK|UP |DN |LFT|RGT|SPC|  |6 |ENT|CLR|BRK|UP |DN |LFT|RGT|SPC|
   +---+---+---+---+---+---+---+---+  |  +---+---+---+---+---+---+---+---+
128|SHF|   |   |   |   |   |   |   |  |7 |SHF|   |   |   |   |   |   |   |
   +---+---+---+---+---+---+---+---+  +--+---+---+---+---+---+---+---+---+

*/
// keyboard scan codes (row, col) for the test_key routine
#define SCANCODE_I    0x0202
#define SCANCODE_J    0x0204
#define SCANCODE_K    0x0208
#define SCANCODE_L    0x0210
#define SCANCODE_SPC  0x4080
#define SCANCODE_RETN 0x4001
#define SCANCODE_Z    0x0804

// test a specific scancode on the memory mapped I/O
INLINE byte test_key(word scancode) {
   byte *addr = (byte *)(0x3800+(scancode >> 8));
   if((*addr & (scancode & 0xFF))!=0) return 1;
   else return 0;
}

// reads the keyboard and return the key pressed
byte read_keyboard() {
        if(test_key(SCANCODE_I  )) return KEY_ROTATE;
   else if(test_key(SCANCODE_J  )) return KEY_LEFT;
   else if(test_key(SCANCODE_K  )) return KEY_DOWN;
   else if(test_key(SCANCODE_L  )) return KEY_RIGHT;
   else if(test_key(SCANCODE_SPC)) return KEY_DROP;
   else if(test_key(SCANCODE_Z  )) return KEY_DROP;   // alternate drop key 
   else return 0;
}

// handle player input, implementing key autorepeat
// for all keys except "rotate" and "drop"

byte player_input() {
   static byte last_key = 0;
   static int repeat_counter = 0;

   byte key = read_keyboard();

   if(key == KEY_LEFT || key == KEY_RIGHT || key == KEY_DOWN) {
      repeat_counter++;
      if(repeat_counter == KEY_REPEAT_COUNTER_MAX) {
         repeat_counter = 0;
         last_key = 0;
      }
   }
   else repeat_counter = 0;

   if(key == last_key) return 0;
   last_key = key;
   return key;
}
