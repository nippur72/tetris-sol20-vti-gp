#include "draw.h"

#define DOUBLEFACTOR 2

/* X start position of the board on the screen */
#define STARTBOARD_X (SCREEN_WIDTH-((BCOLS*DOUBLEFACTOR)+2))/2

#if SCREEN_WIDTH == 64

   /* Y start position of the board on the screen */
   #define STARTBOARD_Y 0

   #define POS_SCORE_X 49
   #define POS_SCORE_Y 1

   #define POS_LEVEL_X 49
   #define POS_LEVEL_Y 6

   #define POS_LINES_X 49
   #define POS_LINES_Y 11

   #define POS_NEXT_X 3
   #define POS_NEXT_Y 1
   
#endif

#define NEXT_X (POS_NEXT_X+2)
#define NEXT_Y (POS_NEXT_Y+3)

void updateScore();
void drawPlayground();
void gameOver();

void gr_drawpiece(sprite *pl);
void gr_erasepiece(sprite *p);

void gr_update_board();
void gr_crunch_line(byte line, byte crunch_char, byte crunch_color);

// grahpic board
#include <stdio.h>            // for sprintf

#include "keyboard.h"
#include "pieces.h"
#include "ckboard.h"

extern unsigned long int score;

// update score table
char tmp[32];
void updateScore() {
   byte color = REVERSE_OFF;
   sprintf(tmp,"%6ld",score);
   draw_string(POS_SCORE_X+1,POS_SCORE_Y+2,tmp,color);

   sprintf(tmp,"%6d",total_lines);
   draw_string(POS_LINES_X+1,POS_LINES_Y+2,tmp,color);

   sprintf(tmp,"%6d",level);
   draw_string(POS_LEVEL_X+1,POS_LEVEL_Y+2,tmp,color);
}


void drawFrame(byte x, byte y, byte w, byte h, byte color) {
   byte i;
   for (i=1; i<w-1; i++) {      
      draw_tile(x+i, y    , FRAME_VERT, color);
      draw_tile(x+i, y+h-1, FRAME_VERT, color);
   }
   for (i=1; i<h-1; i++) {
      draw_tile(x    , y+i, FRAME_HORIZ, color);
      draw_tile(x+w-1, y+i, FRAME_HORIZ, color);
   }

   draw_tile(x     ,y    , FRAME_NE_CORNER, color);
   draw_tile(x+w-1 ,y    , FRAME_NW_CORNER, color);
   draw_tile(x     ,y+h-1, FRAME_SE_CORNER, color);
   draw_tile(x+w-1 ,y+h-1, FRAME_SW_CORNER, color);
}

void fillFrame(byte x, byte y, byte w, byte h, byte ch, byte color) {
   byte i,j;
   for (i=0; i<w; i++) {
      for (j=0; j<h; j++) {
         draw_tile(x+i, y+j, ch, color);
      }
   }
}

// draws the board
void drawPlayground() {

   byte frame_color = REVERSE_OFF;
   byte text_color  = REVERSE_OFF;

   // draw tetris board

   // background screen
   fillFrame(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,CH_BACKGROUND,REVERSE_OFF);

   // tetris frame
   fillFrame(STARTBOARD_X,STARTBOARD_Y,BCOLS*DOUBLEFACTOR,BROWS,CH_SPACE,REVERSE_OFF);
   #if SCREEN_HEIGHT == 16
      fillFrame(STARTBOARD_X-1, 0, 1, SCREEN_HEIGHT, FRAME_BOARD_VERT, REVERSE_OFF);      
      fillFrame(STARTBOARD_X-1+(BCOLS*DOUBLEFACTOR+1), 0, 1, SCREEN_HEIGHT, FRAME_BOARD_VERT, REVERSE_OFF);      
   #else
      drawFrame((byte)(STARTBOARD_X-1),(byte)(STARTBOARD_Y-1),(byte) (BCOLS*DOUBLEFACTOR+2),(byte) (BROWS+2), frame_color);
   #endif

   drawFrame(POS_SCORE_X, POS_SCORE_Y, 8, 4, frame_color);
   drawFrame(POS_LEVEL_X, POS_LEVEL_Y, 8, 4, frame_color);
   drawFrame(POS_LINES_X, POS_LINES_Y, 8, 4, frame_color);

   drawFrame(POS_NEXT_X , POS_NEXT_Y , 2+6*DOUBLEFACTOR, 8, frame_color);
   fillFrame(POS_NEXT_X+1 , POS_NEXT_Y+1 , 6*DOUBLEFACTOR, 6, 32, REVERSE_OFF);

   draw_string(POS_SCORE_X+1, POS_SCORE_Y+1, "SCORE ", text_color);
   draw_string(POS_LEVEL_X+1, POS_LEVEL_Y+1, "LEVEL ", text_color);

   draw_string(POS_LINES_X+1, POS_LINES_Y+1,"LINES ", text_color);
   draw_string(POS_NEXT_X +1, POS_NEXT_Y +1,"NEXT" , text_color);
}

