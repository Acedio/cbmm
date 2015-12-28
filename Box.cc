#include "Box.h"

namespace {
class AtRest : public StateBehavior<BoxStateComponent> {
 public:
  BoxState Update(BoxStateComponent*, const Entity* entity,
                   const Seconds dt) const override {
    // movement
    Body* body = entity->GetComponent<Body>();
    assert(body);
    Body new_body = *body;
    new_body.vel = body->vel - vec2f{0, 20} * dt;

    *body = new_body;

    return state();
  }

  BoxState HandleCollision(BoxStateComponent*,
                            const Entity* entity,
                            const CollisionEvent* collision) const override {
    if (collision->second == MAP_BODY_ID) {
      Body* body = entity->GetComponent<Body>();
      assert(body);
      Body new_body = *body;
      new_body.bbox.lowerLeft += collision->fix;
      if (collision->fix.x != 0) new_body.vel.x = 0;
      if (collision->fix.y != 0) new_body.vel.y = 0;
      *body = new_body;
    }
    return state();
  }

  BoxState state() const override { return BoxState::AT_REST; }
};

class Falling : public AtRest {
  BoxState state() const override { return BoxState::FALLING; }
};
}  // namespace

std::unique_ptr<StateMachineSystem<BoxStateComponent>> MakeBoxStateSystem() {
  std::unique_ptr<StateMachineSystem<BoxStateComponent>> system(
      new StateMachineSystem<BoxStateComponent>());
  system->RegisterStateBehavior(std::unique_ptr<AtRest>(new AtRest()));
  system->RegisterStateBehavior(std::unique_ptr<Falling>(new Falling()));

  return system;
}
