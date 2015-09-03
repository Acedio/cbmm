#include "Input.h"

std::vector<ButtonEvent> GetButtonEvents() {
  std::vector<ButtonEvent> button_events;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        button_events.push_back({Button::QUIT, ButtonState::RELEASED});
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            button_events.push_back({Button::QUIT, ButtonState::RELEASED});
            break;
          case SDLK_p:
            button_events.push_back({Button::PAUSE, ButtonState::RELEASED});
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
  return button_events;
}
