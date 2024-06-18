#include <client.h>
#include <tmx.h>
#include "defs.h"

#include <stdio.h>
#include <unistd.h>
#include "window.h"

int numOtherPlayers;

tmx_map *background;
tmx_map *foreground;
tmx_map *breakable;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int scene;

void client_init()
{
    // Render game scene (placeholder)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red background
    SDL_RenderClear(renderer);

    // Render "Game Scene"
    SDL_Color textColor = {255, 255, 255, 255};
    TTF_Font *Sans = TTF_OpenFont("res/fonts/OpenSans-Regular.ttf", 24); // Load a font (you should provide your own)
    if (Sans == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(Sans, "Client Scene", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
    textRect.y = SCREEN_HEIGHT / 2 - textSurface->h / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(Sans);
};

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

    window = SDL_CreateWindow("SDL Net Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    tmx_map_free(background);
    if (foreground)
    {
        tmx_map_free(foreground);
    }
    if (breakable)
    {
        tmx_map_free(breakable);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDLNet_Quit();
    SDL_Quit();
}

void client_send_message(Player *player)
{
    char data[MAX_PACKET_SIZE];
    sprintf(data, "%d %d %d %d %d", player->id, player->x, player->y, player->direction, player->health);

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

    // parse received position data
    int id, x, y, direction, health;
    sscanf(data, "%d %d %d %d %d", &id, &x, &y, &direction, &health);

    if (id == player.id)
    {
        // update player's own position
        player.x = x;
        player.y = y;
        player.direction = direction;
        player.health = health;
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
                ++numOtherPlayers;
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

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // clear screen with black
    SDL_RenderClear(renderer);

    // render the background map
    render_map(background);

    // render the player
    SDL_Color playerColor = {0, 0, 255, 255}; // blue color for player
    renderPlayer(&player, playerColor);

    // render other players
    SDL_Color otherPlayerColor = {255, 0, 0, 255}; // red color for other players
    for (int i = 0; i < numOtherPlayers; ++i)
    {
        renderPlayer(&otherPlayers[i], otherPlayerColor);
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

    // send updated position and shooting state to server
    client_send_message(&player);
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

void start_client()
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
        exit(EXIT_FAILURE);
    }

    SDLNet_TCP_AddSocket(socketSet, clientSocket);

    // maploader
    tmx_img_load_func = SDL_tex_loader;
    tmx_img_free_func = (void (*)(void *))SDL_DestroyTexture;

    // LOAD BACKGROUND
    background = tmx_load("./res/maps/grasslvl/testlvl.tmx");
    if (!background)
    {
        tmx_perror("Cannot load map");
        exit(EXIT_FAILURE);
    }

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
    exit(EXIT_SUCCESS);
}