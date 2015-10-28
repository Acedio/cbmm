#ifndef TILEMAP_H
#define TILEMAP_H

#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "tmxparser/Tmx.h"

#include "Geometry.h"

enum TileType {
  TILE_EMPTY = 0,
  TILE_BLOCK = 1,
  TILE_SLOPE_01 = 2, 
  TILE_SLOPE_10 = 3,
  TILE_SLOPE_05 = 4,  // _,
  TILE_SLOPE_51 = 5,  // -`
  TILE_SLOPE_15 = 6,  // `-
  TILE_SLOPE_50 = 7,  // ,_
};

class TileMap {
 public:
  explicit TileMap(const Tmx::TileLayer* tile_layer);
  // 0,0 is lower left
  TileType At(int x, int y) const;
  void Print(std::ostream &os) const;

  int GetWidth() const { return w; }
  int GetHeight() const { return h; }

 private:
  int get(int x, int y) const;
  void set(int x, int y, int tile);
  std::vector<int> tiles;
  int w, h;
};

// Probably want some sort of typing to these.
struct MapObject {
  std::string name;
  vec2f pos;
};

class Map {
 public:
  int LoadTmx(const std::string& filename);
  const TileMap* GetLayer(const std::string& layer_name) const;
  const MapObject* GetObject(const std::string& object_name) const;
 private:
  std::unique_ptr<Tmx::Map> map_;
  // Layer name -> layer
  std::map<std::string, TileMap> layers_;
  std::map<int, MapObject> objects_;
};

#endif
