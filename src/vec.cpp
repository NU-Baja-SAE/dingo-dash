#include <Arduino.h>

#include "vec.hpp"

Vec2 &Vec2::operator+=(const Vec2 &rhs) {
  this->x += rhs.x;
  this->y += rhs.y;
  return *this;
}

Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) {
  return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vec2 &Vec2::operator-=(const Vec2 &rhs) {
  this->x -= rhs.x;
  this->y -= rhs.y;
  return *this;
}

Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs) {
  return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vec2 &Vec2::operator*=(const int32_t s) {
  this->x *= s;
  this->y *= s;
  return *this;
}

Vec2 operator*(const Vec2 &v, const int32_t s) {
  return Vec2(v.x * s, v.y * s);
}
