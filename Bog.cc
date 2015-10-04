#include "Bog.h"
#include "Entity.h"
#include "State.h"

#include <iostream>
using namespace std;

void MakeBog(Entity* entity, const Body& body) {
  entity->AddComponent(std::unique_ptr<Body>(new Body(body)));
  //entity->AddComponent(std::unique_ptr<StateMachine>(new StateMachine(&bog_states::Standing::state)));
}

namespace bog_states {
void Standing::Enter(Body*) const {
  //cout << "Enter Standing" << endl;
}
void Standing::Exit(Body*) const {
  //cout << "Exit Standing" << endl;
}
const State* Standing::Update(Body* body, const Seconds dt) const {
  // movement
  Body new_body = *body;
  new_body.vel = body->vel - vec2f{0, 9} * dt;
  new_body.bbox.lowerLeft = new_body.bbox.lowerLeft + body->vel * dt;

  *body = new_body;

  return nullptr;
}
const State* Standing::HandleInput(const ButtonEvent* event) const {
  if (event->button() == Button::JUMP &&
      event->button_state() == ButtonState::PRESSED) {
    return &Jumping::state;
  }
  return nullptr;
}
const State* Standing::HandleCollision(Body* body,
                                       const CollisionEvent* collision) const {
  if (collision->second == MAP_BODY_ID) {
    Body new_body = *body;
    new_body.bbox.lowerLeft += collision->fix;
    if (collision->fix.x != 0) new_body.vel.x = 0;
    if (collision->fix.y != 0) new_body.vel.y = 0;
    *body = new_body;
  }
  return nullptr;
}
const Standing Standing::state = Standing();

void Jumping::Enter(Body* body) const {
  cout << "Enter Jumping" << endl;
  body->vel.y = 5;
}
void Jumping::Exit(Body*) const {
  cout << "Exit Jumping" << endl;
}
const State* Jumping::Update(Body* body, const Seconds dt) const {
  // movement
  Body new_body = *body;
  new_body.vel = body->vel - vec2f{0, 9} * dt;
  new_body.bbox.lowerLeft = new_body.bbox.lowerLeft + body->vel * dt;

  *body = new_body;

  return nullptr;
}
const State* Jumping::HandleInput(const ButtonEvent*) const {
  return nullptr;
}
const State* Jumping::HandleCollision(Body*,
                                      const CollisionEvent* collision) const {
  if (collision->second == MAP_BODY_ID) {
    return &Standing::state;
  }
  return nullptr;
}
const Jumping Jumping::state = Jumping();
}  // bog_states
