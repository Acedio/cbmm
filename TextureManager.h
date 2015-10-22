#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include <string>
#include <map>

#include "Component.h"
#include "TileMap.h"

using namespace std;

typedef unsigned int TextureRef;

class TextureManager {
 public:
  TextureManager();
  ~TextureManager();

  TextureRef LoadTexture(string filename, int level);
  void UnloadTexture(TextureRef ref);

  void BindTexture(TextureRef ref, int unit);

  TextureRef LoadTilemapTexture(const TileMap& tilemap);

 private:
  TextureRef getUnusedRef();
  map<string, TextureRef> filenames;
  map<TextureRef, GLuint> textures;
  map<TextureRef, int> refcounts;
  TextureRef next_unused_ref;
};

enum class Orientation {
  NORMAL,
  FLIPPED_H,
  FLIPPED_V
};

class Sprite : public Component {
 public:
  Sprite() {}
  Sprite(TextureRef texture, int index)
      : texture(texture), index(index) {}

  TextureRef texture = 0;
  int index = 0;
  Orientation orientation = Orientation::NORMAL;

  ComponentType type() const override { return ComponentType::SPRITE; }

  ~Sprite() override {}
};

#endif
