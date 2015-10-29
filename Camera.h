#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "Geometry.h"
#include "System.h"

// Time to start documenting game coordinates vs GL coordinates...
// Game coordinates will be in tiles, with a screen currently being 32x24 tiles,
// with 0,0 in the bottom left corner.
// GL (screen) coordinates are always -1 to 1 in x and y, with 0,0 in the center
// of the screen.
// The Camera's job is to translate game coords to GL coords.

class Camera {
 public:
  // Game (tile) coordinates
  Camera(const vec2f& center, const vec2f& size);
  vec2f Transform(const vec2f& point) const;
  Rect Transform(const Rect& rect) const;
  vec2f center() const { return center_; }
  void center(const vec2f& center) { center_ = center; }
  vec2f half_size() const { return half_size_; }
  void half_size(const vec2f& half_size) { half_size_ = half_size; }
 private:
  // Game (tile) coordinates
  vec2f center_;
  // Half size is more useful here because GL draws from -1 to 1 rather than a
  // unit size of 1 (-0.5 to 0.5).
  vec2f half_size_;
};

class Transform : public Component {
 public:
  ComponentType type() const override { return ComponentType::TRANSFORM; }
  vec2f position;
};

#endif  // CAMERA_H
