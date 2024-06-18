#include "host.h"
#include "window.h"

extern int scene;

void host_init()
{
  // Render game scene (placeholder)
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue background
  SDL_RenderClear(renderer);

  // Render "Game Scene"
  SDL_Color textColor = {255, 255, 255, 255};
  TTF_Font *Sans = TTF_OpenFont("res/fonts/OpenSans-Regular.ttf", 24); // Load a font (you should provide your own)
  if (Sans == NULL)
  {
    printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    return;
  }

  SDL_Surface *textSurface = TTF_RenderText_Solid(Sans, "Host Scene", textColor);
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_Rect textRect;
  textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
  textRect.y = SCREEN_HEIGHT / 2 - textSurface->h / 2;
  textRect.w = textSurface->w;
  textRect.h = textSurface->h;
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
  TTF_CloseFont(Sans);
};

void host_loop()
{
  /* TODO */
}