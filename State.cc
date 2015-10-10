#include "State.h"

std::vector<std::unique_ptr<Event>> StateMachineSystem::Update(
    Seconds dt, const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    StateComponent* state_component = entity.GetComponent<StateComponent>();
    if (state_component) {
      const State* new_state = state_component->state()->Update(&entity, dt);
      HandleTransition(&entity, state_component, new_state);
    }
  }
  return {};
}

std::vector<std::unique_ptr<Event>> StateMachineSystem::HandleEvent(
    const Event* event, const std::vector<Entity>& entities) {
  if (event->type() == EventType::INPUT) {
    for (const auto& entity : entities) {
      StateComponent* state = entity.GetComponent<StateComponent>();
      if (state) {
        const State* new_state = state->state()->HandleInput(
            &entity, static_cast<const ButtonEvent*>(event));
        HandleTransition(&entity, state, new_state);
      }
    }
  } else if (event->type() == EventType::COLLISION) {
    auto* collision = static_cast<const CollisionEvent*>(event);
    if (collision->first < (int)entities.size()) {
      const Entity* entity = &entities[collision->first];
      StateComponent* state = entity->GetComponent<StateComponent>();
      if (state) {
        const State* new_state =
            state->state()->HandleCollision(entity, collision);
        HandleTransition(entity, state, new_state);
      }
    }
  }

  return {};
}

void StateMachineSystem::HandleTransition(const Entity* entity, StateComponent* state,
                                          const State* new_state) {
  if (new_state) {
    state->state()->Exit(entity);
    state->state(new_state);
    state->state()->Enter(entity);
  }
}
