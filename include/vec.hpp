#pragma once

#include <Arduino.h>

template <typename T> struct Vec2 {
  T x;
  T y;

  Vec2(T _x, T _y) : x(_x), y(_y) {};
  Vec2() : x(0), y(0) {};

  T &operator+=(const Vec2 &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
  }

  friend Vec2<T> operator+(const Vec2<T> &lhs, const Vec2<T> &rhs) {
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
  }

  T &operator-=(const Vec2 &rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
  }

  friend Vec2<T> operator-(const Vec2<T> &lhs, const Vec2<T> &rhs) {
    return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
  }

  T &operator*=(const T s) {
    this->x *= s;
    this->y *= s;
    return *this;
  }

  friend Vec2<T> operator*(const Vec2<T> &v, const T s) {
    return Vec2(v.x * s, v.y * s);
  }

  float magnitude() const {
    return sqrt(pow((float)this->x, 2) + pow((float)this->y, 2));
  }

  Vec2<float> normalized() const {
    float mag = this->magnitude();
    return Vec2<float>(this->x / mag, this->y / mag);
  }

  template <typename Into> Vec2<Into> cast() {
    return Vec2<Into>((Into)this->x, (Into)this->y);
  }

  template <typename Into> Vec2<Into> round() {
    return Vec2<Into>((Into)this->x + 0.5, (Into)this->y + 0.5);
  }
};
