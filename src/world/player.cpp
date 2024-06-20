#include "player.hpp"

Player::Player()
{
  // Initialize player
  x = 0;
  y = 0;
  rect.x = x;
  rect.y = y;
  rect.w = 32;
  rect.h = 32;
  texture = nullptr;
  clip.x = 0;
  clip.y = 0;
  clip.w = 32;
  clip.h = 32;
  health = 100;
  speed = 0.2f;
  direction = 0;
  moving = false;
}

Player::~Player()
{
  // Deallocate player
  SDL_DestroyTexture(texture);
  texture = nullptr;
}

void Player::update(Player *player, bool moving, int direction)
{
  // Update player
  if (moving)
  {
    switch (direction)
    {
    case 0:
      this->y -= speed;
      break;
    case 1:
      this->x += speed;
      break;
    case 2:
      this->y += speed;
      break;
    case 3:
      this->x -= speed;
      break;
    }
  }

  this->rect.x = x;
  this->rect.y = y;
}