#include "window.h"
#include "scene.h"
#include <stdbool.h>

bool isRunning = true;
extern int scene;

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *Sans;
SDL_Color RED = {255, 0, 0};
SDL_Color GREEN = {0, 255, 0};
SDL_Color BLUE = {0, 0, 255};

void window_init()
{
  /* Initialize SDL */
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }
  else
  {
    printf("SDL initialized\n");
  }

  /* Initialize window, renderer etc. */
  window = SDL_CreateWindow("SDL Net Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL)
  {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(3);
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(4);
  }
}

void window_loop()
{
  SDL_Event event;
  while (isRunning)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        isRunning = false;
      }
    }

    render_scene();
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