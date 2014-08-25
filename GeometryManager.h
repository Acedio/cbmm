#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <functional>

#include <GL/glew.h>

#include "Geometry.h"

class GeometryManager {
public:
    GeometryManager();
    ~GeometryManager();
    void DrawTileMap();
    void DrawRect(float x, float y, float w, float h);
    void DrawRects(const std::function<const Rect*(size_t i)>& rect_at);
private:
    float vertexData[24];
    unsigned int indexData[4];
    float rectVertexData[32];

    GLuint positionBufferObject;
    GLuint vertexArrayObject;
    GLuint indexBufferObject;
    GLuint rectPositionBufferObject;
};

#endif
