#include "TileMap.h"

#include <fstream>

using namespace std;

TileMap::TileMap() {
    w = 0;
    h = 0;
}

/// Returns 0 on success
int TileMap::Load(char const* fileName) {
    ifstream tileMapFile(fileName, ifstream::in);

    if(!tileMapFile.good()) {
        return -1;
    }

    tileMapFile >> w >> h;

    if(w <= 0 || h <= 0) {
        return -1;
    }

    tiles.resize(w*h);

    for (int y = h - 1; y >= 0; y--) {
        for (int x = 0; x < w; x++) {
            if (!tileMapFile.good()) {
                return -1;
            }
            int temp;
            tileMapFile >> temp;
            tiles.at(y*w + x) = temp;
        }
    }

    return 0;
}

int TileMap::At(int x, int y) {
    if (x < 0 || y < 0 || x >= w || y >= h) {
        return -1;
    }
    return tiles.at(y*w + x);
}

void TileMap::Print(std::ostream &os) {
    for(int y = h - 1; y >= 0; y--) {
        for(int x = 0; x < w; x++) {
            os << tiles[y*w + x] << " ";
        }
        os << endl;
    }
}
