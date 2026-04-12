#pragma once

#include <Arduino.h>

#include "vec.hpp"

class Image {
  const uint16_t width;
  const uint16_t height;
  const char *bits;

public:
  constexpr Image(uint16_t _width, uint16_t _height, char *_bits)
    : width(_width), height(_height), bits(_bits) {};

  template <typename T> void draw(T &u8g2, const Vec2<int16_t> &posn) const {
    // for some reason, we need to invert colors for drawing images
    // TODO can this be avoided by adjusting the palette?
    auto prev_color_idx = u8g2.getColorIndex();
    u8g2.setColorIndex(0);
    u8g2.drawXBMP(posn.x, posn.y, width, height, (unsigned char *)bits);
    u8g2.setColorIndex(prev_color_idx);
  }
};

// Convenience macro to initialize an Image based on an asset identifier
// For example, the asset identifier of include/assets/foo.xbm would be `foo`
#define IMAGE(id) Image(id##_width, id##_height, id##_bits)

// Sprite with a single, immutable frame
class SingleFrameSprite {
public:
  const Image frame;
  Vec2<int16_t> posn;

  SingleFrameSprite(Image _frame, Vec2<int16_t> _posn) : frame(_frame), posn(_posn) {};

  template <typename U8G2> void draw(U8G2 &u8g2) const {
    frame.draw(u8g2, this->posn);
  }
};

// Sprite with an immutable array set of frames
class MultiFrameSprite {
public:
  const Image *frames;
  const size_t num_frames;
  size_t current_frame;
  Vec2<int16_t> posn;

  MultiFrameSprite(
    Image _frames[], size_t _num_frames, size_t _initial_frame, Vec2<int16_t> _posn
  )
    : frames(_frames), num_frames(_num_frames), current_frame(_initial_frame),
      posn(_posn) {};

  template <typename U8G2> void draw(U8G2 &u8g2) const {
    this->frames[current_frame % num_frames].draw(u8g2, this->posn);
  }
};
