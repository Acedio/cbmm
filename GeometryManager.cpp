#include "GeometryManager.h"

#include <GL/glew.h>

GeometryManager::GeometryManager() {
    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
}

GeometryManager::~GeometryManager() {
}

void GeometryManager::Draw() {
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(16*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(32*sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

    glDrawElements(GL_TRIANGLE_STRIP, sizeof(indexData)/sizeof(unsigned int), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
