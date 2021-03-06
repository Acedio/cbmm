#include "Input.h"

namespace {
  Button translate(int sdlk) {
    switch (sdlk) {
      case SDLK_ESCAPE:
        return Button::QUIT;
      case SDLK_d:
        return Button::DEBUG;
      case SDLK_p:
        return Button::PAUSE;
      case SDLK_z:
        return Button::JUMP;
      case SDLK_k:
        return Button::UP;
      case SDLK_j:
        return Button::DOWN;
      case SDLK_h:
        return Button::LEFT;
      case SDLK_l:
        return Button::RIGHT;
      case SDLK_EQUALS:
        return Button::PLUS;
      case SDLK_MINUS:
        return Button::MINUS;
      default:
        return Button::UNKNOWN;
    }
  }
}  // namespace

std::vector<ButtonEvent> GetButtonEvents() {
  std::vector<ButtonEvent> button_events;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      button_events.emplace_back(Button::QUIT, ButtonState::RELEASED);
    } else if ((event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) &&
               event.key.repeat == 0) {  // Ignore repeat presses.
      ButtonState state = (event.type == SDL_KEYUP ? ButtonState::RELEASED
                                                   : ButtonState::PRESSED);
      Button button = translate(event.key.keysym.sym);

      if (button != Button::UNKNOWN) {
        button_events.emplace_back(button, state);
      }
    }
  }
  return button_events;
}
