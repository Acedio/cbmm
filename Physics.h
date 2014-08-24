#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "Geometry.h"

using namespace std;

struct Body {
    bool enabled;
    Rect bbox;
    Vec2f speed;
};

class Physics {
public:
    int AddBody(Body body) { bodies.push_back(body); return bodies.size() - 1; }
    void Update(double t);
private:
    std::vector<Body> bodies;
};

#endif