// displays "game over" and waits for return key
void gameOver() {
   byte color = REVERSE_OFF;
   byte frame_color = REVERSE_OFF;

   byte y = (STARTBOARD_Y+BROWS)/2;

   byte center_x = (SCREEN_WIDTH - 16)/2;

   drawFrame(center_x, y-1,14,3, frame_color);

   draw_string(center_x+1,y-0," GAME  OVER ", REVERSE_OFF);

   // sound effect
   // bit_fx2(7);

   // since it's game over, there's no next piece
   gr_erasepiece(&piece_preview);

   byte flip = 0;
   while(1) {
      flip++;
      if(test_key(SCANCODE_RETN)) break;
           if(flip <  60 ) color = REVERSE_OFF;
      else if(flip < 120 ) color = REVERSE_ON;
      else flip = 0;
      draw_string(center_x+1,y-0," GAME  OVER ", color);
   }
}

// erase piece from the screen
void gr_erasepiece(sprite *p) {
   tile_offset *data = get_piece_offsets(p->piece, p->angle);
   int px = p->x * DOUBLEFACTOR;
   int py = p->y;

   // are we erasing the "next" piece ?
   if(py==PIECE_IS_NEXT) {
      px = NEXT_X;
      py = NEXT_Y;
   }
   else {
      px += STARTBOARD_X;
      py += STARTBOARD_Y;
   }

   for(byte t=0; t<4; t++) {
      #if DOUBLEFACTOR == 1
         int x = px + data->offset_x;
         int y = py + data->offset_y;
         data++;
         draw_tile((byte)x,(byte)y,EMPTY_GR_CHAR,REVERSE_OFF);
      #else
         int x = px + data->offset_x * DOUBLEFACTOR;
         int y = py + data->offset_y;
         data++;
         draw_tile((byte)x ,(byte)y,EMPTY_GR_CHAR,REVERSE_OFF);
         draw_tile((byte)x+1,(byte)y,EMPTY_GR_CHAR,REVERSE_OFF);
      #endif
   }
}

// erase piece from the screen, only for the tiles that have not been "marked"
void gr_erasepiece_unmarked(sprite *p) {
   tile_offset *data = get_piece_offsets(p->piece, p->angle);
   int px = p->x * DOUBLEFACTOR;
   int py = p->y;

   px += STARTBOARD_X;
   py += STARTBOARD_Y;
   
   for(byte t=0; t<4; t++) {
      int cx = p->x + data->offset_x;      
      int cy = p->y + data->offset_y;
            
      #if DOUBLEFACTOR == 1
         int x = px + data->offset_x;
         int y = py + data->offset_y;
         data++;
         // check if the tile has been previously "marked"
         if(board[cy][cx] != MARKED) {  
            draw_tile((byte)x,(byte)y,EMPTY_GR_CHAR,REVERSE_OFF);
         }
      #else
         int x = px + data->offset_x * DOUBLEFACTOR;
         int y = py + data->offset_y;
         data++;
         // check if the tile has been previously "marked"
         if(board[cy][cx] != MARKED) {  
            draw_tile((byte)x ,(byte)y,EMPTY_GR_CHAR,REVERSE_OFF);
            draw_tile((byte)x+1,(byte)y,EMPTY_GR_CHAR,REVERSE_OFF);
         }
      #endif
      }           
   }
}

// draw a piece on the screen
void gr_drawpiece(sprite *p) {
   tile_offset *data = get_piece_offsets(p->piece, p->angle);
   int px = p->x * DOUBLEFACTOR;
   int py = p->y;

   // are we drawing the "next" piece ?
   if(py==PIECE_IS_NEXT) {
      px = NEXT_X;
      py = NEXT_Y;
   }
   else {
      px += STARTBOARD_X;
      py += STARTBOARD_Y;
   }

   for(byte t=0; t<4; t++) {
      #if DOUBLEFACTOR == 1
         int x = px + data->offset_x;
         int y = py + data->offset_y;
         data++;
         byte ch  = piece_chars[p->piece];
         byte col = piece_colors[p->piece];
         draw_tile((byte)x,(byte)y,ch,col);
      #else
         int x = px + data->offset_x * DOUBLEFACTOR;
         int y = py + data->offset_y;
         data++;
         byte ch  = piece_chars[p->piece];
         byte col = piece_colors[p->piece];         
         draw_tile((byte)x  ,(byte)y,ch,col);
         draw_tile((byte)x+1,(byte)y,ch,col);
      #endif
   }
}

// fills the specified line with an empty character
void gr_crunch_line(byte line, byte crunch_char, byte crunch_color) {
   for(byte i=0; i<BCOLS*DOUBLEFACTOR; i++) {
      draw_tile(STARTBOARD_X+i, STARTBOARD_Y+line, crunch_char, crunch_color);
   }
}

void gr_update_board() {
   byte tile,ch,col;
   for(byte line=0;line<BROWS;line++) {
      for(byte column=0;column<BCOLS;column++) {
         tile = board[line][column];
         ch = piece_chars[tile];
         col = piece_colors[tile];
         #if DOUBLEFACTOR == 1
            draw_tile(STARTBOARD_X+column, STARTBOARD_Y+line, ch, col);
         #else
            draw_tile(STARTBOARD_X+column*2,   STARTBOARD_Y+line, ch, col);
            draw_tile(STARTBOARD_X+column*2+1, STARTBOARD_Y+line, ch, col);
         #endif
      }
   }
}
