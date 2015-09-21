#include "State.h"

std::vector<std::unique_ptr<Event>> StateMachineSystem::Update(
    Seconds dt, const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    Body* body = entity.GetComponent<Body>();
    StateComponent* state_component = entity.GetComponent<StateComponent>();
    if (body && state_component) {
      const State* new_state = state_component->state()->Update(body, dt);
      HandleTransition(body, state_component, new_state);
    }
  }
  return {};
}

std::vector<std::unique_ptr<Event>> StateMachineSystem::HandleEvent(
    const Event* event, const std::vector<Entity>& entities) {
  if (event->type() == EventType::INPUT) {
    for (const auto& entity : entities) {
      StateComponent* state;
      Body* body;
      if (entity.GetComponents(&state, &body)) {
        const State* new_state =
            state->state()->HandleInput(static_cast<const ButtonEvent*>(event));
        HandleTransition(body, state, new_state);
      }
    }
  } else if (event->type() == EventType::COLLISION) {
    auto* collision = static_cast<const CollisionEvent*>(event);
    if (collision->first < (int)entities.size()) {
      StateComponent* state;
      Body* body;
      if (entities[collision->first].GetComponents(&state, &body)) {
        const State* new_state =
            state->state()->HandleCollision(body, collision);
        HandleTransition(body, state, new_state);
      }
    }
  }

  return {};
}

void StateMachineSystem::HandleTransition(Body* body, StateComponent* state,
                                          const State* new_state) {
  if (new_state) {
    state->state()->Exit(body);
    state->state(new_state);
    state->state()->Enter(body);
  }
}
