#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "Geometry.h"
#include "TileMap.h"

using namespace std;

typedef int BodyId;

const BodyId MAP_BODY_ID = -1;

struct Body {
  bool enabled;
  Rect bbox;
  vec2f vel;
};

struct Collision {
  BodyId first;
  BodyId second;
  vec2f fix;
};

class Physics {
 public:
  BodyId AddBody(Body body) {
    bodies.push_back(body);
    return bodies.size() - 1;
  }
  void SetTileMap(const TileMap& tile_map) { this->tile_map = tile_map; }
  vector<Collision> Update(double dt);
  const Rect* GetBodyRect(BodyId id);

 private:
  bool RectRectCollision(const Rect& first, const Rect& second, vec2f* fix);
  bool RectMapCollision(const Rect& rect, vec2f* fix);
  TileMap tile_map;
  std::vector<Body> bodies;
};

#endif
