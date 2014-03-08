#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <glew.h>

const float vertexData[] = {
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f,
	32.0f, 24.0f,
	32.0f, 0.0f,
	0.0f, 24.0f,
	0.0f, 0.0f,
};

const unsigned int indexData[] = {
    0, 1, 2, 3
};

class GeometryManager {
private:
    GLuint positionBufferObject;
    GLuint vertexArrayObject;
    GLuint indexBufferObject;
public:
    GeometryManager();
    ~GeometryManager();
    void Draw();
};

#endif
