#include "State.h"

void StateMachine::Update(Body* body, const Seconds dt) {
  const State* new_state = state_->Update(body, dt);
  if (new_state) {
    state_ = new_state;
  }
}

void StateMachine::HandleInput(Body* body, const Button button, const ButtonState button_state) {
  const State* new_state = state_->HandleInput(body, button, button_state);
  if (new_state) {
    state_ = new_state;
  }
}

void StateMachine::HandleCollision(Body* body, const Collision& collision) {
  const State* new_state = state_->HandleCollision(body, collision);
  if (new_state) {
    state_ = new_state;
  }
}

StateMachine::StateMachine(const State* initial_state) {
  state_ = initial_state;
}
