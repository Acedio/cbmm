#include "EntityManager.h"

Entity EntityManager::CreateEntity() {
  // Increment to next id.
  return ++last_id;
}

bool EntityManager::AddComponent(Entity id, std::unique_ptr<Component> component) {
  std::map<Entity, std::unique_ptr<Component>>& components_of_type =
      components_[component->type()];
  if (components_of_type.find(id) != components_of_type.end()) {
    return false;
  }

  components_of_type[id] = std::move(component);
  return true;
}

void EntityManager::RemoveEntity(Entity id) {
  for (auto component_map : components_) {
    component_map.second.erase(id);
  }
}

const std::map<Entity, std::unique_ptr<Component>>*
EntityManager::GetComponentsOfType(ComponentType type) {
  return &components_[type];
}
