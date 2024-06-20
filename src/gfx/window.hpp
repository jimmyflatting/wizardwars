#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include "global.hpp"

class Window
{
public:
  static bool init(SDL_Window *&window);
  static void close(SDL_Window *&window);
};

extern SDL_Window *gWindow;

#endif // WINDOW_HPP