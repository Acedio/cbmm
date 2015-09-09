#include "Bog.h"

void Bog::Update(const Seconds dt) {
  const bog_states::State* new_state = state_->Update(this, dt);
  if (new_state) {
    state_ = new_state;
  }
}
void Bog::HandleInput(const Button button, const ButtonState button_state) {
  const bog_states::State* new_state =
      state_->HandleInput(this, button, button_state);
  if (new_state) {
    state_ = new_state;
  }
}
void Bog::HandleCollision(const Collision& collision) {
  const bog_states::State* new_state = state_->HandleCollision(this, collision);
  if (new_state) {
    state_ = new_state;
  }
}

std::unique_ptr<Bog> Bog::MakeBog(
    const vec2f& /* should probably pass Physics here as well*/) {
  Bog* bog = new Bog();
  bog->state_ = bog_states::State::kStanding;
  return std::unique_ptr<Bog>();
}

Bog::Bog() {
  // Nada.
}

namespace bog_states {
// Initialize static states.
Standing standing;
State* State::kStanding = &standing;
Jumping jumping;
State* State::kJumping = &jumping;

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
