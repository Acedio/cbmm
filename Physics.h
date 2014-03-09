#ifndef PHYSICS_H
#define PHYSICS_H

#include <map.h>

using namespace std;

struct Body {
    Rect bbox;
    Vec2f speed;
};

class Physics {
public:
    void Update(double t);
private:
    unordered_map<int, Body> bodyMap;
};

#endif
