#ifndef COMPONENT_H
#define COMPONENT_H

enum class ComponentType {
  UNKNOWN,
  BODY,
  MOVE_STATE,
};

class Component {
 public:
  virtual ComponentType type() const = 0;
  virtual ~Component() {}
};

#endif  // COMPONENT_H
