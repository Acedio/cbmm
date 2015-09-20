#ifndef ENTITY_H
#define ENTITY_H

#include <unordered_map>
#include <memory>

#include "Component.h"

typedef int EntityId;

class Entity {
 public:
  Entity(EntityId id);
  bool AddComponent(std::unique_ptr<Component> component);
  template <typename T>
  T* GetComponent() const;
  void RemoveComponent(ComponentType type);
 private:
  EntityId id_;
  std::unordered_map<ComponentType, std::unique_ptr<Component>> components_;
};

// Template methods

template <typename T>
T* Entity::GetComponent() const {
  // Gross way of getting at the type: keeping a static instance of the class :P
  static T instance;
  const auto& component_pair = components_.find(instance.type());
  if (component_pair == components_.end()) {
    return nullptr;
  }
  return static_cast<T>(component_pair.second.get());
}

#endif  // ENTITY_H
