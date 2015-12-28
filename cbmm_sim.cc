#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

// TODO: Why is this necessary on Jason's build?
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Animation.h"
#include "Bog.h"
#include "Box.h"
#include "Camera.h"
#include "Display.h"
#include "EntityManager.h"
#include "Event.h"
#include "Font.h"
#include "GeometryManager.h"
#include "Input.h"
#include "Physics.h"
#include "ShaderManager.h"
#include "State.h"
#include "Text.h"
#include "TextureManager.h"

using namespace std;

int main(int, char**) {
  const unsigned int SCREEN_WIDTH = 640;
  const unsigned int SCREEN_HEIGHT = 480;
  const unsigned int SCREEN_WIDTH_TILES = 20;
  const unsigned int SCREEN_HEIGHT_TILES = 15;
  const unsigned int TILE_SIZE = 16;
  const unsigned int SCREEN_BPP = 32;

  Display display(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

  std::unique_ptr<MapProgram> tileProgram = MapProgram::Make();
  std::unique_ptr<TextProgram> textProgram = TextProgram::Make();
  std::unique_ptr<TextureProgram> textureProgram = TextureProgram::Make();
  std::unique_ptr<ColorProgram> colorProgram = ColorProgram::Make();

  GeometryManager geometryManager;
  TextureManager textureManager;
  std::vector<Animation> animations;

  TextureRef tileSetRef =
      textureManager.LoadTexture("resources/tileset.png", 0);
  TextureRef collisionSetRef =
      textureManager.LoadTexture("resources/collision.png", 0);
  TextureRef dogRef =
      textureManager.LoadTexture("resources/dog_tilesheet.png", 0);
  TextureRef bgRef =
      textureManager.LoadTexture("resources/bg.png", 0);
  TextureRef fontRef =
      textureManager.LoadTexture("resources/dialogue.png", 0);

  animations.push_back(Animation({{32, 0.1}, {33, 0.1}, {34, 0.1},
                                  {35, 0.1}, {36, 0.1}, {37, 0.1}}));
  animations.push_back(Animation({{48, 0.1}, {49, 0.1}, {50, 0.1},
                                  {51, 0.1}, {52, 0.1}, {53, 0.1},
                                  {54, 0.1}, {55, 0.1}, {56, 0.1},
                                  {57, 0.1}, {58, 0.1}, {59, 0.1}}));

  std::unique_ptr<Font> font;
  {
    std::unique_ptr<PixelData> pd = LoadToPixelData("resources/dialogue.png");
    font = Font::MakeFont(*pd.get());
  }
  std::unique_ptr<Text> text = Text::MakeText(font.get());
  for (auto c : "And so begins our hero's adventure!") {
    text->AddCharacter(c);
  }

  auto jump_state_system = MakeJumpStateSystem();
  auto lr_state_system = MakeLRStateSystem();
  auto box_state_system = MakeBoxStateSystem();
  Camera camera({0, 0}, {SCREEN_WIDTH_TILES, SCREEN_HEIGHT_TILES});
  BoundingBoxGraphicsSystem bb_graphics(&geometryManager, colorProgram.get());
  SubSpriteGraphicsSystem ss_graphics(&geometryManager, textureProgram.get(),
                                      &textureManager);
  AnimationSystem animation_system(&animations);

  Map level;
  if (level.LoadTmx("resources/test.tmx")) {
    cout << "Error loading test.tmx" << endl;
  }
  const TileMap* collision_map = level.GetLayer("Collision");
  assert(collision_map);
  Physics physics(collision_map);

  const TileMap* tilemap = level.GetLayer("Tiles");
  assert(tilemap);
  TextureRef tileMapRef = textureManager.LoadTilemapTexture(*tilemap);
  TextureRef collisionMapRef = textureManager.LoadTilemapTexture(*collision_map);

  EntityManager em;
  vector<Entity> bogs;
  {
    // Les' make us a bawg.
    const MapObject* mo = level.GetNamedObject("bog-start");
    assert(mo);
    EntityId id = em.CreateEntity();
    bogs.emplace_back(id);
    bogs.back().AddComponent(std::unique_ptr<Transform>(new Transform()));
    bogs.back().AddComponent(std::unique_ptr<Body>(
        new Body(true, {mo->pos, 0.9, 0.75}, {0, 0})));
    bogs.back().AddComponent(std::unique_ptr<JumpStateComponent>(
        new JumpStateComponent(JumpState::STANDING)));
    bogs.back().AddComponent(std::unique_ptr<LRStateComponent>(
        new LRStateComponent(LRState::STILL)));
    bogs.back().AddComponent(std::unique_ptr<Sprite>(
        new Sprite(dogRef, 0, Orientation::NORMAL, {-1.0/16.0, 0})));
    bogs.back().AddComponent(std::unique_ptr<AnimationComponent>(
        new AnimationComponent(0)));

    const MapObject* box_mo = level.GetNamedObject("box");
    assert(box_mo);
    id = em.CreateEntity();
    bogs.emplace_back(id);
    bogs.back().AddComponent(std::unique_ptr<Transform>(new Transform()));
    bogs.back().AddComponent(std::unique_ptr<Body>(
        new Body(true, {box_mo->pos, 0.9, 0.9}, {0, 0})));
    bogs.back().AddComponent(std::unique_ptr<BoxStateComponent>(
          new BoxStateComponent(BoxState::AT_REST)));
    bogs.back().AddComponent(std::unique_ptr<Sprite>(
          new Sprite(tileSetRef, 1 /* block index */, Orientation::NORMAL, {0,0})));
  }

  bool running = true;
  bool paused = true;
  bool debug = false;

  int frames = 0;
  int logic_frames = 0;

  double t = 0;

  double time_scale = 1;

  int last_ticks = SDL_GetTicks();

  const int logics_per_second = 50;

  const int ticks_per_logic = 1000 / logics_per_second;

  int next_logic_ticks = last_ticks + ticks_per_logic;

  while (running) {
    last_ticks = SDL_GetTicks();

    while (last_ticks >= next_logic_ticks) {
      for (ButtonEvent event : GetButtonEvents()) {
        if (event.button_state() == ButtonState::RELEASED) {
          switch (event.button()) {
            case Button::QUIT:
              running = false;
              continue;
            case Button::PAUSE:
              paused = !paused;
              continue;
            case Button::DEBUG:
              debug = !debug;
              continue;
            default:
              break;
          }
        } else if (event.button_state() == ButtonState::PRESSED) {
          switch (event.button()) {
            case Button::PLUS:
              time_scale *= 1.2;
              break;
            case Button::MINUS:
              time_scale /= 1.2;
              break;
            default:
              break;
          }
        }
        // May want to prevent input from triggering two immediate state
        // changes?
        jump_state_system->HandleEvent(&event, bogs);
        lr_state_system->HandleEvent(&event, bogs);
        // TODO: Seems like there should be a way to manage the state systems,
        // this will get out of hand :P
        box_state_system->HandleEvent(&event, bogs);
      }

      // Fixed timestep
      double dt = (double)(ticks_per_logic) / 1000.0;
      if (!paused) {
        jump_state_system->Update(dt, bogs);
        lr_state_system->Update(dt, bogs);
        box_state_system->Update(dt, bogs);
        animation_system.Update(dt, bogs);
        vector<std::unique_ptr<Event>> events = physics.Update(dt, bogs);
        for (const auto& event : events) {
          auto* collision = static_cast<CollisionEvent*>(event.get());
          jump_state_system->HandleEvent(event.get(), bogs);
          lr_state_system->HandleEvent(event.get(), bogs);
          box_state_system->HandleEvent(event.get(), bogs);
        }
        // Interpolate camera to Bog.
        vec2f bog_pos = bogs.at(0).GetComponent<Body>()->bbox.lowerLeft;
        camera.center(bog_pos*0.2 + camera.center()*0.8);
        /* for (const Collision& c : collisions) {
          cout << "a " << c.first << " b " << c.second << " @ (" << c.fix.x << ","
               << c.fix.y << ")" << endl;
        } */
      }

      last_ticks = SDL_GetTicks();
      next_logic_ticks += ticks_per_logic;
      ++logic_frames;
      t += dt;

      if (logic_frames % 100 == 0) {
        cout << "Logic/s: " << (float)logic_frames / t << endl;
      }

    }
    frames++;

    // Begin drawing
    display.Clear();

    glDisable(GL_DEPTH_TEST);

    textureManager.BindTexture(bgRef, 0);
    textureManager.BindTexture(-1, 1);
    textureProgram->Use();
    textureProgram->Setup();
    // bg is four times as large as a tile.
    // Change the src coords based on camera to give a parallax vibe.
    geometryManager.DrawSubTexture(camera.center().x / 16,
                                   camera.center().y / 16,
                                   camera.half_size().x / 2,
                                   camera.half_size().y / 2,
                                   -1, -1, 2, 2);

    textureManager.BindTexture(tileSetRef, 0);
    textureManager.BindTexture(tileMapRef, 1);
    tileProgram->Use();
    tileProgram->map_offset(camera.center() - camera.half_size());
    tileProgram->Setup();
    geometryManager.DrawTileMap(camera);

    if (debug) {
      textureManager.BindTexture(collisionSetRef, 0);
      textureManager.BindTexture(collisionMapRef, 1);
      tileProgram->Use();
      tileProgram->map_offset(camera.center() - camera.half_size());
      tileProgram->Setup();
      geometryManager.DrawTileMap(camera);
    }

    textureManager.BindTexture(dogRef, 0);
    textureManager.BindTexture(-1, 1);
    textureProgram->Use();
    textureProgram->Setup();

    if (debug) {
      bb_graphics.Update(0 /* unused */, camera, bogs);
    }
    ss_graphics.Update(0 /* unused */, camera, bogs);

    textureManager.BindTexture(fontRef, 0);
    textureManager.BindTexture(-1, 1);
    textProgram->Use();
    textProgram->offset({-0.5,0.3});
    textProgram->scale(2.0/15.0);
    textProgram->Setup();

    text->Draw();

    glEnable(GL_DEPTH_TEST);

    display.Swap();

    if (frames % 100 == 0) {
      cout << "Graphics/s: " << (float)frames / t << endl;
    }
  }

  return 0;
}
