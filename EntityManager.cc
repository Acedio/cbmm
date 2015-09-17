#include "EntityManager.h"

Entity EntityManager::CreateEntity() {
  // Increment to next id.
  return ++last_id;
}
