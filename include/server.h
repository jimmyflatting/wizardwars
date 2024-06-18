#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include <client.h>

TCPsocket serverSocket;
SDLNet_SocketSet socketSet;

extern Player clients[MAX_CLIENTS];

int handle_client(void *clientData);
void handle_client_message(TCPsocket clientSocket, const char *data);
void broadcast_message(const char *data, int len, TCPsocket excludeSocket);

void start_server();
void init_server();
void close_server();

#endif /* SERVER_H */