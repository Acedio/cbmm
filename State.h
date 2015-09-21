#ifndef STATE_H
#define STATE_H

#include "Component.h"
#include "Input.h"
#include "Physics.h"
#include "System.h"

class State {
 public:
  virtual void Enter(Body*) const {};
  virtual void Exit(Body*) const {};
  // Update handles physics movement and other such things
  virtual const State* Update(Body* body, const Seconds dt) const = 0;
  // Handles button presses and releases.
  virtual const State* HandleInput(const ButtonEvent* event) const = 0;
  // Handles collision with ground and other objects.
  virtual const State* HandleCollision(Body* body,
                                       const CollisionEvent* event) const = 0;
};

class StateComponent : public Component {
 public:
  StateComponent() {}
  StateComponent(const State* state) : state_(state) {}
  const State* state() { return state_; }
  void state(const State* state) { state_ = state; }

  ComponentType type() const override { return ComponentType::MOVE_STATE; };
 private:
  const State* state_ = nullptr;
};

// TODO: Make this less Body-specific.
class StateMachineSystem : public System {
 public:
  std::vector<std::unique_ptr<Event>> Update(
      Seconds dt, const std::vector<Entity>& entities) override;
  std::vector<std::unique_ptr<Event>> HandleEvent(
      const Event* event, const std::vector<Entity>& entities) override;

 private:
  void HandleTransition(Body* body, StateComponent* state,
                        const State* new_state);
};

#endif  // STATE_H
