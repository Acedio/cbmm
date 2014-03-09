#ifndef GEOMETRY_H
#define GEOMETRY_H

struct Vec2f {
    double x, y;
}

struct Rect {
    Vec2f upperLeft;
    double w, h;
};

#endif
