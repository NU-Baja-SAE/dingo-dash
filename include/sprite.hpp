#pragma once

#include <Arduino.h>

#include "vec.hpp"

class Image {
  const uint32_t width;
  const uint32_t height;
  const char *bits;

public:
  Image(uint32_t _width, uint32_t _height, char *_bits)
    : width(_width), height(_height), bits(_bits) {};

  template <typename T> void draw(T &u8g2, Vec2 &posn) {
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

class Sprite {
public:
  Image img; // mutable
  Vec2 posn; // mutable

  Sprite(Image _img, Vec2 _posn) : img(_img), posn(_posn) {};

  template <typename T> void draw(T &u8g2) { img.draw(u8g2, this->posn); }
};
