#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "Geometry.h"
#include "TileMap.h"

using namespace std;

struct Body {
  bool enabled;
  Rect bbox;
  vec2f vel;
};

class Physics {
 public:
  int AddBody(Body body) {
    bodies.push_back(body);
    return bodies.size() - 1;
  }
  void SetTileMap(const TileMap& tile_map) { this->tile_map = tile_map; }
  void Update(double dt);
  const Rect* GetBodyRect(size_t i);

 private:
  bool RectMapCollision(const Rect& rect, vec2f* fix);
  TileMap tile_map;
  std::vector<Body> bodies;
};

#endif
