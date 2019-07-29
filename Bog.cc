#include "Bog.h"

#include <iostream>

#include "Animation.h"
#include "Entity.h"
#include "State.h"
#include "TextureManager.h"

// TODO: Pull out common update code into a utility function.

namespace {
class Standing : public StateBehavior<JumpStateComponent> {
 public:
  void Enter(JumpStateComponent* state_component, const Entity*) const override {
    state_component->time_since_map_collision(0);
  }

  void Exit(JumpStateComponent*, const Entity*) const override {
  }

  JumpState Update(JumpStateComponent* state_component, const Entity* entity,
                   const Seconds dt) const override {
    state_component->time_since_map_collision(
        state_component->time_since_map_collision() + dt);
    // movement
    Body* body = entity->GetComponent<Body>();
    assert(body);
    Body new_body = *body;
    new_body.vel = body->vel - vec2f{0, 20} * dt;

    *body = new_body;

    // This is the time that Bog can fall (e.g. no collision with map) before he
    // can no longer jump. 0.2 feels about right and also has a bonus of being a
    // bad hack to make Bog be able to jump when he's going down slopes.
    if (state_component->time_since_map_collision() > 0.2) {
      return JumpState::FALLING;
    }
    return state();
  }

  JumpState HandleInput(JumpStateComponent*, const Entity* entity,
                        const ButtonEvent* event) const override {
    Body* body = entity->GetComponent<Body>();
    if (event->button() == Button::JUMP &&
        event->button_state() == ButtonState::PRESSED) {
      return JumpState::JUMPING;
    } else if (event->button() == Button::LEFT &&
               event->button_state() == ButtonState::PRESSED) {
      // TODO: Hmm... How to deal with people pressing left while holding right?
      // How do we know how to stop moving when a button is released?
      body->vel.x = -1.0;
    } else if (event->button() == Button::RIGHT &&
               event->button_state() == ButtonState::PRESSED) {
      body->vel.x = 1.0;
    }
    return state();
  }

  JumpState HandleCollision(JumpStateComponent* state_component,
                            const Entity* entity,
                            const CollisionEvent* collision) const override {
    if (collision->second == MAP_BODY_ID) {
      state_component->time_since_map_collision(0);
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

  JumpState state() const override { return JumpState::STANDING; }
};

class Falling : public StateBehavior<JumpStateComponent> {
 public:
  void Enter(JumpStateComponent*, const Entity*) const override {
  }
  void Exit(JumpStateComponent*, const Entity*) const override {
  }
  JumpState Update(JumpStateComponent*, const Entity* entity,
                   const Seconds dt) const override {
    // movement
    Body* body = entity->GetComponent<Body>();
    assert(body);
    Body new_body = *body;
    new_body.vel = body->vel - vec2f{0, 20} * dt;

    *body = new_body;

    return state();
  }
  JumpState HandleInput(JumpStateComponent*, const Entity*,
                        const ButtonEvent*) const override {
    return state();
  }
  JumpState HandleCollision(JumpStateComponent*, const Entity* entity,
                            const CollisionEvent* collision) const override {
    if (collision->second == MAP_BODY_ID) {
      Body* body = entity->GetComponent<Body>();
      assert(body);
      Body new_body = *body;
      new_body.bbox.lowerLeft += collision->fix;
      if (collision->fix.x != 0) new_body.vel.x = 0;
      if (collision->fix.y != 0) new_body.vel.y = 0;
      *body = new_body;
      if (collision->fix.y > 0) {
        return JumpState::STANDING;
      }
    }
    return state();
  }
  JumpState state() const override { return JumpState::FALLING; }
};

class Jumping : public StateBehavior<JumpStateComponent> {
 public:
  void Enter(JumpStateComponent*, const Entity* entity) const override {
    Body* body = entity->GetComponent<Body>();
    assert(body);
    body->vel.y = 6;
  }
  void Exit(JumpStateComponent*, const Entity* entity) const override {
    Body* body = entity->GetComponent<Body>();
    assert(body);
    body->vel.y = 5;
  }
  JumpState Update(JumpStateComponent* state_component, const Entity* entity,
                   const Seconds dt) const override {
    if (state_component->time() > 0.3) {
      return JumpState::FALLING;
    }
    // movement
    Body* body = entity->GetComponent<Body>();
    assert(body);
    Body new_body = *body;
    *body = new_body;

    return state();
  }
  JumpState HandleInput(JumpStateComponent*, const Entity*,
                        const ButtonEvent* button_event) const override {
    if (button_event->button() == Button::JUMP &&
        button_event->button_state() == ButtonState::RELEASED) {
      return JumpState::FALLING;
    }
    return state();
  }
  JumpState HandleCollision(JumpStateComponent*, const Entity* entity,
                            const CollisionEvent* collision) const override {
    if (collision->second == MAP_BODY_ID) {
      Body* body = entity->GetComponent<Body>();
      assert(body);
      Body new_body = *body;
      new_body.bbox.lowerLeft += collision->fix;
      if (collision->fix.x != 0) new_body.vel.x = 0;
      if (collision->fix.y != 0) new_body.vel.y = 0;
      *body = new_body;
      if (collision->fix.y < 0) {
        return JumpState::FALLING;
      }
    }
    return state();
  }
  JumpState state() const override { return JumpState::JUMPING; }
};
}  // namespace

std::unique_ptr<StateMachineSystem<JumpStateComponent>> MakeJumpStateSystem() {
  std::unique_ptr<StateMachineSystem<JumpStateComponent>> system(
      new StateMachineSystem<JumpStateComponent>());
  system->RegisterStateBehavior(std::unique_ptr<Standing>(new Standing()));
  system->RegisterStateBehavior(std::unique_ptr<Jumping>(new Jumping()));
  system->RegisterStateBehavior(std::unique_ptr<Falling>(new Falling()));

  return system;
}

namespace {
class Left : public StateBehavior<LRStateComponent> {
 public:
  void Enter(LRStateComponent*, const Entity* entity) const override {
    AnimationComponent* animation;
    Sprite* sprite;
    bool exists = entity->GetComponents(&sprite, &animation);
    assert(exists);
    sprite->orientation = Orientation::FLIPPED_H;
    animation->key = 0;
    animation->t = 0;
  }

