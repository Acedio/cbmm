#include "State.h"

void StateMachine::Update(Body* body, const Seconds dt) {
  const State* new_state = state_->Update(body, dt);
  HandleTransition(new_state);
}

void StateMachine::HandleInput(const Button button,
                               const ButtonState button_state) {
  const State* new_state = state_->HandleInput(button, button_state);
  HandleTransition(new_state);
}

void StateMachine::HandleCollision(Body* body, const Collision& collision) {
  const State* new_state = state_->HandleCollision(body, collision);
  HandleTransition(new_state);
}

StateMachine::StateMachine(const State* initial_state) {
  state_ = initial_state;
  state_->Enter();
}

void StateMachine::HandleTransition(const State* new_state) {
  if (new_state) {
    state_->Exit();
    state_ = new_state;
    state_->Enter();
  }
}
