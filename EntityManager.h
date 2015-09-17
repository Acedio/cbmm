#ifndef MANAGER_H
#define MANAGER_H

#include <map>
#include <memory>

#include "Component.h"
#include "Entity.h"

template <typename T>
using ComponentMap = std::map<Entity, std::unique_ptr<T>>;

class EntityManager {
 public:
  Entity CreateEntity();
  bool AddComponent(Entity id, std::unique_ptr<Component> component);
  void RemoveEntity(Entity id);
  const ComponentMap* GetComponentsOfType(ComponentType type);

 private:
  std::map<ComponentType, std::map<Entity, std::unique_ptr<Component>>> components_;
  int last_id = 0;
};

#endif  // MANAGER_H
