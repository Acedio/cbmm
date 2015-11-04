#include "TileMap.h"

#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

TileMap::TileMap(const Tmx::TileLayer* tile_layer) {
  w = tile_layer->GetWidth();
  h = tile_layer->GetHeight();

  tiles.resize(w*h);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      // tmx maps have origins in the upper left, but we want lower left.
      set(x, h - 1 - y, tile_layer->GetTileId(x, y));
    }
  }
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

int Map::LoadTmx(const std::string& filename) {
  map_.reset(new Tmx::Map());
  map_->ParseFile(filename);

  if (map_->HasError()) {
    std::cout << "Error code: " << map_->GetErrorCode() << std::endl;
    std::cout << "Error text: " << map_->GetErrorText() << std::endl;
    
    return map_->GetErrorCode();
  }

  for (int i = 0; i < map_->GetNumTileLayers(); ++i) {
    const Tmx::TileLayer* tile_layer = map_->GetTileLayer(i);
    std::string layer_name = tile_layer->GetName();
    layers_.emplace(layer_name, TileMap(tile_layer));
    std::cout << "Found layer \"" << layer_name << "\"." << std::endl;
  }

  for (int i = 0; i < map_->GetNumObjectGroups(); ++i) {
    const Tmx::ObjectGroup* object_group = map_->GetObjectGroup(i);
    for (int j = 0; j < object_group->GetNumObjects(); ++j) {
      const Tmx::Object* object = object_group->GetObject(j);
      MapObject mo;
      mo.name = object->GetName();
      // Flip the y-axis
      mo.pos = {
          (double)object->GetX() / map_->GetTileWidth(),
          (double)(map_->GetHeight() * map_->GetTileHeight() - object->GetY()) /
                  map_->GetTileHeight() -
              1};
      objects_[object->GetId()] = mo;
    }
  }

  return 0;
}

const TileMap* Map::GetLayer(const std::string& layer_name) const {
  const auto layer = layers_.find(layer_name);
  if (layer == layers_.end()) {
    return nullptr;
  }
  return &layer->second;
}

const MapObject* Map::GetNamedObject(const std::string& object_name) const {
  for (const auto& object : objects_) {
    if (object.second.name == object_name) {
      return &object.second;
    }
  }
  return nullptr;
}
