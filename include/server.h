#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>

#define MAX_CLIENTS 4
#define SERVER_PORT 1234
#define MAX_PACKET_SIZE 1024
#define MAX_PROJECTILES 100

typedef struct
{
  int id;
  TCPsocket socket;
  int x;
  int y;
  int direction;
  bool active;
  SDL_Thread *thread;
  int health;
  bool shooting;
  int projectile_x;
  int projectile_y;
  int projectile_direction;
} Player;

typedef struct
{
  int x, y;
  int direction;
  bool active;
} Projectile;

Player clients[MAX_CLIENTS];
Projectile projectiles[MAX_PROJECTILES];

TCPsocket serverSocket;
SDLNet_SocketSet socketSet;