#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>

struct vec2f {
  vec2f(double x, double y) : x(x), y(y) {}
  vec2f() {}
  double x = 0;
  double y = 0;
};

inline vec2f operator+(const vec2f& a, const vec2f& b) {
  return {a.x + b.x, a.y + b.y};
}

inline vec2f& operator+=(vec2f& a, const vec2f& b) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

inline vec2f operator-(const vec2f& a, const vec2f& b) {
  return {a.x - b.x, a.y - b.y};
}

inline vec2f operator*(const vec2f& a, const float& b) {
  return {a.x * b, a.y * b};
}

inline vec2f operator*(const float& a, const vec2f& b) { return b * a; }

inline vec2f operator/(const vec2f& a, const float& b) {
  return {a.x / b, a.y / b};
}

struct Rect {
  vec2f lowerLeft;
  double w, h;
};

#endif
