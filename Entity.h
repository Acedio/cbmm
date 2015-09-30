#ifndef ENTITY_H
#define ENTITY_H

#include <cassert>
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
  template <typename T1, typename T2>
  bool GetComponents(T1** t1, T2** t2) const;
  template <typename T1, typename T2, typename T3>
  bool GetComponents(T1** t1, T2** t2, T3** t3) const;
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

template <typename T1, typename T2>
bool Entity::GetComponents(T1** t1, T2** t2) const {
  assert(t1);
  assert(t2);
  *t1 = GetComponent<T1>();
  *t2 = GetComponent<T2>();
  return *t1 && *t2;
}
template <typename T1, typename T2, typename T3>
bool Entity::GetComponents(T1** t1, T2** t2, T3** t3) const {
  assert(t1);
  assert(t2);
  assert(t3);
  *t1 = GetComponent<T1>();
  *t2 = GetComponent<T2>();
  *t3 = GetComponent<T3>();
  return *t1 && *t2 && *t3;
}
#endif  // ENTITY_H
