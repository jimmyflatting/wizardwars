#include "window.h"

bool isRunning = true;
extern int scene;

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *Sans;
SDL_Color RED = {255, 0, 0};
SDL_Color GREEN = {0, 255, 0};
SDL_Color BLUE = {0, 0, 255};
Uint32 frame_start = 0;
Uint32 frame_time;
const int FRAME_RATE = 60;
const int FRAME_DELAY = 1000 / FRAME_RATE;

bool window_init()
{
  /* Initialize SDL */
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Initialize SDL_ttf
  if (TTF_Init() == -1)
  {
    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    return false;
  }

  /* Create window */
  window = SDL_CreateWindow("SDL Net Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL)
  {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  /* Create render for window */
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  /* Initialize renderer color */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  return true;
}

void window_update()
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_RIGHT])
  {
    scene += 1;
    if (scene > 2)
    {
      scene = 0;
    }
  }
}

void window_render()
{
  frame_start = SDL_GetTicks();
  SDL_RenderClear(renderer);

  switch (scene)
  {
  case 0:
    main_menu_init();
    break;
  case 1:
    client_init();
    break;
  case 2:
    host_init();
    break;
  }

  SDL_RenderPresent(renderer);

  // handle frame rate
  frame_time = SDL_GetTicks() - frame_start;
  if (FRAME_DELAY > frame_time)
  {
    SDL_Delay(FRAME_DELAY - frame_time);
  }
}

void window_destroy()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  window = NULL;
  renderer = NULL;

  TTF_Quit();
  SDL_Quit();
}