#ifndef BOG_H
#define BOG_H

#include "Input.h"
#include "Physics.h"

namespace bog_states {
class BogState;
}  // bog_states

class Bog {
 private:
  BodyId body_id_;
};

namespace bog_states {
class State {
 public:
  virtual const State* Update(Bog* bog, const Seconds dt) = 0;
  virtual const State* HandleInput(Bog* bog, const Button button) = 0;
};

class Standing : public State {
 public:
  const State* Update(Bog* bog, const Seconds dt) override {
    return nullptr;
  }
  const State* HandleInput(Bog* bog, const Button button) override {
    return nullptr;
  }
}  // bog_states

#endif  // BOG_H
