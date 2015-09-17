#include <cmath>
#include <iostream>
#include <vector>

#include <SDL.h>

#include "Bog.h"
#include "Display.h"
#include "EntityManager.h"
#include "GeometryManager.h"
#include "Input.h"
#include "Physics.h"
#include "ShaderManager.h"
#include "State.h"
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
  TextureRef dogRef =
      textureManager.LoadTexture("resources/dog_tilesheet.png", 0);

  Physics physics;
  EntityManager em;
  vector<Entity> bogs;
  ComponentMap<Body> bodies;
  ComponentMap<StateMachine> state_machines;
  for (int x = 0; x < 16; x += 2) {
    Entity entity = em.CreateEntity();
    bogs.push_back(entity);
    bodies[entity].reset(
        new Body(true, {{(double)x, (double)x}, 1, 1}, {1, (double)0 / 2.0}));
    state_machines[entity].reset(new StateMachine(&bog_states::Standing::state));
  }

  TileMap collision_map;
  if (collision_map.LoadTmx("resources/test.tmx", "Collision")) {
    cout << "Error loading test.tmx" << endl;
  }
  physics.SetTileMap(collision_map);

  TileMap tilemap;
  if (tilemap.LoadTmx("resources/test.tmx", "Tiles")) {
    cout << "Error loading test.tmx" << endl;
  }
  TextureRef tileMapRef = textureManager.LoadTilemapTexture(tilemap);

  bool running = true;
  bool paused = true;

  int frames = 0;

  double t = 0;
  double delta = 0;

  int last_ticks = SDL_GetTicks();

  while (running) {
    for (ButtonEvent event : GetButtonEvents()) {
      if (event.button_state == ButtonState::RELEASED) {
        switch (event.button) {
          case Button::QUIT:
            running = false;
            break;
          case Button::PAUSE:
            paused = !paused;
          default:
            break;
        }
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

    auto dog = [](int i) {
      i %= 21;
      int rows = 0;
      if (i < 1) {
        return rows + i;
      }
      i -= 1;
      rows += 16;
      if (i < 4) {
        return rows + i;
      }
      i -= 4;
      rows += 16;
      if (i < 6) {
        return rows + i;
      }
      i -= 6;
      rows += 16;
      if (i < 12) {
        return rows + i;
      }
      return 0;
    };
    for (int i = 0; i < 32; ++i) {
      float angle = sin(delta*(float)i/32);
      float magnitude = (float)i/16.0;
      geometryManager.DrawSubSprite(dog(i),
                                    magnitude * cos(angle) - 1.0,
                                    magnitude * sin(angle));
    }

    shaderManager.UseProgram(lineProgram);
    auto iter = bodies.begin();
    geometryManager.DrawRects([&iter, &bodies]() {
      Rect* ret = nullptr;
      if (iter != bodies.end()) {
        ret = &iter->second->bbox;
        ++iter;
      }
      return ret;
    });
    glEnable(GL_DEPTH_TEST);

    display.Swap();

    shaderManager.ClearProgram();

    double dt = (double)(SDL_GetTicks() - last_ticks) / 1000.0;
    if (!paused) {
      for (const auto& bog : bogs) {
        state_machines[bog]->Update(bodies[bog].get(), dt);
      }
      vector<Collision> collisions = physics.Update(dt, bodies);
      for (const auto& collision : collisions) {
        Entity id = collision.first;
        state_machines[id]->HandleCollision(bodies[id].get(), collision);
      }
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
