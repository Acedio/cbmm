#ifndef STATE_H
#define STATE_H

#include "Component.h"
#include "Physics.h"
#include "Input.h"

class State : public Component {
 public:
  ComponentType type() const override { return ComponentType::MOVE_STATE; };
  virtual void Enter() const {};
  virtual void Exit() const {};
  // Update handles physics movement and other such things
  virtual const State* Update(Body* body, const Seconds dt) const = 0;
  // Handles button presses and releases.
  virtual const State* HandleInput(const Button button,
                                   const ButtonState button_state) const = 0;
  // Handles collision with ground and other objects.
  virtual const State* HandleCollision(Body* body,
                                       const Collision& collision) const = 0;
};

class StateMachine {
 public:
  void Update(Body* body, const Seconds dt);
  void HandleInput(const Button button, const ButtonState button_state);
  // This and Update should probably be part of a "PhysicsComponent" class.
  // There should be an easy way of describing and sharing physical behaviors.
  // -- Or maybe they should just interact with the PhysicsComponent?
  void HandleCollision(Body* body, const Collision& collision);
  StateMachine(const State* initial_state);
 private:
  void HandleTransition(const State* new_state);
  const State* state_;
};

#endif  // STATE_H
