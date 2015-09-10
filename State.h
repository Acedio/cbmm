#ifndef STATE_H
#define STATE_H

#include "Entity.h"
#include "Input.h"

class State {
 public:
  // Update handles physics movement and other such things
  virtual const State* Update(Entity* entity, const Seconds dt) const = 0;
  // Handles button presses and releases.
  virtual const State* HandleInput(Entity* entity, const Button button,
                                   const ButtonState button_state) const = 0;
  // Handles collision with ground and other objects.
  virtual const State* HandleCollision(Entity* entity,
                                       const Collision& collision) const = 0;
};

class StateMachine {
 public:
  void Update(Entity* entity, const Seconds dt);
  void HandleInput(Entity* entity, const Button button,
                   const ButtonState button_state);
  // This and Update should probably be part of a "PhysicsComponent" class.
  // There should be an easy way of describing and sharing physical behaviors.
  // -- Or maybe they should just interact with the PhysicsComponent?
  void HandleCollision(Entity* entity, const Collision& collision);
  StateMachine(const State* initial_state);
 private:
  const State* state_;
};

#endif  // STATE_H
