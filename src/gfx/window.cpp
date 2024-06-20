#include "window.hpp"

bool Window::init(SDL_Window *&window)
{
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    success = false;
  }
  else
  {
    window = SDL_CreateWindow(GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
      std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      success = false;
    }
  }

  return success;
}

void Window::close(SDL_Window *&window)
{
  SDL_DestroyWindow(window);
  window = nullptr;
  SDL_Quit();
}