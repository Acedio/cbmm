#include "TileMap.h"
#include "tmxparser/Tmx.h"

#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

TileMap::TileMap() {
  w = 0;
  h = 0;
}

int TileMap::LoadTmx(const std::string& filename,
                     const std::string& layer_name) {
  std::unique_ptr<Tmx::Map> map(new Tmx::Map());
  map->ParseFile(filename);

  if (map->HasError()) {
    std::cout << "Error code: " << map->GetErrorCode() << std::endl;
    std::cout << "Error text: " << map->GetErrorText() << std::endl;
    
    return map->GetErrorCode();
  }

  for (int i = 0; i < map->GetNumTileLayers(); ++i) {
    const Tmx::TileLayer *tile_layer = map->GetTileLayer(i);
    if (tile_layer->GetName() == layer_name) {
      std::cout << "Found layer \"" << layer_name << "\"." << std::endl;
      w = tile_layer->GetWidth();
      h = tile_layer->GetHeight();

      tiles.resize(w*h);

      for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
          // tmx maps have origins in the upper left, but we want lower left.
          set(x, h - 1 - y, tile_layer->GetTileId(x, y));
        }
      }
      return 0;
    }
  }
  std::cout << "Couldn't find layer \"" << layer_name << "\"." << std::endl;
  return -1;
}

TileType TileMap::At(int x, int y) const {
  if (x < 0 || y < 0 || x >= w || y >= h) {
    return TileType::TILE_EMPTY;
  }
  return (TileType)get(x, y);
}

void TileMap::Print(std::ostream& os) const {
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      os << get(x, y) << " ";
    }
    os << endl;
  }
}

int TileMap::get(int x, int y) const {
  return tiles[y * w + x];
}

void TileMap::set(int x, int y, int tile) {
  tiles[y * w + x] = tile;
}
