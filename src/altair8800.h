// Altair 8800
// video: VDM-1 at CC00h
// input: Cromemco D+7A board + JS-1 joystick, no keyboard
//
// D+7A I/O ports used by the JS-1:
//   0x18  buttons D0-D3 (active low: pressed = bit clear): D0 = drop, D1 = rotate
//   0x19  joystick X axis, signed two's complement (not centered at 0 on real HW)
//   0x1A  joystick Y axis, signed two's complement (not centered at 0; Y increases = up)

#include "types.h"
#include "keyboard.h"

#define COUNTER_MAX 1500    // the speed counter at level 0

#define SCREEN_WIDTH   64
#define SCREEN_HEIGHT  16
#define SCREEN_ADDRESS 0xCC00

/* when testing */
//#define SCREEN_ADDRESS 0xC000

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

#define CRUNCH_CHAR_2   4
#define CRUNCH_COLOR_2  REVERSE_OFF

#define FILLED_GR_CHAR  127
#define FILLED_GR_COLOR REVERSE_ON

// joystick definitions
#define JS_BIT_ROTATE     0x02    // button on D1
#define JS_BIT_DROP       0x01    // button on D0
// Real JS-1 is uncalibrated: idle is not (0,0) and travel is asymmetric, so a
// single |axis| > 64 test around 0 cannot work. Measured signed readings:
//   X: rest = +9,   full left = -74, full right = +95
//   Y: rest = +118, full up   = +127, full down  = +43
// Thresholds are ~halfway from rest to full deflection on that axis. Y-up has
// only 9 counts of travel before saturating at +127, so its window is tight.
#define JS_X_RIGHT         52     // rest(+9) .. right(+95)
#define JS_X_LEFT         -32     // rest(+9) .. left(-74)
#define JS_Y_UP           122     // rest(+118) .. up(+127)
#define JS_Y_DOWN          80     // rest(+118) .. down(+43)
#define KEY_REPEAT_COUNTER_MAX 100 /* stick autorepeat timer value */

#define SCANCODE_RETN 0x0d          // dummy value: intro/game over read the joystick

// one-time VDM-1 video card initialization
INLINE void init_video() {
   __asm
   xor a
   out (0xC8),a
   __endasm;
}

byte test_key(byte key);
byte player_input();

// reads the JS-1 buttons port
byte _js_buttons = 0;
INLINE byte read_buttons() {
   __asm
   in a,(0x18)
   ld (__js_buttons),a
   __endasm;
   return _js_buttons;
}

// reads the JS-1 X axis as a signed value
byte _js_x = 0;
INLINE signed char read_axis_x() {
   __asm
   in a,(0x19)
   ld (__js_x),a
   __endasm;
   return (signed char)_js_x;
}

// reads the JS-1 Y axis as a signed value
byte _js_y = 0;
INLINE signed char read_axis_y() {
   __asm
   in a,(0x1A)
   ld (__js_y),a
   __endasm;
   return (signed char)_js_y;
}

// start/restart test used by intro screen and game over: the DROP button
byte test_key(byte key) {
   return (read_buttons() & JS_BIT_DROP) ? 0 : 1;
}

// converts the joystick state into a KEY_* code
// buttons active low; axes compared against the measured rest/travel, not 0
byte read_joystick() {
   byte b;
   signed char x, y;

   b = read_buttons();

        if(!(b & JS_BIT_DROP))   return KEY_DROP;
   else if(!(b & JS_BIT_ROTATE)) return KEY_ROTATE;

   x = read_axis_x();
   y = read_axis_y();

        if(x > JS_X_RIGHT) return KEY_RIGHT;
   else if(x < JS_X_LEFT)  return KEY_LEFT;
   else if(y < JS_Y_DOWN)  return KEY_DOWN;     
   else if(y > JS_Y_UP)    return KEY_ROTATE;   
   else return 0;
}

// handle player input, implementing autorepeat
// for the stick directions; rotate and drop fire once per press
byte player_input() {
   static byte last_key = 0;
   static int repeat_counter = 0;

   byte key = read_joystick();

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
