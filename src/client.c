#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

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
int numOtherPlayers = 0;

Projectile projectiles[MAX_PROJECTILES];

SDL_Window *window;
SDL_Renderer *renderer;
TCPsocket clientSocket;
SDLNet_SocketSet socketSet;

void initClient()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("SDL Net Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDLNet_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDLNet_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    socketSet = SDLNet_AllocSocketSet(1);
    if (!socketSet)
    {
        fprintf(stderr, "SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDLNet_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void closeClient()
{
    SDLNet_FreeSocketSet(socketSet);
    SDLNet_TCP_Close(clientSocket);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDLNet_Quit();
    SDL_Quit();
}

void sendMessage(Player *player)
{
    char data[MAX_PACKET_SIZE];
    sprintf(data, "%d %d %d %d %d %d %d %d %d", player->id, player->x, player->y, player->direction, player->health, player->shooting,
            player->projectile_x, player->projectile_y, player->projectile_direction);

    // send message to server
    if (SDLNet_TCP_Send(clientSocket, data, strlen(data) + 1) < strlen(data) + 1)
    {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    }
}

void receiveMessage()
{
    char data[MAX_PACKET_SIZE];
    int len = SDLNet_TCP_Recv(clientSocket, data, MAX_PACKET_SIZE);
    if (len <= 0)
    {
        fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
        return;
    }

    data[len] = '\0';
    printf("Received from server: %s\n", data);
    if (data[0] == 'P')
    {
        // projectile data
        int x, y, direction;
        sscanf(data, "P %d %d %d", &x, &y, &direction);
        for (int i = 0; i < MAX_PROJECTILES; ++i)
        {
            if (!projectiles[i].active)
            {
                projectiles[i].x = x;
                projectiles[i].y = y;
                projectiles[i].direction = direction;
                projectiles[i].active = true;
                break;
            }
        }
    }
    else
    {

        // parse received position data
        int id, x, y, direction, health, shooting;
        sscanf(data, "%d %d %d %d %d %d", &id, &x, &y, &direction, &health, &shooting);

        if (id == player.id)
        {
            // update player's own position
            player.x = x;
            player.y = y;
            player.direction = direction;
            player.health = health;
            player.shooting = shooting;
        }
        else
        {
            // update other player's position
            int i;
            for (i = 0; i < numOtherPlayers; ++i)
            {
                if (otherPlayers[i].id == id)
                {
                    otherPlayers[i].x = x;
                    otherPlayers[i].y = y;
                    otherPlayers[i].direction = direction;
                    otherPlayers[i].health = health;
                    otherPlayers[i].shooting = shooting;
                    break;
                }
            }

            if (i == numOtherPlayers)
            {
                // new player
                if (numOtherPlayers < MAX_CLIENTS)
                {
                    otherPlayers[numOtherPlayers].id = id;
                    otherPlayers[numOtherPlayers].x = x;
                    otherPlayers[numOtherPlayers].y = y;
                    otherPlayers[numOtherPlayers].direction = direction;
                    otherPlayers[numOtherPlayers].health = health;
                    otherPlayers[numOtherPlayers].shooting = shooting;
                    ++numOtherPlayers;
                }
            }
        }
    }
}

void renderPlayer(Player *p, SDL_Color color)
{
    SDL_Rect rect = {p->x, p->y, 32, 32}; // assuming player size is 32x32
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void renderProjectile(Projectile *p)
{
    if (p->active)
    {
        SDL_Rect rect = {p->x, p->y, 8, 8};                 // size of projectile
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // yellow color for projectiles
        SDL_RenderFillRect(renderer, &rect);
    }
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // clear screen with black
    SDL_RenderClear(renderer);

    // render the player
    SDL_Color playerColor = {0, 0, 255, 255}; // blue color for player
    renderPlayer(&player, playerColor);

    // render other players
    SDL_Color otherPlayerColor = {255, 0, 0, 255}; // red color for other players
    for (int i = 0; i < numOtherPlayers; ++i)
    {
        renderPlayer(&otherPlayers[i], otherPlayerColor);
    }

    // render projectiles
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white color for projectiles

    // render projectiles from other players
    for (int i = 0; i < numOtherPlayers; ++i)
    {
        if (otherPlayers[i].shooting)
        {
            // draw a simple line for the projectile
            SDL_RenderDrawLine(renderer, otherPlayers[i].projectile_x, otherPlayers[i].projectile_y,
                               otherPlayers[i].projectile_x + PROJECTILE_LENGTH * cos(otherPlayers[i].projectile_direction),
                               otherPlayers[i].projectile_y + PROJECTILE_LENGTH * sin(otherPlayers[i].projectile_direction));
        }
    }

    // render player's own projectiles
    if (player.shooting)
    {
        SDL_RenderDrawLine(renderer, player.projectile_x, player.projectile_y,
                           player.projectile_x + PROJECTILE_LENGTH * cos(player.projectile_direction),
                           player.projectile_y + PROJECTILE_LENGTH * sin(player.projectile_direction));
    }

    // update the screen
    SDL_RenderPresent(renderer);
}

void handleInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            printf("Window closed. Exiting.\n");
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                player.shooting = true;
                player.projectile_x = player.x; // init position of the projectile
                player.projectile_y = player.y;
                player.projectile_direction = player.direction; // init direction of the projectile
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                player.shooting = false;
                break;
            }
            break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT])
    {
        player.x -= 5;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        player.x += 5;
    }
    if (state[SDL_SCANCODE_UP])
    {
        player.y -= 5;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        player.y += 5;
    }

    // update the projectile's position
    if (player.shooting)
    {
        // move projectile in the specified direction
        player.projectile_x += PROJECTILE_SPEED * cos(player.projectile_direction);
        player.projectile_y += PROJECTILE_SPEED * sin(player.projectile_direction);
    }

    // send updated position and shooting state to server
    sendMessage(&player);
}

TCPsocket connectToServer()
{
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, SERVER_HOST, SERVER_PORT) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return NULL;
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket)
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return NULL;
    }

    char idMsg[16];
    int len = SDLNet_TCP_Recv(socket, idMsg, sizeof(idMsg));
    if (len <= 0)
    {
        fprintf(stderr, "Failed to receive ID from server: %s\n", SDLNet_GetError());
        SDLNet_TCP_Close(socket);
        return NULL;
    }

    player.id = atoi(idMsg);
    printf("Assigned ID: %d\n", player.id);

    return socket;
}

int main()
{
    // initialize SDL and SDL_net
    initClient();

    // dummy initialization of player position
    player.id = -1; // will be assigned by server
    player.x = 320; // init player position
    player.y = 240;
    player.direction = 0;

    int retryCount = 0;

    while (retryCount < MAX_RETRIES && (clientSocket = connectToServer()) == NULL)
    {
        printf("Retrying connection to server... (%d/%d)\n", ++retryCount, MAX_RETRIES);
        SDL_Delay(1000); // wait before retrying
    }

    if (clientSocket == NULL)
    {
        fprintf(stderr, "Failed to connect to server after %d attempts. Exiting.\n", MAX_RETRIES);
        closeClient();
        return EXIT_FAILURE;
    }

    SDLNet_TCP_AddSocket(socketSet, clientSocket);

    // main loop
    while (1)
    {
        handleInput();

        // check for incoming data
        if (SDLNet_CheckSockets(socketSet, 0) > 0 && SDLNet_SocketReady(clientSocket))
        {
            receiveMessage();
        }

        render();      // render player and other players
        SDL_Delay(16); // add a small delay to avoid high CPU usage (approximately 60 FPS, change for delta later)
    }

    // cleanup and quit
    closeClient();
    return 0;
}
