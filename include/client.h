#ifndef CLIENT_H
#define CLIENT_H

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

typedef struct
{
  // network
  int id;
  TCPsocket socket;
  SDL_Thread *thread;
  bool active;

  // game
  int x;
  int y;
  int direction;
  int health;
} Player;

Player player;
Player otherPlayers[MAX_CLIENTS];

// SDL_Window *window;
// SDL_Renderer *renderer;
TCPsocket clientSocket;
SDLNet_SocketSet socketSet;

extern int numOtherPlayers;

// new refactored functions
void client_init();
void client_loop();

// old functions
void start_client();
void initClient();
void closeClient();
void client_send_message(Player *player);
void receiveMessage();

void render();
void renderPlayer(Player *p, SDL_Color color);
void handleInput();

TCPsocket connectToServer();

#endif /* CLIENT_H */