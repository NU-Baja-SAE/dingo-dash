#include <Arduino.h>
#include <U8g2lib.h>

#include <assets/thermometer16.xbm>

#include "sprite.hpp"
#include "vec.hpp"
#include "BajaCan.h"
#include "can_data.hpp"

// DISPLAY INIT ================================================================

// requires macro-expanded arguments
#define U8G2_T6963_Exp(W, H, S) U8G2_T6963_##W##X##H##_##S##_8080
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
#define wr 12    // "write" for the 8080 interface (WR)
#define cs 14    // chip select line (CS)
#define dc 27    // data/command selection line (DC)
#define reset 26 // reset line

// rd shouldn't be floating, connect to 3.3v

// display definition ----------------------------------------------------------

U8G2 u8g2(U8G2_R0, d0, d1, d2, d3, d4, d5, d6, d7, wr, cs, dc, reset);

// DISPLAY SETUP ===============================================================

Sprite hud(IMAGE(thermometer16), Vec2());

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  can_init();
}

// RENDER LOOP =================================================================

void loop() {
  Can_Data data = read_input();
  Serial.printf("RPM: %.2f\n", data.engine_rpm);
  u8g2.clearBuffer();
  hud.posn += Vec2(1, 2);
  hud.posn.x %= 100;
  hud.posn.y %= 100;
  hud.draw(u8g2);
  //Serial.println(data.engine_rpm);
  u8g2.drawStr(20, 20, String(data.engine_rpm, 2).c_str());
  //u8g2.drawStr(20, 20, "Hello, world!");

  u8g2.sendBuffer();

  delay(1000);
}

