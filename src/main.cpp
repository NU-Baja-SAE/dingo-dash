// hardware
#include <Arduino.h>
#include <U8g2lib.h>

#include "compiled-assets.h"

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
#define d2 4
#define d3 16
#define d4 17
#define d5 5
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

static unsigned char hud_monochrome_flip[sizeof __assets_hud_bmp];

// oh my god someone please shoot me for this
unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);

  for (int i = 0; i < sizeof __assets_hud_bmp; i++) {
    hud_monochrome_flip[i] = reverse(__assets_hud_bmp[i]);
  }
  u8g2.setColorIndex(100);
  // u8g2.drawStr(0, 20, "Hello from ESP32!");
  u8g2.sendBuffer();
  delay(1000);
}

// GAME LOOP ===================================================================

auto x = 0;
void loop() {
  u8g2.clearBuffer();

  u8g2.drawXBMP(0, 0, 240, 128, hud_monochrome_flip);

  x += 2;
  u8g2.drawCircle((50 + x) % SCREEN_WIDTH, 50, 20);
  u8g2.drawArc((50+x) % SCREEN_WIDTH, 50, 10, 192-30, 192+30);
  u8g2.drawCircle((50 + x) % SCREEN_WIDTH - 7, 43, 3);
  u8g2.drawCircle((50 + x) % SCREEN_WIDTH + 7, 43, 3);
  u8g2.sendBuffer();

  delay(100);
}
