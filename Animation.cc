#include "Animation.h"

#include <cmath>

Animation::Animation(const std::vector<Frame>& frames) {
  frames_ = frames;
  length_ = 0;
  for (const Frame& frame : frames_) {
    length_ += frame.length;
  }
}

int Animation::GetIndexAtSeconds(Seconds seconds) const {
  if (frames_.size() == 0) {
    return -1;
  }

  Seconds mod_seconds = fmod(seconds, length_);
  Seconds t = 0;
  for (const Frame& frame : frames_) {
    t += frame.length;
    if (t > mod_seconds) {
      return frame.tex_index;
    }
  }
  return -1;
}

std::vector<std::unique_ptr<Event>> AnimationSystem::Update(
    Seconds dt, const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    Sprite* sprite;
    AnimationComponent* animation;
    if (entity.GetComponents(&sprite, &animation)) {
      animation->t += dt;
      assert(animations_->size() > animation->key);
      sprite->index = 
        (*animations_)[animation->key].GetIndexAtSeconds(animation->t);
    }
  }
  return {};
}
