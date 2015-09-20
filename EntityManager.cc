#include "EntityManager.h"

EntityId EntityManager::CreateEntity() {
  // Increment to next id.
  return ++last_id;
}
