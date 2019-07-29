#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <memory>
#include <vector>
#include <GL/glew.h>

#include "Camera.h"
#include "Geometry.h"

class Program {
 public:
  void Setup() {}
  void Use();

 protected:
  GLuint id;
};

class TextureProgram : public Program {
 public:
  void Setup();
  static std::unique_ptr<TextureProgram> Make();
 private:
  TextureProgram() {}
  GLint texture_uniform_;
};

class TextProgram : public Program {
 public:
  void Setup();
  static std::unique_ptr<TextProgram> Make();
  void scale(float scale) { scale_ = scale; }
  void offset(vec2f offset) { offset_ = offset; }
 private:
  TextProgram() {}
  vec2f offset_ = {0, 0};
  float scale_ = 1;
  GLint texture_uniform_;
  GLint transform_uniform_;
};

class MapProgram : public Program {
 public:
  void Setup();
  void map_offset(const vec2f& offset) { map_offset_ = offset; }
  static std::unique_ptr<MapProgram> Make();
 private:
  MapProgram() {}
  vec2f map_offset_ = {0, 0};
  GLint tileset_uniform_, tilemap_uniform_, offset_uniform_;
};

class ColorProgram : public Program {
 public:
  void Setup();
  static std::unique_ptr<ColorProgram> Make();
 private:
  ColorProgram() {}
};

#endif
