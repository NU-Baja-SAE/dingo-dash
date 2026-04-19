#include <Arduino.h>
#include <U8g2lib.h>

#include "assets.hpp"
#include "can.hpp"
#include "drawing.hpp"
#include "sprite.hpp"
#include "vec.hpp"

//   +----------------+
//  /  DISPLAY INIT  / =========================================================
// +----------------+

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

//   +----------------+
//  /  DISPLAY SETUP / =========================================================
// +----------------+

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  can::init();
}

//   +---------------+
//  /  RENDER LOOP  / ==========================================================
// +---------------+

// helper macros ---------------------------------------------------------------

#define LEN(arr) (sizeof(arr) / sizeof(*arr))
#define CLAMP(x, y, z) min(max(x, y), z)
#define LERP(x, y, f) ((x) + (((y) - (x)) * (f)))

// constants -------------------------------------------------------------------

// ... timing
const float dt = 100; // millis per frame
// ... needle
const float max_speed = 50.;
const float idle_needle_angle_deg = 200.;
const float max_speed_needle_angle_deg = -20.;
const Vec2<int16_t> needle_origin(120, 78);
const float needle_base_width = 6.;
const float needle_length = 40.;
// ... speed
const Vec2<int16_t> speed_text_pos(105, 60);
// ... thermometer
const float temp_danger_zone = 150.;
const Vec2<int16_t> temp_text_pos(205, 48);
// ... gas
const Vec2<int16_t> gas_text_pos(219, 94);
// ... fonts
const auto small_font = u8g2_font_3x5im_te;
const auto big_font = u8g2_font_boutique_bitmap_9x9_bold_tn;

// sprites ---------------------------------------------------------------------

SingleFrameSprite bg(image_background, Vec2<int16_t>());
SingleFrameSprite knob(image_knob, Vec2<int16_t>(118, 76));

// clang-format off
MultiFrameSprite comms(
  (Image *)frames_comms, LEN(frames_comms), 0, Vec2<int16_t>(80, 0));
MultiFrameSprite gas(
  (Image *)frames_gas, LEN(frames_gas), 0, Vec2<int16_t>(190, 75));
MultiFrameSprite thermometer(
  (Image *)frames_thermometer, LEN(frames_thermometer), 0, Vec2<int16_t>(188, 33));
MultiFrameSprite mode_manual(
  (Image *)frames_mode_manual, LEN(frames_mode_manual), 0, Vec2<int16_t>(11, 84));
MultiFrameSprite mode_torque(
  (Image *)frames_mode_torque, LEN(frames_mode_torque), 0, Vec2<int16_t>(11, 68));
MultiFrameSprite mode_power(
  (Image *)frames_mode_power, LEN(frames_mode_power), 0, Vec2<int16_t>(19, 52));
// clang-format on

// drawing helper fns ----------------------------------------------------------

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

float get_needle_angle_deg(float speed) {
  return LERP(
    idle_needle_angle_deg, max_speed_needle_angle_deg, speed / max_speed
  );
}

void draw_needle(U8G2 u8g2, float speed) {}

// render loop -----------------------------------------------------------------

// elapsed time, seconds
// TODO remove
float t = 0.0;

void loop() {
  can::CanInput data = can::read();
  can::debug(data);

  t += dt / 1000.;

  // Dummy values (temporary)
  auto temp_val = (sin(t * 0.2) / 2 + .5) * 160;
  auto gas_val = cos(t * 0.5) / 2 + .5;
  auto speed_val = (sin(t * 0.3) / 2 + .5) * 50;

  u8g2.clearBuffer();
  bg.draw(u8g2);

  // setup data for drawing
  thermometer.current_frame = get_thermometer_frame(temp_val);
  gas.current_frame = get_gas_frame(gas_val);

  mode_manual.current_frame = ((int)t) % 3 != 0;
  mode_torque.current_frame = ((int)t) % 3 != 1;
  mode_power.current_frame = ((int)t) % 3 != 2;

  comms.current_frame = ((int)t) % 2 == 0;

  float needle_angle = get_needle_angle_deg(speed_val);

  // draw everything
  thermometer.draw(u8g2);
  gas.draw(u8g2);
  mode_manual.draw(u8g2);
  mode_torque.draw(u8g2);
  mode_power.draw(u8g2);
  comms.draw(u8g2);

  char temp_str_buf[8];
  char gas_str_buf[8];
  char speed_str_buf[8];

  snprintf(temp_str_buf, 8, "%.0f\xB0", temp_val);
  snprintf(gas_str_buf, 8, "%.0f%%", gas_val * 100);
  snprintf(speed_str_buf, 8, "%05.2f%%", speed_val);

  u8g2.setFont(small_font);
  u8g2.drawStr(temp_text_pos.x, temp_text_pos.y, temp_str_buf);
  u8g2.drawStr(gas_text_pos.x, gas_text_pos.y, gas_str_buf);
  u8g2.setFont(big_font);
  u8g2.drawStr(speed_text_pos.x, speed_text_pos.y, speed_str_buf);

  draw_needle_polar(
    u8g2, needle_origin, needle_length, needle_angle, needle_base_width
  );

  knob.draw(u8g2);

  u8g2.sendBuffer();

  delay(dt);
}
