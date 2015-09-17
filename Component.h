#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"

enum class ComponentType {
  BODY,
  MOVE_STATE,
};

class Component {
 public:
  virtual ComponentType type() const = 0;
};

#endif  // COMPONENT_H
