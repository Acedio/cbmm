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
  Sprite(TextureRef texture, int index, Orientation orientation, vec2f offset)
      : texture(texture),
        index(index),
        orientation(orientation),
        offset(offset) {}

  TextureRef texture = 0;
  int index = 0;
  Orientation orientation = Orientation::NORMAL;
  // The delta between where this sprite would normally be drawn and where it
  // will be drawn. Used for physics Bodys that do not line up with their
  // sprites.
  vec2f offset = {0,0};

  ComponentType type() const override { return ComponentType::SPRITE; }

  ~Sprite() override {}
};

#endif
