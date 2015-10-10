#ifndef STATE_H
#define STATE_H

#include "Component.h"
#include "Input.h"
#include "Physics.h"
#include "System.h"

class State {
 public:
  // TODO: const& should be fine here.
  virtual void Enter(const Entity*) const {};
  virtual void Exit(const Entity*) const {};
  // Update handles physics movement and other such things
  virtual const State* Update(const Entity* entity, const Seconds dt) const = 0;
  // Handles button presses and releases.
  virtual const State* HandleInput(const Entity* entity,
                                   const ButtonEvent* event) const = 0;
  // Handles collision with ground and other objects.
  virtual const State* HandleCollision(const Entity* entity,
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

class StateMachineSystem : public System {
 public:
  std::vector<std::unique_ptr<Event>> Update(
      Seconds dt, const std::vector<Entity>& entities) override;
  std::vector<std::unique_ptr<Event>> HandleEvent(
      const Event* event, const std::vector<Entity>& entities) override;

 private:
  void HandleTransition(const Entity* entity, StateComponent* state,
                        const State* new_state);
};

#endif  // STATE_H
