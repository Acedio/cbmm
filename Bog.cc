#include "Bog.h"

void Bog::Update(const Seconds dt) {
  const bog_states::State* new_state = state->Update(this, dt);
  if (new_state) {
    state = new_state;
  }
}
void Bog::HandleInput(const Button button, const ButtonState button_state) {
  const bog_states::State* new_state =
      state->HandleInput(this, button, button_state);
  if (new_state) {
    state = new_state;
  }
}
void Bog::HandleCollision(const Collision& collision) {
  const bog_states::State* new_state = state->HandleCollision(this, collision);
  if (new_state) {
    state = new_state;
  }
}

namespace bog_states {
// Initialize static states.
State* State::kStanding = nullptr;
State* State::kJumping = nullptr;

const State* Standing::Update(Bog*, const Seconds) const {
  return nullptr;
}
const State* Standing::HandleInput(Bog*, const Button button,
                                   const ButtonState button_state) const {
  if (button == Button::JUMP && button_state == ButtonState::PRESSED) {
    return State::kJumping;
  }
  return nullptr;
}
const State* Standing::HandleCollision(Bog*, const Collision&) const {
  return nullptr;
}

const State* Jumping::Update(Bog*, const Seconds) const {
  return nullptr;
}
const State* Jumping::HandleInput(Bog*, const Button, const ButtonState) const {
  return nullptr;
}
const State* Jumping::HandleCollision(Bog*, const Collision&) const {
  return State::kStanding;
}
}  // bog_states
