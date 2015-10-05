#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <cassert>
#include <functional>

#include <GL/glew.h>

#include "Camera.h"
#include "Geometry.h"
#include "ShaderManager.h"
#include "System.h"
#include "TextureManager.h"

class GeometryManager {
 public:
  GeometryManager();
  ~GeometryManager();
  void DrawTileMap();
  void DrawSubSprite(int si, float dx, float dy);
  void DrawSubTexture(float sx, float sy, float sw, float sh,
                      float dx, float dy, float dw, float dh);
  void DrawRect(float x, float y, float w, float h);
  void DrawRects(const std::function<bool(Rect*)>& next_rect);

 private:
  float vertexData[24];
  unsigned int indexData[6];
  float rectVertexData[32];
  float texVertexData[24];

  GLuint positionBufferObject;
  GLuint vertexArrayObject;
  GLuint indexBufferObject;
  GLuint rectPositionBufferObject;
  GLuint texPositionBufferObject;
};

class GraphicsSystem : public System {
 public:
  std::vector<std::unique_ptr<Event>> Update(
      Seconds, const std::vector<Entity>&) override {
    // TODO: Just dying here probably isn't what we want to do :P
    assert(false);
    return {};
  }
  virtual std::vector<std::unique_ptr<Event>> Update(
      Seconds dt, const Camera& camera,
      const std::vector<Entity>& entities) = 0;
};

class BoundingBoxGraphicsSystem : public GraphicsSystem {
 public:
  BoundingBoxGraphicsSystem(GeometryManager* geometry_manager,
                            ShaderManager* shader_manager);
  std::vector<std::unique_ptr<Event>> Update(
      Seconds dt, const Camera& camera,
      const std::vector<Entity>& entities) override;

 private:
  // Not owned.
  GeometryManager* geometry_manager_;
  ShaderManager* shader_manager_;
  int line_program_;
};

class SubSpriteGraphicsSystem : public GraphicsSystem {
 public:
  SubSpriteGraphicsSystem(GeometryManager* geometry_manager,
                          ShaderManager* shader_manager,
                          TextureManager* texture_manager);
  std::vector<std::unique_ptr<Event>> Update(
      Seconds dt, const Camera& camera,
      const std::vector<Entity>& entities) override;

 private:
  // Not owned.
  GeometryManager* geometry_manager_;
  ShaderManager* shader_manager_;
  TextureManager* texture_manager_;
  int texture_program_;
};

#endif
