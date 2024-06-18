#include "main_menu.h"
#include "window.h"
#include "buttons.h"
#include "scene.h"

extern int scene;
extern SDL_Renderer *renderer;

void main_menu_init()
{
  // Render game scene (placeholder)
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0); // Green background
  SDL_RenderClear(renderer);

  // Render "Game Scene"
  SDL_Color textColor = {255, 255, 255, 255};
  TTF_Font *Sans = TTF_OpenFont("res/fonts/OpenSans-Regular.ttf", 24); // Load a font (you should provide your own)
  if (Sans == NULL)
  {
    printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    return;
  }

  SDL_Surface *textSurface = TTF_RenderText_Solid(Sans, "Menu Scene", textColor);
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_Rect textRect;
  textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
  textRect.y = SCREEN_HEIGHT / 2 - textSurface->h / 2;
  textRect.w = textSurface->w;
  textRect.h = textSurface->h;
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  // render buttons for join game and host game
  Button joinButton;
  init_button(&joinButton, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, 200, 50, (SDL_Color){255, 0, 0, 255}, "Join Game", change_scene);
  render_button(&joinButton);
  handle_button_click(&joinButton, 1);

  Button hostButton;
  init_button(&hostButton, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 150, 200, 50, (SDL_Color){255, 0, 0, 255}, "Host Game", change_scene);
  render_button(&hostButton);
  handle_button_click(&hostButton, 2);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
  TTF_CloseFont(Sans);
}

void main_menu_loop()
{
  /* TODO */
}

void main_menu_render()
{
  /* TODO */
}