#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "Component.h"
#include "EnumHashMap.h"

typedef int EntityId;

typedef EnumHashMap<ComponentType, std::unique_ptr<Component>> ComponentMap;

class Entity {
 public:
  Entity(EntityId id);
  bool AddComponent(std::unique_ptr<Component> component);
  template <typename T>
  T* GetComponent() const;
  void RemoveComponent(ComponentType type);
 private:
  EntityId id_;
  ComponentMap components_;
};

// Template methods

template <typename T>
T* Entity::GetComponent() const {
  // Gross way of getting at the type: keeping a static instance of the class :P
  static T instance;
  const auto component_pair = components_.find(instance.type());
  if (component_pair == components_.end()) {
    return nullptr;
  }
  return static_cast<T*>(component_pair->second.get());
}

#endif  // ENTITY_H
