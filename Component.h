#ifndef COMPONENT_H
#define COMPONENT_H

enum class ComponentType {
  UNKNOWN,
  BODY,
  MOVE_STATE,
  SPRITE,
};

class Component {
 public:
  virtual ComponentType type() const = 0;
  virtual ~Component() {}
};

#endif  // COMPONENT_H
