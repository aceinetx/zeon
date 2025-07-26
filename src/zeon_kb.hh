#pragma once
#include <SDL3/SDL.h>

namespace z {
int getKeyboardModifiers(uint16_t const mod);
int getWindowsKeyCode(SDL_KeyboardEvent const& key);
} // namespace z
