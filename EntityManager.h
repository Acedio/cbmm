#ifndef MANAGER_H
#define MANAGER_H

#include <map>
#include <memory>

#include "Entity.h"

class EntityManager {
 public:
  EntityId CreateEntity();

 private:
  int last_id = 0;
};

#endif  // MANAGER_H
