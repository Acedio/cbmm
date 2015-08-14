#include <cmath>
#include <iostream>

#include <SDL.h>

#include "Display.h"
#include "GeometryManager.h"
#include "Physics.h"
#include "ShaderManager.h"
#include "TextureManager.h"

using namespace std;

int main(int, char**) {
  const unsigned int SCREEN_WIDTH = 1024;
  const unsigned int SCREEN_HEIGHT = 768;
  const unsigned int SCREEN_BPP = 32;

  Display display(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

  ShaderManager shaderManager = ShaderManager();
  int tileProgram =
      shaderManager.AddProgram("resources/texture_vertex.glsl",
                               "resources/tile_fragment.glsl");
  int textureProgram =
      shaderManager.AddProgram("resources/texture_vertex.glsl",
                               "resources/texture_fragment.glsl");
  int lineProgram =
      shaderManager.AddProgram("resources/color_vertex.glsl",
                               "resources/color_fragment.glsl");

  GeometryManager geometryManager = GeometryManager();

  TextureManager textureManager = TextureManager();

  TextureRef tileSetRef =
      textureManager.LoadTexture("resources/tileset.png", 0);
  TextureRef tileMapRef =
      textureManager.LoadTexture("resources/tilemap.png", 0);
  TextureRef dogRef =
      textureManager.LoadTexture("resources/dog_tilesheet.png", 0);

  Physics physics;
  for (int x = 0; x < 16; x++) {
    physics.AddBody(
        {true, {{(double)x, (double)(1 + x)}, 1, 1}, {1, (double)0 / 2.0}});
  }

  TileMap map;
  if (map.Load("resources/tiles.map")) {
    cout << "Error loading tiles.map" << endl;
  }
  physics.SetTileMap(map);

  bool running = true;
  bool paused = true;

  int frames = 0;

  double t = 0;
  double delta = 0;

  int last_ticks = SDL_GetTicks();

  while (running) {
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

    glDisable(GL_DEPTH_TEST);

    textureManager.BindTexture(tileSetRef, 0);
    textureManager.BindTexture(tileMapRef, 1);
    shaderManager.UseProgram(tileProgram);
    geometryManager.DrawTileMap();

    textureManager.BindTexture(dogRef, 0);
    textureManager.BindTexture(-1, 1);
    shaderManager.UseProgram(textureProgram);
    geometryManager.DrawSubTexture(5.0 / 16, 1.0 - 4.0 / 16, 1.0 / 16, 1.0 / 16,
                                   -2.0 / 16, 1.333333 / 16, 1.0 / 16,
                                   1.333333 / 16);
    for (int i = 0; i < 16; ++i) {
      float angle = delta/(float)i;
      float magnitude = (float)i/16.0;
      geometryManager.DrawSubSprite((i / 4) * 16 + (i % 4), magnitude * cos(angle), magnitude * sin(angle));
    }

    shaderManager.UseProgram(lineProgram);
    geometryManager.DrawRects(
        [&physics](size_t i) { return physics.GetBodyRect(i); });
    glEnable(GL_DEPTH_TEST);

    display.Swap();

    shaderManager.ClearProgram();

    double dt = (double)(SDL_GetTicks() - last_ticks) / 1000.0;
    if (!paused) {
      vector<Collision> collisions = physics.Update(dt);
      delta += 8*dt;
      /* for (const Collision& c : collisions) {
        cout << "a " << c.first << " b " << c.second << " @ (" << c.fix.x << ","
             << c.fix.y << ")" << endl;
      } */
    }
    t += dt;
    frames++;
    last_ticks = SDL_GetTicks();

    if (frames % 100 == 0) {
      cout << (float)frames / t << endl;
    }
  }

  return 0;
}
