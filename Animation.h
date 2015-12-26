#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

#include "Geometry.h"
#include "System.h"
#include "TextureManager.h"

struct Frame {
  int tex_index;
  Seconds length;
};

class Animation {
  public:
    explicit Animation(const std::vector<Frame>& frames);
    Animation() {}
    int GetIndexAtSeconds(Seconds seconds) const;
  private:
    std::vector<Frame> frames_;
    Seconds length_;
};

class AnimationComponent : public Component {
  public:
    explicit AnimationComponent(int key) : key(key) {}
    AnimationComponent() : key(-1) {}
    ComponentType type() const override { return ComponentType::ANIMATION; }
    int key;
    Seconds t = 0;
};

class AnimationSystem : public System {
  public:
    // animations must outlive this.
    explicit AnimationSystem(const std::vector<Animation>* animations) :
      animations_(animations) {}
    std::vector<std::unique_ptr<Event>> Update(
        Seconds dt, const std::vector<Entity>& entities) override;

  private:
    const std::vector<Animation>* animations_;
};

#endif  // ANIMATION_H
