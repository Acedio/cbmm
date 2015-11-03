#ifndef BOG_H
#define BOG_H

#include <memory>

#include "Entity.h"
#include "Input.h"
#include "Physics.h"
#include "State.h"

enum class JumpState {
  UNKNOWN,
  STANDING,
  JUMPING,
  FALLING,
};

// TODO: Probably put these in the .cc file.
inline string ToString(JumpState state) {
  switch (state) {
    CASE(JumpState::STANDING);
    CASE(JumpState::JUMPING);
    CASE(JumpState::FALLING);
    default:
    CASE(JumpState::UNKNOWN);
  }
}

class JumpStateComponent : public StateComponent<JumpState> {
 public:
  JumpStateComponent(JumpState state) : StateComponent<JumpState>(state) {}
  JumpStateComponent() : JumpStateComponent(JumpState::UNKNOWN) {}
  ComponentType type() const override { return ComponentType::JUMP_STATE; }
  Seconds time_since_map_collision() { return time_since_map_collision_; }
  void time_since_map_collision(Seconds new_time) {
    time_since_map_collision_ = new_time;
  }
 private:
  Seconds time_since_map_collision_ = 0;
};

enum class LRState {
  UNKNOWN,
  STILL,
  LEFT,
  RIGHT,
};

inline string ToString(LRState state) {
  switch (state) {
    CASE(LRState::STILL);
    CASE(LRState::LEFT);
    CASE(LRState::RIGHT);
    default:
    CASE(LRState::UNKNOWN);
  }
}

class LRStateComponent : public StateComponent<LRState> {
 public:
  LRStateComponent(LRState state) : StateComponent<LRState>(state) {}
  LRStateComponent() : LRStateComponent(LRState::UNKNOWN) {}
  ComponentType type() const override { return ComponentType::LR_STATE; }
};

std::unique_ptr<StateMachineSystem<JumpStateComponent>> MakeJumpStateSystem();
std::unique_ptr<StateMachineSystem<LRStateComponent>> MakeLRStateSystem();

#endif  // BOG_H
