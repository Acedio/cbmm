#ifndef BOG_H
#define BOG_H

#include <memory>

#include "Actor.h"
#include "Input.h"
#include "Physics.h"
#include "State.h"

std::unique_ptr<Entity> MakeBog(Body* body);

namespace bog_states {
class Standing : public State {
 public:
  const State* Update(Entity* entity, const Seconds dt) const override;
  const State* HandleInput(Entity* entity, const Button button,
                           const ButtonState button_state) const override;
  const State* HandleCollision(Entity* entity,
                               const Collision& collision) const override;
  static const Standing state;
};

class Jumping : public State {
 public:
  const State* Update(Entity* entity, const Seconds dt) const override;
  const State* HandleInput(Entity* entity, const Button button,
                           const ButtonState button_state) const override;
  const State* HandleCollision(Entity* entity,
                               const Collision& collision) const override;
  static const Jumping state;
};
}  // bog_states

#endif  // BOG_H
