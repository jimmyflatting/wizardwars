#include "renderer.hpp"
#include "global.hpp"

bool Renderer::init(SDL_Renderer *&renderer, SDL_Window *&gWindow)
{
  bool success = true;

  renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
  {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    success = false;
  }
  else
  {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  }

  return success;
}

void Renderer::close(SDL_Renderer *&renderer)
{
  SDL_DestroyRenderer(renderer);
  renderer = nullptr;
}

void Renderer::update(SDL_Renderer *&renderer)
{
  scene_manager.update();
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_Rect rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
  SDL_RenderFillRect(renderer, &rect);
}

void Renderer::render(SDL_Renderer *&renderer)
{

  // update scene
  scene_manager.render();

  SDL_RenderPresent(renderer);
}