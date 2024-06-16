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

void initServer()
{
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, SERVER_PORT) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        exit(EXIT_FAILURE);
    }

    socketSet = SDLNet_AllocSocketSet(MAX_CLIENTS + 1);
    if (!socketSet)
    {
        fprintf(stderr, "SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        exit(EXIT_FAILURE);
    }

    serverSocket = SDLNet_TCP_Open(&ip);
    if (!serverSocket)
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        exit(EXIT_FAILURE);
    }

    SDLNet_TCP_AddSocket(socketSet, serverSocket);
    printf("Server started. Waiting for connections...\n");
}

void closeServer()
{
    SDLNet_FreeSocketSet(socketSet);
    SDLNet_TCP_Close(serverSocket);
    SDLNet_Quit();
}

void broadcastMessage(const char *data, int len, TCPsocket excludeSocket)
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i].id != -1 && clients[i].socket != excludeSocket)
        {
            SDLNet_TCP_Send(clients[i].socket, data, len);
        }
    }
}

void handleClientMessage(TCPsocket clientSocket, const char *data)
{
    int id, x, y, direction, health;
    bool shooting;
    sscanf(data, "%d %d %d %d %d %s", &id, &x, &y, &direction, &health, &shooting);

    // update client position, health, and shooting status
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i].id == id)
        {
            clients[i].x = x;
            clients[i].y = y;
            clients[i].direction = direction;
            clients[i].health = health;
            clients[i].shooting = shooting;
            break;
        }
    }

    // broadcast updated positions to all clients
    broadcastMessage(data, strlen(data) + 1, clientSocket);
}

int handleClient(void *clientData)
{
    Player *client = (Player *)clientData;
    TCPsocket clientSocket = client->socket;
    bool running = true;
    char data[MAX_PACKET_SIZE];

    while (running)
    {
        int len = SDLNet_TCP_Recv(clientSocket, data, MAX_PACKET_SIZE);
        if (len <= 0)
        {
            running = false;
            printf("Client disconnected.\n");
        }
        else
        {
            data[len] = '\0';
            printf("Received from client: %s\n", data);

            int id, x, y, direction, health, shooting, projectile_x, projectile_y, projectile_direction;
            sscanf(data, "%d %d %d %d %d %d %d %d %d", &id, &x, &y, &direction, &health, &shooting,
                   &projectile_x, &projectile_y, &projectile_direction);

            // update client position
            for (int i = 0; i < MAX_CLIENTS; ++i)
            {
                if (clients[i].id != -1 && clients[i].socket == clientSocket)
                {
                    clients[i].x = x;
                    clients[i].y = y;
                    clients[i].direction = direction;
                    clients[i].health = health;
                    clients[i].shooting = shooting;
                    clients[i].projectile_x = projectile_x;
                    clients[i].projectile_y = projectile_y;
                    clients[i].projectile_direction = projectile_direction;
                    break;
                }
            }

            // broadcast updated positions to all clients, including projectiles
            broadcastMessage(data, len, clientSocket);
        }
    }

    // clean up client disconnect
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i].id != -1 && clients[i].socket == clientSocket)
        {
            clients[i].id = -1;
            clients[i].socket = NULL;
            break;
        }
    }

    SDLNet_TCP_DelSocket(socketSet, clientSocket);
    SDLNet_TCP_Close(clientSocket);
    return 0;
}

void updateProjectiles()
{
    for (int i = 0; i < MAX_PROJECTILES; ++i)
    {
        if (projectiles[i].active)
        {
            // update projectile position based on its direction
            switch (projectiles[i].direction)
            {
            case 0:
                projectiles[i].y -= 5;
                break; // up
            case 1:
                projectiles[i].x += 5;
                break; // right
            case 2:
                projectiles[i].y += 5;
                break; // down
            case 3:
                projectiles[i].x -= 5;
                break; // left
            }
            // check for collisions with players and deactivate if necessary
            for (int j = 0; j < MAX_CLIENTS; ++j)
            {
                if (clients[j].id != -1 && clients[j].health > 0 &&
                    abs(projectiles[i].x - clients[j].x) < 32 &&
                    abs(projectiles[i].y - clients[j].y) < 32)
                {
                    clients[j].health -= 10;
                    projectiles[i].active = false;
                }
            }
        }
    }
}

void broadcastProjectileStates()
{
    char data[MAX_PACKET_SIZE];
    for (int i = 0; i < MAX_PROJECTILES; ++i)
    {
        if (projectiles[i].active)
        {
            sprintf(data, "P %d %d %d", projectiles[i].x, projectiles[i].y, projectiles[i].direction);
            broadcastMessage(data, strlen(data) + 1, NULL);
        }
    }
}

int main()
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        clients[i].id = -1;
        clients[i].socket = NULL;
        clients[i].active = false;
        clients[i].thread = NULL;
    }

    initServer();

    memset(projectiles, 0, sizeof(projectiles));

    while (1)
    {
        if (SDLNet_CheckSockets(socketSet, 0) > 0)
        {
            if (SDLNet_SocketReady(serverSocket))
            {
                TCPsocket clientSocket = SDLNet_TCP_Accept(serverSocket);
                if (clientSocket)
                {
                    int i;
                    for (i = 0; i < MAX_CLIENTS; ++i)
                    {
                        if (!clients[i].active)
                        {
                            clients[i].id = i;
                            clients[i].socket = clientSocket;
                            clients[i].active = true;
                            break;
                        }
                    }

                    if (i == MAX_CLIENTS)
                    {
                        fprintf(stderr, "Server is full. Rejecting client.\n");
                        SDLNet_TCP_Close(clientSocket);
                    }
                    else
                    {
                        SDLNet_TCP_AddSocket(socketSet, clientSocket);
                        printf("Client connected. ID: %d\n", clients[i].id);

                        // notify the client of its ID
                        char idMsg[16];
                        sprintf(idMsg, "%d", clients[i].id);
                        SDLNet_TCP_Send(clientSocket, idMsg, strlen(idMsg) + 1);

                        // handle client communication
                        clients[i].thread = SDL_CreateThread(handleClient, "ClientThread", (void *)&clients[i]);
                        if (clients[i].thread == NULL)
                        {
                            fprintf(stderr, "SDL_CreateThread failed: %s\n", SDL_GetError());
                            SDLNet_TCP_Close(clientSocket);
                            clients[i].active = false;
                        }
                        else
                        {
                            SDL_DetachThread(clients[i].thread); // detach the thread to clean up automatically when it exits
                        }
                    }
                }
            }

            // check each client socket for incoming data
            for (int i = 0; i < MAX_CLIENTS; ++i)
            {
                if (clients[i].active && SDLNet_SocketReady(clients[i].socket))
                {
                    // no longer calling handleClient here as it is handled in the thread
                }
            }
        }
        updateProjectiles();
    }

    closeServer();
    return 0;
}