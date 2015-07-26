#ifndef TILEMAP_H
#define TILEMAP_H

#include <ostream>
#include <vector>

enum TileType {
  TILE_EMPTY = 0,
  TILE_BLOCK = 1,
  TILE_SLOPE_01 = 2,
  TILE_SLOPE_10 = 3
};

class TileMap {
 public:
  TileMap();
  int Load(char const *fileName);
  int At(int x, int y) const;
  void Print(std::ostream &os) const;

 private:
  std::vector<int> tiles;
  int w, h;
};

#endif
