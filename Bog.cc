#include "Bog.h"
#include "Entity.h"
#include "State.h"

#include <iostream>
using namespace std;

Entity MakeBog(
    Physics* physics, vector<unique_ptr<StateMachine>>* state_machines,
    const Body& body /* should probably pass Physics here as well*/) {
  Entity bog = physics->AddBody(body);
  state_machines->emplace_back(new StateMachine(&bog_states::Standing::state));
  return bog;
}

namespace bog_states {
// Initialize static states.
const State* Standing::Update(Body* body, const Seconds dt) const {
  // movement
  Body new_body = *body;
  new_body.vel = body->vel - vec2f{0, 9} * dt;
  new_body.bbox.upperLeft = new_body.bbox.upperLeft + body->vel * dt;

  *body = new_body;

  return nullptr;
}
const State* Standing::HandleInput(Body*, const Button button,
                                   const ButtonState button_state) const {
  if (button == Button::JUMP && button_state == ButtonState::PRESSED) {
    return &Jumping::state;
  }
  return nullptr;
}
const State* Standing::HandleCollision(Body* body,
                                       const Collision& collision) const {
  if (collision.second == MAP_BODY_ID) {
    Body new_body = *body;
    new_body.bbox.upperLeft += collision.fix;
    if (collision.fix.x != 0) new_body.vel.x = 0;
    if (collision.fix.y != 0) new_body.vel.y = 0;
    *body = new_body;
  }
  return nullptr;
}
const Standing Standing::state = Standing();

const State* Jumping::Update(Body*, const Seconds) const {
  return nullptr;
}
const State* Jumping::HandleInput(Body*, const Button, const ButtonState) const {
  return nullptr;
}
const State* Jumping::HandleCollision(Body*, const Collision&) const {
  return &Standing::state;
}
const Jumping Jumping::state = Jumping();
}  // bog_states
