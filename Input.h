#ifndef INPUT_H
#define INPUT_H

#include <vector>

#include <SDL.h>

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

struct ButtonEvent {
  Button button;
  ButtonState button_state;
};

std::vector<ButtonEvent> GetButtonEvents();

#endif  // INPUT_H
