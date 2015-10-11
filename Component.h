#ifndef COMPONENT_H
#define COMPONENT_H

enum class ComponentType {
  UNKNOWN,
  BODY,
  JUMP_STATE,
  LR_STATE,
  SPRITE,
  TRANSFORM,
};

class Component {
 public:
  virtual ComponentType type() const = 0;
  virtual ~Component() {}
};

#endif  // COMPONENT_H
