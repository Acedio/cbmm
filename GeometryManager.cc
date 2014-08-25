#include "GeometryManager.h"

#include <algorithm>
#include <iterator>

#include <GL/glew.h>

namespace {
    const float initialVertexData[24] = {
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
        32.0f, 24.0f,
        32.0f, 0.0f,
        0.0f, 24.0f,
        0.0f, 0.0f,
    };

    const unsigned int initialIndexData[4] = {
        0, 1, 2, 3
    };

    const float initialRectVertexData[32] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f/12, 0.0f, 1.0f,
        1.0f/16, 1.0f/12, 0.0f, 1.0f,
        1.0f/16, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
    };
}

GeometryManager::GeometryManager() {
    std::copy(initialVertexData,
            initialVertexData + 24,
            vertexData);
    std::copy(initialIndexData,
            initialIndexData + 4,
            indexData);
    std::copy(initialRectVertexData,
            initialRectVertexData + 32,
            rectVertexData);

    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &rectPositionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, rectPositionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertexData), rectVertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
}

GeometryManager::~GeometryManager() {
}

void GeometryManager::DrawTileMap() {
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(16*sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

    glDrawElements(GL_TRIANGLE_STRIP, sizeof(indexData)/sizeof(unsigned int), GL_UNSIGNED_INT, (void*)0);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertexData), rectVertexData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(16*sizeof(float)));

    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GeometryManager::DrawRects(const std::function<const Rect*(size_t i)>& rect_at) {
    for (int i = 0;; ++i) {
        const Rect* rect = rect_at(i);
        if (!rect) {
            break;
        }
        DrawRect(rect->upperLeft.x/16.0 - 0.5, rect->upperLeft.y/12.0 - 0.5, rect->w/16.0, rect->h/12.0);
    }
}
