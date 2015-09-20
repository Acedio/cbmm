#ifndef EVENT_H
#define EVENT_H

#include "Entity.h"
#include "Geometry.h"

enum class EventType { COLLISION, INPUT };

class Event {
 public:
  virtual EventType type() const = 0;
};

#endif  // EVENT_H
