#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "Geometry.h"
#include "System.h"

// Time to start documenting game coordinates vs GL coordinates...
// Game coordinates will be in "pixels", with a screen currently being 512x384
// pixels. TODO: Is 0,0 top left or bottom left?
// GL (screen) coordinates are always -1 to 1 in x and y, with 0,0 in the center
// of the screen.
// The Camera's job is to translate game coords to GL coords.

class Camera : public System {
 public:
  Camera() {};
  // Game coordinates
  Camera(const vec2f& center, const vec2f& size);
  std::vector<std::unique_ptr<Event>> Update(
      Seconds, const std::vector<Entity>&) override;
 private:
  // Game coordinates
  vec2f center_ = {0,0};
  vec2f size_ = {32, 24};
};

class Transform : public Component {
 public:
  ComponentType type() const override { return ComponentType::TRANSFORM; }
  vec2f position;
};

#endif  // CAMERA_H
