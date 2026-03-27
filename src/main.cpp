// hardware
#include <Arduino.h>
#include <U8g2lib.h>

#include "assets/hud_monochrome.xbm"
#include "assets/smiley.xbm"
#include "vec.hpp"

// requires macro-expanded arguments
#define U8G2_T6963_Exp(W, H, S) U8G2_T6963_ ## W ## X ## H ## _ ## S ## _8080
#define U8G2_T6963(W, H, S) U8G2_T6963_Exp(W, H, S)

// display spec ----------------------------------------------------------------

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 128

// buffer size: {1, 2, F}
// 1: only one page of display memory stored in RAM; use firstPage()/nextPage()
//    for drawing
// 2: two pages of display memory stored in RAM, same behavior as 1
// F: keep a copy of the full frame buffer in RAM, use clearBuffer to clear RAM
//    and sendBuffer to send it to the display
#define BUFFER_SIZE F

// define the device type that we're using
#define U8G2 U8G2_T6963(SCREEN_WIDTH, SCREEN_HEIGHT, BUFFER_SIZE)

// pins ------------------------------------------------------------------------

// data lines of the parallel interface
#define d0 2
#define d1 0
#define d2 25
#define d3 32
#define d4 17
#define d5 33
#define d6 18
#define d7 19
// "write" for the 8080 interface (WR)
#define wr 12
// chip select line (CS)
#define cs 14
// data/command selection line (DC)
#define dc 27
// reset line
#define reset 26

// rd shouldn't be floating, connect to 3.3v

// display definition ----------------------------------------------------------

U8G2 u8g2(U8G2_R0, d0, d1, d2, d3, d4, d5, d6, d7, wr, cs, dc, reset);

// DISPLAY SETUP ===============================================================

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
}

// GAME LOOP ===================================================================

void loop() {
  u8g2.clearBuffer();

  u8g2.setColorIndex(0); // for some reason, we need to invert colors for drawing images
  u8g2.drawXBMP(0, 0, hud_monochrome_width, hud_monochrome_height, (unsigned char*) hud_monochrome_bits);
  u8g2.drawXBMP(0, 0, smiley_width, smiley_height, (unsigned char*) smiley_bits);

  u8g2.setColorIndex(1);
  u8g2.drawStr(20, 20, "hello world??");

  u8g2.sendBuffer();

  delay(1000);
}
