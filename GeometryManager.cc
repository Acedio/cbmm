#include "GeometryManager.h"

#include <algorithm>
#include <cassert>
#include <iterator>

#include <GL/glew.h>

#include "Camera.h"
#include "Physics.h"

namespace {
const float initialVertexData[24] = {
    // Positions
    1.0f,  1.0f,  0.0f,  1.0f, 1.0f,  -1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f,  0.0f,  1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
    // Texture coords
    16.0f, 12.0f, 16.0f, 0.0f, 0.0f,  12.0f, 0.0f, 0.0f,
};

const unsigned int initialIndexData[6] = {0, 1, 2, 2, 1, 3};

const float initialRectVertexData[32] = {
    0.0f,      0.0f,      0.0f, 1.0f, 0.0f,      1.0f / 6, 0.0f, 1.0f,
    1.0f / 8, 1.0f / 6, 0.0f, 1.0f, 1.0f / 8, 0.0f,      0.0f, 1.0f,
    1.0f,      1.0f,      0.0f, 0.0f, 0.0f,      1.0f,      0.0f, 0.0f,
    1.0f,      0.0f,      0.0f, 0.0f, 0.0f,      1.0f,      0.0f, 0.0f,
};
}

GeometryManager::GeometryManager() {
  std::copy(initialVertexData, initialVertexData + 24, vertexData);
  std::copy(initialIndexData, initialIndexData + 6, indexData);
  std::copy(initialRectVertexData, initialRectVertexData + 32, rectVertexData);
  for (size_t i = 0; i < 24; ++i) texVertexData[i] = 1.0;

  glGenBuffers(1, &positionBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &indexBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenBuffers(1, &rectPositionBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, rectPositionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertexData), rectVertexData,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  glGenBuffers(1, &texPositionBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, texPositionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texVertexData), texVertexData,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vertexArrayObject);
}

GeometryManager::~GeometryManager() {}

void GeometryManager::DrawTileMap(const Camera& camera) {
  vertexData[16] = 2 * camera.half_size().x;
  vertexData[17] = 2 * camera.half_size().y;
  vertexData[18] = 2 * camera.half_size().x;
  vertexData[21] = 2 * camera.half_size().y;

  glBindVertexArray(vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                        (void*)(16 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawElements(GL_TRIANGLES, sizeof(indexData) / sizeof(unsigned int),
                 GL_UNSIGNED_INT, (void*)0);
  glDisable(GL_BLEND);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void GeometryManager::DrawSubSprite(int si, Orientation orientation,
                                    const vec2f& dest_pos,
                                    const Camera& camera) {
  // TODO: Need to clamp the values to always land within the given tile.
  // Texels are occasionally grabbed from neighboring tiles.
  float width = 1.0 / 16.0;
  float height = 1.0 / 16.0;
  int rows = 16;
  int cols = 16;
  int row = si / cols;
  int col = si % rows;
  float x = col / (float) cols;
  // - height because y is defined from top of sprite but height is "upward"
  float y = 1.0 - row / (float) rows - height;
  if (orientation == Orientation::FLIPPED_H) {
    x += width;
    width = -width;
  }
  vec2f transformed = camera.Transform(dest_pos);
  DrawSubTexture(x, y, width, height, transformed.x, transformed.y,
                 1.0 / camera.half_size().x, 1.0 / camera.half_size().y);
}

void GeometryManager::DrawSubTexture(float sx, float sy, float sw, float sh,
                                     float dx, float dy, float dw, float dh) {
  texVertexData[0] = dx + dw;
  texVertexData[1] = dy + dh;
  texVertexData[2] = 0;

  texVertexData[4] = dx + dw;
  texVertexData[5] = dy;
  texVertexData[6] = 0;

  texVertexData[8] = dx;
  texVertexData[9] = dy + dh;
  texVertexData[10] = 0;

  texVertexData[12] = dx;
  texVertexData[13] = dy;
  texVertexData[14] = 0;

  texVertexData[16] = sx + sw;
  texVertexData[17] = sy + sh;

  texVertexData[18] = sx + sw;
  texVertexData[19] = sy;

  texVertexData[20] = sx;
  texVertexData[21] = sy + sh;

  texVertexData[22] = sx;
  texVertexData[23] = sy;

  glBindVertexArray(vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, texPositionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texVertexData), texVertexData,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                        (void*)(16 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawElements(GL_TRIANGLES, sizeof(indexData) / sizeof(unsigned int),
                 GL_UNSIGNED_INT, (void*)0);
  glDisable(GL_BLEND);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void GeometryManager::DrawRect(float x, float y, float w, float h) {
  rectVertexData[0] = x;
  rectVertexData[1] = y + h;
  rectVertexData[4] = x;
  rectVertexData[5] = y;
  rectVertexData[8] = x + w;
  rectVertexData[9] = y;
  rectVertexData[12] = x + w;
  rectVertexData[13] = y + h;

  glBindVertexArray(vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, rectPositionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertexData), rectVertexData,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
                        (void*)(16 * sizeof(float)));

  glDrawArrays(GL_LINE_LOOP, 0, 4);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void GeometryManager::DrawRects(const std::function<bool(Rect*)>& next_rect) {
  Rect rect;
  while (next_rect(&rect)) {
    DrawRect(rect.lowerLeft.x, rect.lowerLeft.y, rect.w, rect.h);
  }
}

BoundingBoxGraphicsSystem::BoundingBoxGraphicsSystem(
    GeometryManager* geometry_manager, ColorProgram* color_program)
    : geometry_manager_(geometry_manager), color_program_(color_program) {
  assert(geometry_manager_);
  assert(color_program_);
}

std::vector<std::unique_ptr<Event>> BoundingBoxGraphicsSystem::Update(
    Seconds, const Camera& camera, const std::vector<Entity>& entities) {
  color_program_->Use();
  auto iter = entities.begin();
  geometry_manager_->DrawRects([&iter, &entities, &camera](Rect* rect) {
    if (iter != entities.end()) {
      // TODO: What will the Transform component be used for?
      *rect = camera.Transform(iter->GetComponent<Body>()->bbox);
      ++iter;
      return true;
    }
    return false;
  });
  return {};
}

SubSpriteGraphicsSystem::SubSpriteGraphicsSystem(
    GeometryManager* geometry_manager, TextureProgram* texture_program,
    TextureManager* texture_manager)
    : geometry_manager_(geometry_manager),
      texture_program_(texture_program),
      texture_manager_(texture_manager) {
  assert(geometry_manager_);
  assert(texture_program_);
  assert(texture_manager_);
}

std::vector<std::unique_ptr<Event>> SubSpriteGraphicsSystem::Update(
    Seconds, const Camera& camera, const std::vector<Entity>& entities) {
  texture_manager_->BindTexture(-1, 1);
  texture_program_->Use();

  for (const auto& entity : entities) {
    Sprite* sprite;
    Body* body;
    if (entity.GetComponents(&sprite, &body)) {
      // TODO: Figure out how to ellide all draws of the same texture source
      // together.
      texture_manager_->BindTexture(sprite->texture, 0);
      geometry_manager_->DrawSubSprite(
          sprite->index, sprite->orientation,
          body->bbox.lowerLeft + sprite->offset, camera);
    }
  }
  return {};
}
