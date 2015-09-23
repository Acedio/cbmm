#include "GeometryManager.h"

#include <algorithm>
#include <cassert>
#include <iterator>

#include <GL/glew.h>

#include "Physics.h"

namespace {
const float initialVertexData[24] = {
    1.0f,  1.0f,  0.0f,  1.0f, 1.0f,  -1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f,  0.0f,  1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
    32.0f, 24.0f, 32.0f, 0.0f, 0.0f,  24.0f, 0.0f, 0.0f,
};

const unsigned int initialIndexData[6] = {0, 1, 2, 2, 1, 3};

const float initialRectVertexData[32] = {
    0.0f,      0.0f,      0.0f, 1.0f, 0.0f,      1.0f / 12, 0.0f, 1.0f,
    1.0f / 16, 1.0f / 12, 0.0f, 1.0f, 1.0f / 16, 0.0f,      0.0f, 1.0f,
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
  glBindVertexArray(vertexArrayObject);
}

GeometryManager::~GeometryManager() {}

void GeometryManager::DrawTileMap() {
  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                        (void*)(16 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

  glDrawElements(GL_TRIANGLES, sizeof(indexData) / sizeof(unsigned int),
                 GL_UNSIGNED_INT, (void*)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GeometryManager::DrawSubSprite(int si, float dx, float dy) {
  const float width = 1.0 / 16.0;
  const float height = 1.0 / 16.0;
  const int rows = 16;
  const int cols = 16;
  int row = si / cols;
  int col = si % rows;
  float x = col / (float) cols;
  // - height because y is defined from top of sprite but height is "upward"
  float y = 1.0 - row / (float) rows - height;
  DrawSubTexture(x, y, width, height, dx, dy, 1.0 / 16.0, 1.33333 / 16);
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
}

void GeometryManager::DrawRect(float x, float y, float w, float h) {
  rectVertexData[0] = x;
  rectVertexData[1] = y;
  rectVertexData[4] = x;
  rectVertexData[5] = y - h;
  rectVertexData[8] = x + w;
  rectVertexData[9] = y - h;
  rectVertexData[12] = x + w;
  rectVertexData[13] = y;
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
}

void GeometryManager::DrawRects(const std::function<const Rect*()>& next_rect) {
  const Rect* rect;
  while ((rect = next_rect())) {
    if (!rect) {
      break;
    }
    DrawRect(rect->upperLeft.x / 16.0 - 1, rect->upperLeft.y / 12.0 - 1,
             rect->w / 16.0, rect->h / 12.0);
  }
}

BoundingBoxGraphicsSystem::BoundingBoxGraphicsSystem(
    GeometryManager* geometry_manager, ShaderManager* shader_manager)
    : geometry_manager_(geometry_manager), shader_manager_(shader_manager) {
  assert(geometry_manager_);
  assert(shader_manager_);
  line_program_ = shader_manager_->AddProgram("resources/color_vertex.glsl",
                                              "resources/color_fragment.glsl");
  assert(line_program_ >= 0);
}

std::vector<std::unique_ptr<Event>> BoundingBoxGraphicsSystem::Update(
    Seconds, const std::vector<Entity>& entities) {
  shader_manager_->UseProgram(line_program_);
  auto iter = entities.begin();
  geometry_manager_->DrawRects([&iter, &entities]() {
    Rect* ret = nullptr;
    if (iter != entities.end()) {
      ret = &iter->GetComponent<Body>()->bbox;
      ++iter;
    }
    return ret;
  });
  return {};
}
