#include "host.h"

extern int scene;

void host_init()
{
  printf("Host initialized\n");
  SDL_Delay(1000);
};

void host_loop()
{
  int cnt = 0;
  printf("Host loop\n");
  while (scene == 1)
  {
    printf("Host looping yo %d\n", cnt);
    SDL_Delay(5000);
    cnt++;
    if (cnt == 3)
    {
      scene = 2; // Transition to the next scene, e.g., client or game over
    }
  }
}
