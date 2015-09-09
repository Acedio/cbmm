#ifndef ACTOR_H
#define ACTOR_H

#include "Physics.h"

// Actors control the core game stuff for objects. Things like physical
// properties and behavior should be encompassed within.
class Actor {
 public:
  virtual void Update(Seconds dt) = 0;

  // Probably should return events here.
  virtual void HandleCollision(const Collision& collision) = 0;
};

#endif  // ACTOR_H
