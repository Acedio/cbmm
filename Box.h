#ifndef BOX_H
#define BOX_H

#include <memory>

#include "Entity.h"
#include "Physics.h"
#include "State.h"

enum class BoxState {
  UNKNOWN,
  AT_REST,
  FALLING,
};

inline std::string ToString(BoxState state) {
  switch (state) {
    CASE(BoxState::AT_REST);
    CASE(BoxState::FALLING);
    default:
    CASE(BoxState::UNKNOWN);
  }
}

class BoxStateComponent : public StateComponent<BoxState> {
  public:
    BoxStateComponent(BoxState initial) : StateComponent<BoxState>(initial) {}
    BoxStateComponent() : BoxStateComponent(BoxState::UNKNOWN) {}
    ComponentType type() const override { return ComponentType::BOX_STATE; }
};

std::unique_ptr<StateMachineSystem<BoxStateComponent>> MakeBoxStateSystem();

#endif  // BOX_H
