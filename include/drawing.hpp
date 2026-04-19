#include <vec.hpp>

template <typename U8G2>
void draw_needle_cartesian(
  U8G2 u8g2, Vec2<int16_t> begin, Vec2<int16_t> end, float base_width
) {
  Vec2<int16_t> needle_v = end - begin;
  Vec2<int16_t> needle_ortho(-needle_v.y, needle_v.x); // 90deg ccw
  Vec2<float> needle_ortho_norm = needle_ortho.normalized();
  Vec2<int16_t> needle_base_offset
    = (needle_ortho_norm * (base_width / 2)).round<int16_t>();
  Vec2<int16_t> p1 = begin + needle_base_offset;
  Vec2<int16_t> p2 = begin - needle_base_offset;
  u8g2.drawTriangle(p1.x, p1.y, p2.x, p2.y, end.x, end.y);
  // so that we don't get artifacting at the tip / shaky length
  u8g2.drawLine(p1.x, p1.y, end.x, end.y);
  u8g2.drawLine(p2.x, p2.y, end.x, end.y);
}

template <typename U8G2>
void draw_needle_polar(
  U8G2 u8g2, Vec2<int16_t> begin, float len, float angle_deg, float base_width
) {
  Vec2<float> direction(cos(radians(angle_deg)), -sin(radians(angle_deg)));
  draw_needle_cartesian(
    u8g2, begin, begin + (direction * len).round<int16_t>(), base_width
  );
}
