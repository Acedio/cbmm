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
};

class JumpStateComponent : public StateComponent<JumpState> {
 public:
  JumpStateComponent(JumpState state) : StateComponent<JumpState>(state) {}
  JumpStateComponent() : JumpStateComponent(JumpState::UNKNOWN) {}
  ComponentType type() const override { return ComponentType::JUMP_STATE; }
};

enum class LRState {
  UNKNOWN,
  STILL,
  LEFT,
  RIGHT,
};

class LRStateComponent : public StateComponent<LRState> {
 public:
  ComponentType type() const override { return ComponentType::LR_STATE; }
};

std::unique_ptr<StateMachineSystem<JumpStateComponent>> MakeJumpStateSystem();

#endif  // BOG_H
