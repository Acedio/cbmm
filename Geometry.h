#ifndef GEOMETRY_H
#define GEOMETRY_H

struct vec2f {
  double x, y;
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
  vec2f upperLeft;
  double w, h;
};

#endif
