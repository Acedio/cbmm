#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "Physics.h"

class StateMachine;

struct Entity {
  // Not owned.
  Body* body;
  std::unique_ptr<StateMachine> state_machine;
};

#endif  // ENTITY_H
