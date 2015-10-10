#include "Bog.h"
#include "Entity.h"
#include "State.h"

#include <iostream>
using namespace std;

namespace bog_states {
void Standing::Enter(const Entity*) const {
  //cout << "Enter Standing" << endl;
}
void Standing::Exit(const Entity*) const {
  //cout << "Exit Standing" << endl;
}
const State* Standing::Update(const Entity* entity, const Seconds dt) const {
  // movement
  Body* body = entity->GetComponent<Body>();
  assert(body);
  Body new_body = *body;
  new_body.vel = body->vel - vec2f{0, 9} * dt;
  new_body.bbox.lowerLeft = new_body.bbox.lowerLeft + body->vel * dt;

  *body = new_body;

  return nullptr;
}
const State* Standing::HandleInput(const Entity* entity,
                                   const ButtonEvent* event) const {
  Body* body = entity->GetComponent<Body>();
  if (event->button() == Button::JUMP &&
      event->button_state() == ButtonState::PRESSED) {
    return &Jumping::state;
  } else if (event->button() == Button::LEFT &&
             event->button_state() == ButtonState::PRESSED) {
    // TODO: Hmm... How to deal with people pressing left while holding right?
    // How do we know how to stop moving when a button is released?
    body->vel.x = -1.0;
  } else if (event->button() == Button::RIGHT &&
             event->button_state() == ButtonState::PRESSED) {
    body->vel.x = 1.0;
  }
  return nullptr;
}
const State* Standing::HandleCollision(const Entity* entity,
                                       const CollisionEvent* collision) const {
  if (collision->second == MAP_BODY_ID) {
    Body* body = entity->GetComponent<Body>();
    assert(body);
    Body new_body = *body;
    new_body.bbox.lowerLeft += collision->fix;
    if (collision->fix.x != 0) new_body.vel.x = 0;
    if (collision->fix.y != 0) new_body.vel.y = 0;
    *body = new_body;
  }
  return nullptr;
}
const Standing Standing::state = Standing();

void Jumping::Enter(const Entity* entity) const {
  cout << "Enter Jumping" << endl;
  Body* body = entity->GetComponent<Body>();
  assert(body);
  body->vel.y = 5;
}
void Jumping::Exit(const Entity*) const {
  cout << "Exit Jumping" << endl;
}
const State* Jumping::Update(const Entity* entity, const Seconds dt) const {
  // movement
  Body* body = entity->GetComponent<Body>();
  assert(body);
  Body new_body = *body;
  new_body.vel = body->vel - vec2f{0, 9} * dt;
  new_body.bbox.lowerLeft = new_body.bbox.lowerLeft + body->vel * dt;

  *body = new_body;

  return nullptr;
}
const State* Jumping::HandleInput(const Entity*, const ButtonEvent*) const {
  return nullptr;
}
const State* Jumping::HandleCollision(const Entity*,
                                      const CollisionEvent* collision) const {
  if (collision->second == MAP_BODY_ID) {
    return &Standing::state;
  }
  return nullptr;
}
const Jumping Jumping::state = Jumping();
}  // bog_states
