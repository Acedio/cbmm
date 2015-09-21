#ifndef BOG_H
#define BOG_H

#include <memory>

#include "Entity.h"
#include "Input.h"
#include "Physics.h"
#include "State.h"

void MakeBog(Entity* entity, const Body& body);

namespace bog_states {
class Standing : public State {
 public:
  void Enter(Body* body) const override;
  void Exit(Body* body) const override;
  const State* Update(Body* body, const Seconds dt) const override;
  const State* HandleInput(const ButtonEvent* event) const override;
  const State* HandleCollision(Body* body,
                               const CollisionEvent* event) const override;
  static const Standing state;
};

class Jumping : public State {
 public:
  void Enter(Body* body) const override;
  void Exit(Body* body) const override;
  const State* Update(Body* body, const Seconds dt) const override;
  const State* HandleInput(const ButtonEvent* event) const override;
  const State* HandleCollision(Body* body,
                               const CollisionEvent* event) const override;
  static const Jumping state;
};
}  // bog_states

#endif  // BOG_H
