#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 352
#define TILE_SIZE 32

SDL_Window *window;
SDL_Renderer *renderer;

typedef struct
{
  int x;
  int y;
  int w;
  int h;
} Rect;

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("Tilemap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1088, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  SDL_Rect srcRect = {0, 0, TILE_SIZE, TILE_SIZE};
  SDL_Rect destRect = {0, 0, TILE_SIZE, TILE_SIZE};

  SDL_Surface *surface = IMG_Load("../res/tileset.png");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  while (1)
  {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
    {
      break;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < SCREEN_WIDTH / TILE_SIZE; i++)
    {
      for (int j = 0; j < SCREEN_HEIGHT / TILE_SIZE; j++)
      {
        destRect.x = i * TILE_SIZE;
        destRect.y = j * TILE_SIZE;
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
      }
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}