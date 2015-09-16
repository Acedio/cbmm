#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"

enum class ComponentType {
  PHYSICS,
};

class Component {
 public:
  Component(Entity id) { id_ = id; }
  Entity id() { return id_; }
  virtual ComponentType type() = 0;
 private:
  Entity id_;
};

#endif  // COMPONENT_H
