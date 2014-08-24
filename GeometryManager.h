#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <GL/glew.h>

class GeometryManager {
private:
    float vertexData[24];
    unsigned int indexData[4];
    float rectVertexData[32];

    GLuint positionBufferObject;
    GLuint vertexArrayObject;
    GLuint indexBufferObject;
    GLuint rectPositionBufferObject;
public:
    GeometryManager();
    ~GeometryManager();
    void DrawTileMap();
    void DrawRect(float x, float y);
};

#endif
