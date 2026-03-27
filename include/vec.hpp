#pragma once

#include <Arduino.h>

struct Vec2 {
  int32_t x;
  int32_t y;

  Vec2(int32_t _x, int32_t _y) : x(_x), y(_y) {};
  Vec2() : x(0), y(0) {};

  Vec2 &operator+=(const Vec2 &rhs);
  friend Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs);

  Vec2 &operator-=(const Vec2 &rhs);
  friend Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs);

  Vec2 &operator*=(const int32_t s);
  friend Vec2 operator*(const Vec2 &v, const int32_t s);
};
