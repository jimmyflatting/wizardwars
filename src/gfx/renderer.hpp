#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include "global.hpp" // Make sure this is included

class Renderer
{
public:
  static bool init(SDL_Renderer *&renderer, SDL_Window *&window);
  static void close(SDL_Renderer *&renderer);
  static void update(SDL_Renderer *&renderer);
  static void render(SDL_Renderer *&renderer);
};

extern SDL_Renderer *gRenderer;
extern SDL_Window *gWindow;
extern SceneManager scene_manager;

#endif // RENDERER_HPP