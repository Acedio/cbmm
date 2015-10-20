#ifndef TILEMAP_H
#define TILEMAP_H

#include <ostream>
#include <vector>

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
  TileMap();
  int LoadTmx(const std::string& filename, const std::string& layer_name);
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

#endif
