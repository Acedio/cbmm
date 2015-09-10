#include "Bog.h"
#include "Entity.h"
#include "State.h"

#include <iostream>
using namespace std;

std::unique_ptr<Entity> MakeBog(
    Body* body/* should probably pass Physics here as well*/) {
  Entity* bog = new Entity();
  bog->body = body;
  bog->state_machine.reset(new StateMachine(&bog_states::Standing::state));
  return std::unique_ptr<Entity>(bog);
}

namespace bog_states {
// Initialize static states.
const State* Standing::Update(Entity* bog, const Seconds dt) const {
  // movement
  Body new_body = *bog->body;
  new_body.bbox.upperLeft = new_body.bbox.upperLeft + new_body.vel * dt;

  *bog->body = new_body;

  return nullptr;
}
const State* Standing::HandleInput(Entity*, const Button button,
                                   const ButtonState button_state) const {
  if (button == Button::JUMP && button_state == ButtonState::PRESSED) {
    return &Jumping::state;
  }
  return nullptr;
}
const State* Standing::HandleCollision(Entity* bog,
                                       const Collision& collision) const {
  if (collision.second == MAP_BODY_ID) {
    Body new_body = *bog->body;
    cout << new_body.bbox.upperLeft.y << " to ";
    new_body.bbox.upperLeft += collision.fix;
    cout << new_body.bbox.upperLeft.y << endl;
    *bog->body = new_body;
  }
  return nullptr;
}
const Standing Standing::state = Standing();

const State* Jumping::Update(Entity*, const Seconds) const {
  return nullptr;
}
const State* Jumping::HandleInput(Entity*, const Button, const ButtonState) const {
  return nullptr;
}
const State* Jumping::HandleCollision(Entity*, const Collision&) const {
  return &Standing::state;
}
const Jumping Jumping::state = Jumping();
}  // bog_states
