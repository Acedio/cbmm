// Physics stuff
// NOTE: Y-axis is up-facing for physics purposes, e.g. gravity is down.
#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "Entity.h"
#include "Component.h"
#include "Geometry.h"
#include "TileMap.h"
#include "System.h"

const EntityId MAP_BODY_ID = -1;

class Body : public Component {
 public:
  Body() {}
  Body(bool enabled, Rect bbox, vec2f vel)
      : enabled(enabled), bbox(bbox), vel(vel), last_pos(bbox.lowerLeft) {}

  bool enabled = false;
  Rect bbox = {{0,0},0,0};
  vec2f vel = {0,0};
  vec2f last_pos = {0,0};

  ComponentType type() const override { return ComponentType::BODY; }

  ~Body() override {}
};

class CollisionEvent : public Event {
 public:
  EventType type() const override { return EventType::COLLISION; }
  EntityId first;
  EntityId second;
  vec2f fix;
};

class Physics : public System {
 public:
  // tile_map must outlive this object.
  explicit Physics(const TileMap* tile_map) : tile_map_(tile_map) {}
  std::vector<std::unique_ptr<Event>> Update(
      Seconds dt, const std::vector<Entity>& entities) override;

 private:
  bool RectRectCollision(const Rect& first, const Rect& second, vec2f* fix);
  bool XCollision(const Rect& rect, double* x_fix);
  bool YCollision(const Rect& rect, double* y_fix);
  bool RectMapCollision(const Rect& rect, const vec2f& last_pos, vec2f* fix);
  const TileMap* tile_map_;
};

#endif
