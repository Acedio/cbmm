#include "TileMap.h"

#include <fstream>

using namespace std;

TileMap::TileMap() {
    tiles = NULL;
    w = 0;
    h = 0;
}

TileMap::~TileMap() {
    if(tiles != NULL) {
        delete[] tiles;
    }
}

/// Returns 0 on success
int TileMap::Load(char const * fileName) {
    ifstream tileMapFile(fileName, ifstream::in);

    if(!tileMapFile.good()) {
        return -1;
    }

    tileMapFile >> w >> h;

    if(w <= 0 || h <= 0) {
        return -1;
    }

    tiles = new int[w * h];

    if(tiles == NULL) {
        return -1;
    }

    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            tileMapFile >> tiles[y*w + x];
        }
    }

    return 0;
}

void TileMap::Print(ostream &os) {
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            os << tiles[y*w + x] << " ";
        }
        os << endl;
    }
}
