// Physics stuff
// NOTE: Y-axis is up-facing for physics purposes, e.g. gravity is down.
#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "Component.h"
#include "EntityManager.h"
#include "Geometry.h"
#include "TileMap.h"

using namespace std;

const Entity MAP_BODY_ID = -1;

class Body : public Component {
 public:
  Body(bool enabled, Rect bbox, vec2f vel) {
    this->enabled = enabled;
    this->bbox = bbox;
    this->vel = vel;
  }

  bool enabled;
  Rect bbox;
  vec2f vel;

  ComponentType type() const override { return ComponentType::BODY; }
};

struct Collision {
  Entity first;
  Entity second;
  vec2f fix;
};

typedef double Seconds;

class Physics {
 public:
  void SetTileMap(const TileMap& tile_map) { this->tile_map = tile_map; }
  vector<Collision> Update(Seconds dt, const ComponentMap<Body>& bodies);

 private:
  bool RectRectCollision(const Rect& first, const Rect& second, vec2f* fix);
  bool RectMapCollision(const Rect& rect, vec2f* fix);
  TileMap tile_map;
};

#endif
