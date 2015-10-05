#include "Physics.h"
#include "Camera.h"

#include <iostream>

Camera::Camera(const vec2f& center, const vec2f& size)
    : center_(center), half_size_(size/2) {}

vec2f Camera::Transform(const vec2f& point) const {
  vec2f translated = point - center_;
  vec2f transformed = {translated.x / half_size_.x, translated.y / half_size_.y};
  /* std::cout << "p " << point.x << ", " << point.y << std::endl
            << "t " << transform->position.x << ", "
            << transform->position.y << std::endl; */
  return transformed;
}

Rect Camera::Transform(const Rect& rect) const {
  Rect transformed = {Transform(rect.lowerLeft), rect.w / half_size_.x,
                      rect.h / half_size_.y};
  return transformed;
}
