#include <iostream>

#include <SDL/SDL.h>

#include "ShaderManager.h"
#include "Display.h"
#include "TextureManager.h"
#include "GeometryManager.h"

using namespace std;

int main(int argc, char **argv) {
    const unsigned int SCREEN_WIDTH = 640;
    const unsigned int SCREEN_HEIGHT = 480;
    const unsigned int SCREEN_BPP = 32;

    Display display(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

    ShaderManager shaderManager = ShaderManager();

    GeometryManager geometryManager = GeometryManager();

    TextureManager textureManager = TextureManager();

    TextureRef ref = textureManager.LoadTexture("bell.png");

    textureManager.BindTexture(ref);

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

        shaderManager.UseProgram();

        geometryManager.Draw();

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
