// Physics stuff
// NOTE: Y-axis is up-facing for physics purposes, e.g. gravity is down.
#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "Geometry.h"
#include "TileMap.h"

using namespace std;

typedef int BodyId;

const BodyId MAP_BODY_ID = -1;

class Body {
 public:
  bool enabled() { return enabled_; }
  void set_enabled(bool enabled) { enabled_ = enabled; };
  const Rect& bbox() { return bbox_; }
  void set_bbox(const Rect& bbox) { bbox_ = bbox; }
  const vec2f& vel() { return vel_; }
  void set_vel(const vec2f& vel) { vel_ = vel; }

  Body(bool enabled, const Rect& bbox, const vec2f& vel) {
    enabled_ = enabled;
    bbox_ = bbox;
    vel_ = vel;
  }
 private:
  bool enabled_;
  Rect bbox_;
  vec2f vel_;
};

struct Collision {
  BodyId first;
  BodyId second;
  vec2f fix;
};

typedef double Seconds;

class Physics {
 public:
  BodyId AddBody(Body body) {
    bodies.push_back(body);
    return bodies.size() - 1;
  }
  void SetTileMap(const TileMap& tile_map) { this->tile_map = tile_map; }
  vector<Collision> Update(Seconds dt);
  const Rect* GetBodyRect(BodyId id);

 private:
  bool RectRectCollision(const Rect& first, const Rect& second, vec2f* fix);
  bool RectMapCollision(const Rect& rect, vec2f* fix);
  TileMap tile_map;
  std::vector<Body> bodies;
};

#endif
