

#include <InputManager/InputManager.hpp>

InputManager::t_keyboard InputManager::keyboard;

#ifndef OS_ANDROID
void InputManager::KeyDown(SDL_Keycode key) {
  switch (key) {
    case SDLK_SPACE:
      keyboard.space = true;
      break;
    case SDLK_LCTRL:
      keyboard.lctrl = true;
      break;
    case SDLK_RCTRL:
      keyboard.rctrl = true;
      break;
    case SDLK_LSHIFT:
      keyboard.lshift = true;
      break;
    case SDLK_RSHIFT:
      keyboard.rshift = true;
      break;
    case SDLK_UP:
      keyboard.up = true;
      break;
    case SDLK_DOWN:
      keyboard.down = true;
      break;
    case SDLK_LEFT:
      keyboard.left = true;
      break;
    case SDLK_RIGHT:
      keyboard.right = true;
      break;
    case SDLK_w:
      keyboard.w = true;
      break;
    case SDLK_a:
      keyboard.a = true;
      break;
    case SDLK_s:
      keyboard.s = true;
      break;
    case SDLK_d:
      keyboard.d = true;
      break;
  }
}

void InputManager::KeyUp(SDL_Keycode key) {
  switch (key) {
    case SDLK_SPACE:
      keyboard.space = false;
      break;
    case SDLK_LCTRL:
      keyboard.lctrl = false;
      break;
    case SDLK_RCTRL:
      keyboard.rctrl = false;
      break;
    case SDLK_LSHIFT:
      keyboard.lshift = false;
      break;
    case SDLK_RSHIFT:
      keyboard.rshift = false;
      break;
    case SDLK_UP:
      keyboard.up = false;
      break;
    case SDLK_DOWN:
      keyboard.down = false;
      break;
    case SDLK_LEFT:
      keyboard.left = false;
      break;
    case SDLK_RIGHT:
      keyboard.right = false;
      break;
    case SDLK_w:
      keyboard.w = false;
      break;
    case SDLK_a:
      keyboard.a = false;
      break;
    case SDLK_s:
      keyboard.s = false;
      break;
    case SDLK_d:
      keyboard.d = false;
      break;
  }
}
#endif // OS_ANDROID