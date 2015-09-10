#include "State.h"

void StateMachine::Update(Entity* entity, const Seconds dt) {
  const State* new_state = state_->Update(entity, dt);
  if (new_state) {
    state_ = new_state;
  }
}

void StateMachine::HandleInput(Entity* entity, const Button button, const ButtonState button_state) {
  const State* new_state = state_->HandleInput(entity, button, button_state);
  if (new_state) {
    state_ = new_state;
  }
}

void StateMachine::HandleCollision(Entity* entity, const Collision& collision) {
  const State* new_state = state_->HandleCollision(entity, collision);
  if (new_state) {
    state_ = new_state;
  }
}

StateMachine::StateMachine(const State* initial_state) {
  state_ = initial_state;
}
