#include "framerate.hpp"

void handle_frame_rate(int start_time)
{
  int frame_time = SDL_GetTicks() - start_time;
  if (frame_time < SCREEN_TICKS_PER_FRAME)
  {
    SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_time);
  }
}