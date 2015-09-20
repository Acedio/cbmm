#ifndef INPUT_H
#define INPUT_H

#include <vector>

#include <SDL.h>

#include "Event.h"

enum class Button {
  UNKNOWN = 0,
  JUMP,
  ACTION,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  QUIT,
  PAUSE
};

enum class ButtonState {
  UNKNOWN = 0,
  PRESSED,
  RELEASED
};

class ButtonEvent : public Event {
 public:
  EventType type() const override { return EventType::INPUT; }
  ButtonEvent(Button button, ButtonState button_state)
      : button_(button), button_state_(button_state) {}
  Button button() const { return button_; }
  ButtonState button_state() const { return button_state_; }
 private:
  Button button_;
  ButtonState button_state_;
};

std::vector<ButtonEvent> GetButtonEvents();

#endif  // INPUT_H
