#ifndef BOG_H
#define BOG_H

#include "Input.h"
#include "Physics.h"

namespace bog_states {
class State;
}  // bog_states

class Bog {
 public:
  static const bog_states::State* kStanding;
  static const bog_states::State* kJumping;

  void Update(const Seconds dt);
  void HandleInput(const Button button, const ButtonState button_state);
  // This and Update should probably be part of a "PhysicsComponent" class.
  // There should be an easy way of describing and sharing physical behaviors.
  void HandleCollision(const Collision& collision);
 private:
  const bog_states::State* state;
  BodyId body_id_;
};

namespace bog_states {
class State {
 public:
  // Update handles physics movement and other such things
  virtual const State* Update(Bog* bog, const Seconds dt) const = 0;
  // Handles button presses and releases.
  virtual const State* HandleInput(Bog* bog, const Button button,
                                   const ButtonState button_state) const = 0;
  // Handles collision with ground and other objects.
  virtual const State* HandleCollision(Bog* bog,
                                       const Collision& collision) const = 0;
};

class Standing : public State {
 public:
  const State* Update(Bog* bog, const Seconds dt) const override;
  const State* HandleInput(Bog* bog, const Button button,
                           const ButtonState button_state) const override;
  const State* HandleCollision(Bog* bog,
                               const Collision& collision) const override;
};

class Jumping : public State {
 public:
  const State* Update(Bog* bog, const Seconds dt) const override;
  const State* HandleInput(Bog* bog, const Button button,
                           const ButtonState button_state) const override;
  const State* HandleCollision(Bog* bog,
                               const Collision& collision) const override;
};
}  // bog_states

#endif  // BOG_H
