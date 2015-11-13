#ifndef STATE_H
#define STATE_H

#include <cassert>
#include <iostream>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "Component.h"
#include "EnumHashMap.h"
#include "Input.h"
#include "Physics.h"
#include "System.h"

#define CASE(x) case x: return #x

template <typename StateEnum>
class StateComponent : public Component {
 public:
  StateComponent() {}
  StateComponent(StateEnum state) : state_(state) {}
  StateEnum state() { return state_; }
  void state(StateEnum state) { state_ = state; }
  Seconds time() { return time_; }
  void time(Seconds time) { time_ = time; }

  virtual ComponentType type() const = 0;
 private:
  StateEnum state_ = StateEnum::UNKNOWN;
  // Time spent in this state.
  Seconds time_ = 0;
};

// ComponentType should be a subclass of StateComponent.
template <typename ComponentType>
class StateBehavior {
 public:
  typedef decltype(std::declval<ComponentType>().state()) StateEnum;
  virtual void Enter(ComponentType*, const Entity*) const {};
  virtual void Exit(ComponentType*, const Entity*) const {};
  // Update handles physics movement and other such things
  virtual StateEnum Update(ComponentType* state_component, const Entity*,
                           const Seconds dt) const {
    state_component->time(state_component->time() + dt);
    return state();
  };
  // Handles button presses and releases.
  virtual StateEnum HandleInput(ComponentType*, const Entity*,
                                const ButtonEvent*) const {
    return state();
  };
  // Handles collision with ground and other objects.
  virtual StateEnum HandleCollision(ComponentType*, const Entity*,
                                    const CollisionEvent*) const {
    return state();
  };
  virtual StateEnum state() const = 0;
};

// ComponentType should be a subclass of StateComponent.
template <typename ComponentType>
class StateMachineSystem : public System {
 public:
  typedef decltype(std::declval<ComponentType>().state()) StateEnum;
  std::vector<std::unique_ptr<Event>> Update(
      Seconds dt, const std::vector<Entity>& entities) override {
    for (const auto& entity : entities) {
      ComponentType* state_component = entity.GetComponent<ComponentType>();
      if (state_component) {
        // Update the time component.
        state_component->time(state_component->time() + dt);

        auto state = state_component->state();
        const StateBehavior<ComponentType>* behavior = behaviors_[state].get();
        auto new_state = behavior->Update(state_component, &entity, dt);
        HandleTransition(&entity, state_component, new_state);
      }
    }
    return {};
  }

  // TODO: Maybe split this into two: HandleEvent (for broadcasts) and
  // HandleMessage (for directed messages).
  std::vector<std::unique_ptr<Event>> HandleEvent(
      const Event* event, const std::vector<Entity>& entities) override {
    if (event->type() == EventType::INPUT) {
      for (const auto& entity : entities) {
        ComponentType* state_component = entity.GetComponent<ComponentType>();
        if (state_component) {
          auto state = state_component->state();
          const StateBehavior<ComponentType>* behavior = behaviors_[state].get();
          auto new_state = behavior->HandleInput(
              state_component, &entity, static_cast<const ButtonEvent*>(event));
          HandleTransition(&entity, state_component, new_state);
        }
      }
    } else if (event->type() == EventType::COLLISION) {
      auto* collision = static_cast<const CollisionEvent*>(event);
      if (collision->first < (int)entities.size()) {
        const Entity& entity = entities[collision->first];
        ComponentType* state_component = entity.GetComponent<ComponentType>();
        if (state_component) {
          auto state = state_component->state();
          const StateBehavior<ComponentType>* behavior = behaviors_[state].get();
          auto new_state =
              behavior->HandleCollision(state_component, &entity, collision);
          HandleTransition(&entity, state_component, new_state);
        }
      }
    }

    return {};
  }

  StateMachineSystem() {}

  void RegisterStateBehavior(
      std::unique_ptr<StateBehavior<ComponentType>> behavior) {
    assert(behavior.get());
    StateEnum state = behavior->state();
    bool inserted = behaviors_.emplace(state, std::move(behavior)).second;
    assert(inserted);
  }

 private:
  void HandleTransition(const Entity* entity, ComponentType* state_component,
                        StateEnum new_state) {
    const StateEnum old_state = state_component->state();
    if (new_state != old_state) {
      const StateBehavior<ComponentType>* old_behavior =
          behaviors_[old_state].get();
      const StateBehavior<ComponentType>* new_behavior =
          behaviors_[new_state].get();
#ifdef DEBUG
      std::cout << "Exiting " << ToString(old_behavior->state()) << std::endl;
#endif
      old_behavior->Exit(state_component, entity);
      state_component->state(new_state);
      state_component->time(0);
#ifdef DEBUG
      std::cout << "Entering " << ToString(new_behavior->state()) << std::endl;
#endif
      new_behavior->Enter(state_component, entity);
    }
  }

  EnumHashMap<StateEnum, std::unique_ptr<StateBehavior<ComponentType>>>
      behaviors_;
};

#endif  // STATE_H
