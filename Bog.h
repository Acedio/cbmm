#ifndef BOG_H
#define BOG_H

#include <memory>

#include "Actor.h"
#include "Entity.h"
#include "Input.h"
#include "Physics.h"
#include "State.h"

Entity MakeBog(Physics* physics,
               vector<unique_ptr<StateMachine>>* state_machines,
               const Body& body);

namespace bog_states {
class Standing : public State {
 public:
  void Enter() const override;
  void Exit() const override;
  const State* Update(Body* body, const Seconds dt) const override;
  const State* HandleInput(const Button button,
                           const ButtonState button_state) const override;
  const State* HandleCollision(Body* body,
                               const Collision& collision) const override;
  static const Standing state;
};

class Jumping : public State {
 public:
  void Enter() const override;
  void Exit() const override;
  const State* Update(Body* body, const Seconds dt) const override;
  const State* HandleInput(const Button button,
                           const ButtonState button_state) const override;
  const State* HandleCollision(Body* body,
                               const Collision& collision) const override;
  static const Jumping state;
};
}  // bog_states

#endif  // BOG_H
