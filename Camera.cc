#include "Physics.h"
#include "Camera.h"

Camera::Camera(const vec2f& center, const vec2f& size)
    : center_(center), size_(size) {}

std::vector<std::unique_ptr<Event>> Camera::Update(
    Seconds, const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    // Take entity position, transform it with camera coords, possibly cull it
    // somehow if it isn't viewable, then write the GL coords to a transform
    // component.
    Body* body;
    Transform* transform;
    if (entity.GetComponents(&body, &transform)) {
      vec2f point = body->bbox.lowerLeft;
      vec2f half_size = size_/2;
      vec2f translated = point - (center_ + half_size);
      transform->position = {translated.x/half_size.x, translated.y/half_size.y};
    }
  }
  return {};
}
