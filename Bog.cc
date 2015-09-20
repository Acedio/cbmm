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
void Standing::Enter() const {
  cout << "Enter Standing" << endl;
}
void Standing::Exit() const {
  cout << "Exit Standing" << endl;
}
const State* Standing::Update(Body* body, const Seconds dt) const {
  // movement
  Body new_body = *body;
  new_body.vel = body->vel - vec2f{0, 9} * dt;
  new_body.bbox.upperLeft = new_body.bbox.upperLeft + body->vel * dt;

  *body = new_body;

  return nullptr;
}
const State* Standing::HandleInput(const Button button,
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

void Jumping::Enter() const {
  cout << "Enter Jumping" << endl;
}
void Jumping::Exit() const {
  cout << "Exit Jumping" << endl;
}
const State* Jumping::Update(Body* body, const Seconds dt) const {
  // movement
  Body new_body = *body;
  new_body.vel = body->vel - vec2f{0, 9} * dt;
  new_body.bbox.upperLeft = new_body.bbox.upperLeft + body->vel * dt;

  *body = new_body;

  return nullptr;
}
const State* Jumping::HandleInput(const Button, const ButtonState) const {
  return nullptr;
}
const State* Jumping::HandleCollision(Body*, const Collision&) const {
  return &Standing::state;
}
const Jumping Jumping::state = Jumping();
}  // bog_states
