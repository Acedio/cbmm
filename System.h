#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>

#include "Entity.h"
#include "Event.h"

typedef double Seconds;

class System {
 public:
  virtual std::vector<std::unique_ptr<Event>> Update(
      Seconds, const std::vector<Entity>&) {
    return {};
  }
  virtual std::vector<std::unique_ptr<Event>> HandleEvent(
      const Event*, const std::vector<Entity>&) {
    return {};
  }
};

#endif  // SYSTEM_H
