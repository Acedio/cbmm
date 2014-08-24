#include <cmath>
#include <iostream>

#include <SDL.h>

#include "ShaderManager.h"
#include "Display.h"
#include "TextureManager.h"
#include "GeometryManager.h"

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

    bool running = true;

	int frames = 0;

    float t = 0;

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
        for (float x = -1; x < 1; x += 1.0/16.0) {
            geometryManager.DrawRect(x, abs(sin(t+x)) - 1.0/12.0);
        }
        glEnable(GL_DEPTH_TEST);

        display.Swap();

        shaderManager.ClearProgram();

		t += (float)(SDL_GetTicks()-last_ticks)/1000.0;
		frames++;
        last_ticks = SDL_GetTicks();

		if (frames%100 == 0) {
			cout << (float)frames/t << endl;
		}

    }

    return 0;
}