  void Exit(LRStateComponent*, const Entity*) const override {
  }

  LRState Update(LRStateComponent*, const Entity* entity,
                 const Seconds dt) const override {
    // movement
    Body* body = entity->GetComponent<Body>();
    assert(body);
    Body new_body = *body;
    new_body.vel.x -= 16.0 * dt;
    new_body.vel.x = std::max(new_body.vel.x, -4.0);

    *body = new_body;

    return state();
  }

  LRState HandleInput(LRStateComponent*, const Entity*,
                      const ButtonEvent* event) const override {
    if (event->button() == Button::LEFT &&
        event->button_state() == ButtonState::RELEASED) {
      return LRState::STILL;
    } else if (event->button() == Button::RIGHT &&
               event->button_state() == ButtonState::PRESSED) {
      return LRState::RIGHT;
    }
    return state();
  }

  LRState state() const override { return LRState::LEFT; }
};

class Right : public StateBehavior<LRStateComponent> {
 public:
  void Enter(LRStateComponent*, const Entity* entity) const override {
    AnimationComponent* animation;
    Sprite* sprite;
    bool exists = entity->GetComponents(&sprite, &animation);
    assert(exists);
    sprite->orientation = Orientation::NORMAL;
    animation->key = 0;
    animation->t = 0;
  }

  void Exit(LRStateComponent*, const Entity*) const override {
  }

  LRState Update(LRStateComponent*, const Entity* entity,
                 const Seconds dt) const override {
    // movement
    Body* body = entity->GetComponent<Body>();
    assert(body);
    Body new_body = *body;
    new_body.vel.x += 16.0 * dt;
    new_body.vel.x = std::min(new_body.vel.x, 4.0);

    *body = new_body;

    return state();
  }

  LRState HandleInput(LRStateComponent*, const Entity*,
                      const ButtonEvent* event) const override {
    if (event->button() == Button::RIGHT &&
        event->button_state() == ButtonState::RELEASED) {
      return LRState::STILL;
    } else if (event->button() == Button::LEFT &&
               event->button_state() == ButtonState::PRESSED) {
      return LRState::LEFT;
    }
    return state();
  }

  LRState state() const override { return LRState::RIGHT; }
};

class Still : public StateBehavior<LRStateComponent> {
 public:
  void Enter(LRStateComponent*, const Entity* entity) const override {
    AnimationComponent* animation;
    Body* body;
    bool exists = entity->GetComponents(&body, &animation);
    assert(exists);
    animation->key = 1;
    animation->t = 0;
    body->vel.x = 0;
  }
  void Exit(LRStateComponent*, const Entity*) const override {
  }
  LRState HandleInput(LRStateComponent*, const Entity*,
                      const ButtonEvent* event) const override {
    if (event->button() == Button::RIGHT &&
        event->button_state() == ButtonState::PRESSED) {
      return LRState::RIGHT;
    } else if (event->button() == Button::LEFT &&
               event->button_state() == ButtonState::PRESSED) {
      return LRState::LEFT;
    }
    return state();
  }
  LRState state() const override { return LRState::STILL; }
};
}  // namespace

std::unique_ptr<StateMachineSystem<LRStateComponent>> MakeLRStateSystem() {
  std::unique_ptr<StateMachineSystem<LRStateComponent>> system(
      new StateMachineSystem<LRStateComponent>());
  system->RegisterStateBehavior(std::unique_ptr<Left>(new Left()));
  system->RegisterStateBehavior(std::unique_ptr<Right>(new Right()));
  system->RegisterStateBehavior(std::unique_ptr<Still>(new Still()));

  return system;
}
