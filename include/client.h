#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <map.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 1234
#define MAX_CLIENTS 4
#define MAX_PACKET_SIZE 1024
#define MAX_RETRIES 5
#define MAX_PROJECTILES 100
#define PROJECTILE_SPEED 10
#define PROJECTILE_LENGTH 20

typedef struct
{
  int id;
  int x, y;
  int direction;
  int health;
  bool shooting;
  int projectile_x, projectile_y, projectile_direction;
} Player;

typedef struct
{
  int x, y;
  int direction;
  bool active;
} Projectile;

Player player;
Player otherPlayers[MAX_CLIENTS];

Projectile projectiles[MAX_PROJECTILES];

SDL_Window *window;
SDL_Renderer *renderer;
TCPsocket clientSocket;
SDLNet_SocketSet socketSet;