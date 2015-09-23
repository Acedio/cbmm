#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>

#include "Entity.h"
#include "Event.h"

typedef double Seconds;

// TODO: Split systems into ones that update and ones that handle events?
class System {
 public:
  virtual std::vector<std::unique_ptr<Event>> Update(
      Seconds, const std::vector<Entity>&) {
    return {};
  }
  // TODO: Null check on Event?
  virtual std::vector<std::unique_ptr<Event>> HandleEvent(
      const Event*, const std::vector<Entity>&) {
    return {};
  }
};

#endif  // SYSTEM_H
