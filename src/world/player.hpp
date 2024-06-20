#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include "global.hpp"

class Player
{
public:
  int x, y;
  SDL_Rect rect;
  SDL_Texture *texture;
  SDL_Rect clip;
  int health;
  float speed;
  int direction;
  bool moving;

  Player();
  ~Player();

  void update(Player *player, bool moving, int direction);
  static void render(SDL_Renderer *renderer);
};

#endif // PLAYER_HPP