#include <init.h>
#include "window.h"
#include "buttons.h"

extern Button joinButton;
extern Button hostButton;

int main(int argc, char **argv)
{
  if (!window_init())
  {
    printf("Failed to initialize!\n");
    return 1;
  }

  while (isRunning)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        isRunning = false;
      }

      // handle button click
      else if (event.type == SDL_MOUSEBUTTONDOWN)
      {
        handle_button_click(&joinButton, 1);
        handle_button_click(&hostButton, 2);
      }
    }
    // update the game
    window_update();

    // render the game
    window_render();
  }

  window_destroy();

  return 0;
}