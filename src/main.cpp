#include <Arduino.h>
#include <U8g2lib.h>

#include "assets.hpp"
#include "sprite.hpp"
#include "vec.hpp"

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

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
}

// RENDER LOOP =================================================================

#define LEN(arr) (sizeof(arr) / sizeof(*arr))
#define CLAMP(x, y, z) min(max(x, y), z)

SingleFrameSprite bg(image_background, Vec2());
SingleFrameSprite knob(image_knob, Vec2(118, 76));

// clang-format off
MultiFrameSprite comms(
  (Image *)frames_comms, LEN(frames_comms), 0, Vec2(80, 0));
MultiFrameSprite gas(
  (Image *)frames_gas, LEN(frames_gas), 0, Vec2(190, 75));
MultiFrameSprite thermometer(
  (Image *)frames_thermometer, LEN(frames_thermometer), 0, Vec2(188, 33));
MultiFrameSprite mode_manual(
  (Image *)frames_mode_manual, LEN(frames_mode_manual), 0, Vec2(11, 84));
MultiFrameSprite mode_torque(
  (Image *)frames_mode_torque, LEN(frames_mode_torque), 0, Vec2(11, 68));
MultiFrameSprite mode_power(
  (Image *)frames_mode_power, LEN(frames_mode_power), 0, Vec2(19, 52));
// clang-format on

// dummy constants
constexpr float temp_danger_zone = 150.;

// gas_level is a float in the range [0, 1]. Outside this range will be clamped
size_t get_gas_frame(float gas_percentage) {
  return CLAMP(
    // cast to int to avoid underflow
    (int)roundf(LEN(frames_gas) * gas_percentage),
    0,
    (int)(LEN(frames_gas) - 1)
  );
}

// temp_fahrenheit is a float in the range [0, temp_danger_zone]. Outside this
// range will be clamped
size_t get_thermometer_frame(float temp_fahrenheit) {
  return CLAMP(
    (int)roundf(LEN(frames_thermometer) * (temp_fahrenheit / temp_danger_zone)),
    0,
    (int)(LEN(frames_thermometer) - 1)
  );
}

float x = 0.0;

void loop() {
  x += 0.2;
  u8g2.clearBuffer();
  bg.draw(u8g2);

  thermometer.current_frame = get_thermometer_frame((sin(x) / 2 + .5) * 160);
  gas.current_frame = get_gas_frame(cos(x) / 2 + .5);

  mode_manual.current_frame += 1;
  mode_torque.current_frame += 1;
  mode_power.current_frame += 1;
  comms.current_frame += 1;

  thermometer.draw(u8g2);
  gas.draw(u8g2);
  mode_manual.draw(u8g2);
  mode_torque.draw(u8g2);
  mode_power.draw(u8g2);
  comms.draw(u8g2);

  knob.draw(u8g2);

  u8g2.sendBuffer();

  delay(200);
}
