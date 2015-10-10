#ifndef BOG_H
#define BOG_H

#include <memory>

#include "Entity.h"
#include "Input.h"
#include "Physics.h"
#include "State.h"

namespace bog_states {
class Standing : public State {
 public:
  void Enter(const Entity* entity) const override;
  void Exit(const Entity* entity) const override;
  const State* Update(const Entity* entity, const Seconds dt) const override;
  const State* HandleInput(const Entity* entity,
                           const ButtonEvent* event) const override;
  const State* HandleCollision(const Entity* entity,
                               const CollisionEvent* event) const override;
  static const Standing state;
};

class Jumping : public State {
 public:
  void Enter(const Entity* entity) const override;
  void Exit(const Entity* entity) const override;
  const State* Update(const Entity* entity, const Seconds dt) const override;
  const State* HandleInput(const Entity* entity,
                           const ButtonEvent* event) const override;
  const State* HandleCollision(const Entity* entity,
                               const CollisionEvent* event) const override;
  static const Jumping state;
};
}  // bog_states

#endif  // BOG_H
