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
  EntityId CreateEntity();

 private:
  int last_id = 0;
};

#endif  // MANAGER_H
