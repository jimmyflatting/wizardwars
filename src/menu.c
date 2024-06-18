#include "main_menu.h"

extern int scene;
extern SDL_Renderer *renderer;

void menu_init()
{
  printf("Menu initialized\n");
  SDL_Delay(1000);
}

void menu_loop()
{
  int cnt = 0;
  printf("Menu loop\n");
  while (scene == 0)
  {
    printf("Menu looping yo %d\n", cnt);
    SDL_Delay(5000);
    cnt++;
    if (cnt == 3)
    {
      scene = 1; // Transition to the host scene
    }
  }
}

void menu_render()
{
  while (scene == 0)
  {
    printf("Menu render\n");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }
}