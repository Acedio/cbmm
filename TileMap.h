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
  int LoadTmx(const std::string& filename, const std::string& layer_name);
  int Load(char const *fileName);
  int At(int x, int y) const;
  void Print(std::ostream &os) const;

  int GetWidth() const { return w; }
  int GetHeight() const { return h; }

 private:
  std::vector<int> tiles;
  int w, h;
};

#endif
