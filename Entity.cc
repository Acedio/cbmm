#include "Entity.h"

Entity::Entity(EntityId id) : id_(id) {}

bool Entity::AddComponent(std::unique_ptr<Component> component) {
  ComponentType type = component->type();
  return components_.emplace(type, std::move(component)).second;
}

void Entity::RemoveComponent(ComponentType type) {
  components_.erase(type);
}
