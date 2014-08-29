#include <cmath>
#include <iostream>

#include <SDL.h>

#include "Display.h"
#include "GeometryManager.h"
#include "Physics.h"
#include "ShaderManager.h"
#include "TextureManager.h"

using namespace std;

int main() {
    const unsigned int SCREEN_WIDTH = 1024;
    const unsigned int SCREEN_HEIGHT = 768;
    const unsigned int SCREEN_BPP = 32;

    Display display(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

    ShaderManager shaderManager = ShaderManager();
    int tileProgram = shaderManager.AddProgram("texture_vertex.glsl", "tile_fragment.glsl");
    int lineProgram = shaderManager.AddProgram("color_vertex.glsl", "color_fragment.glsl");

    GeometryManager geometryManager = GeometryManager();

    TextureManager textureManager = TextureManager();

    TextureRef tileSetRef = textureManager.LoadTexture("tileset.png", 0);
    TextureRef tileMapRef = textureManager.LoadTexture("tilemap.png", 0);

    textureManager.BindTexture(tileSetRef, 0);
    textureManager.BindTexture(tileMapRef, 1);

    Physics physics;
    for (int x = 0; x < 16; x++) {
        physics.AddBody({true, {{(double)x, (double)(1 + x)}, 1, 1}, {0, (double)0/2.0}});
    }

    TileMap map;
    map.Load("tiles.map");
    physics.SetTileMap(map);

    bool running = true;
    bool paused = true;

	int frames = 0;

    double t = 0;

    int last_ticks = SDL_GetTicks();

    while(running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_p:
                            paused = !paused;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        // Begin drawing
        
        display.Clear();

        shaderManager.UseProgram(tileProgram);
        geometryManager.DrawTileMap();

        glDisable(GL_DEPTH_TEST);
        shaderManager.UseProgram(lineProgram);
        geometryManager.DrawRects([&physics](size_t i){ return physics.GetBodyRect(i); });
        glEnable(GL_DEPTH_TEST);

        display.Swap();

        shaderManager.ClearProgram();

		double dt = (double)(SDL_GetTicks()-last_ticks)/1000.0;
        if (!paused) {
            physics.Update(dt);
        }
        t += dt;
		frames++;
        last_ticks = SDL_GetTicks();

		if (frames%100 == 0) {
			cout << (float)frames/t << endl;
		}

    }

    return 0;
}
