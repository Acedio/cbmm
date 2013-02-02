#ifndef TILEMAP_H
#define TILEMAP_H

#include <ostream>

using namespace std;

class TileMap {
private:
    int *tiles;
    int w, h;
public:
    TileMap();
    ~TileMap();
    int Load(char const * fileName);
    void Print(ostream &os);
};

#endif
