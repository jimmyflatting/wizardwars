#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Include SDL_ttf.h for text rendering
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h> // For usleep function (Unix/Linux specific)

// Define your game constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Global variables
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
bool gIsRunning = true;
int gCurrentScene = 0; // 0: Main Menu, 1: Game Scene
Uint32 gSceneStartTime = 0;

// Function prototypes
bool init();
void update();
void render();
void destroy();
void changeScene(int scene);

int main(int argc, char *argv[])
{
  if (!init())
  {
    printf("Failed to initialize!\n");
    return 1;
  }

  changeScene(0); // Start with the main menu scene

  while (gIsRunning)
  {
    // Process events
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_QUIT)
      {
        gIsRunning = false;
      }
    }

    // Update game state
    update();

    // Render game
    render();
  }

  destroy();
  return 0;
}

bool init()
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Initialize SDL_ttf
  if (TTF_Init() == -1)
  {
    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    return false;
  }

  // Create window
  gWindow = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (gWindow == NULL)
  {
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Create renderer for window
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (gRenderer == NULL)
  {
    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Initialize renderer color
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

  return true;
}

void update()
{
  Uint32 currentTime = SDL_GetTicks();

  // Check if it's time to change scenes
  if (gCurrentScene == 0 && currentTime - gSceneStartTime > 3000)
  { // Main menu to game after 3 seconds
    changeScene(1);
  }
  else if (gCurrentScene == 1 && currentTime - gSceneStartTime > 5000)
  { // Game to main menu after 5 seconds
    changeScene(0);
  }

  // Update game logic here
}

void render()
{
  // Clear screen
  SDL_RenderClear(gRenderer);

  // Render based on current scene
  if (gCurrentScene == 0)
  {
    // Render main menu
    SDL_Color textColor = {0, 0, 0, 255};
    TTF_Font *Sans = TTF_OpenFont("test/OpenSans-Regular.ttf", 24); // Load a font (you should provide your own)
    if (Sans == NULL)
    {
      printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
      return;
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(Sans, "Main Menu", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    SDL_Rect textRect;
    textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
    textRect.y = SCREEN_HEIGHT / 2 - textSurface->h / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(Sans);
  }
  else if (gCurrentScene == 1)
  {
    // Render game scene (placeholder)
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Red background
    SDL_RenderClear(gRenderer);

    // Render "Game Scene"
    SDL_Color textColor = {255, 255, 255, 255};
    TTF_Font *Sans = TTF_OpenFont("test/OpenSans-Regular.ttf", 24); // Load a font (you should provide your own)
    if (Sans == NULL)
    {
      printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
      return;
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(Sans, "Game Scene", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    SDL_Rect textRect;
    textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
    textRect.y = SCREEN_HEIGHT / 2 - textSurface->h / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(Sans);
  }

  // Update screen
  SDL_RenderPresent(gRenderer);
}

void destroy()
{
  // Destroy window
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;

  // Quit SDL_ttf
  TTF_Quit();

  // Quit SDL subsystems
  SDL_Quit();
}

void changeScene(int scene)
{
  gCurrentScene = scene;
  gSceneStartTime = SDL_GetTicks(); // Record the time when the scene change occurred
}
