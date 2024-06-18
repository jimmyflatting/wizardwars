#include <server.h>

Player clients[MAX_CLIENTS];

void init_server()
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

void close_server()
{
    SDLNet_FreeSocketSet(socketSet);
    SDLNet_TCP_Close(serverSocket);
    SDLNet_Quit();
}

void broadcast_message(const char *data, int len, TCPsocket excludeSocket)
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i].id != -1 && clients[i].socket != excludeSocket)
        {
            SDLNet_TCP_Send(clients[i].socket, data, len);
        }
    }
}

void handle_client_message(TCPsocket clientSocket, const char *data)
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
            break;
        }
    }

    // broadcast updated positions to all clients
    broadcast_message(data, strlen(data) + 1, clientSocket);
}

int handle_client(void *clientData)
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
            // TODO decrease clients count by 1
        }
        else
        {
            data[len] = '\0';
            printf("Received from client: %s\n", data);

            int id, x, y, direction, health;
            sscanf(data, "%d %d %d %d %d", &id, &x, &y, &direction, &health);

            // update client position
            for (int i = 0; i < MAX_CLIENTS; ++i)
            {
                if (clients[i].id != -1 && clients[i].socket == clientSocket)
                {
                    clients[i].x = x;
                    clients[i].y = y;
                    clients[i].direction = direction;
                    clients[i].health = health;
                    break;
                }
            }

            // broadcast updated positions to all clients
            broadcast_message(data, len, clientSocket);
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

void start_server()
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        clients[i].id = -1;
        clients[i].socket = NULL;
        clients[i].active = false;
        clients[i].thread = NULL;
    }

    init_server();

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
                        clients[i].thread = SDL_CreateThread(handle_client, "ClientThread", (void *)&clients[i]);
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
        }
    }

    close_server();
    exit(EXIT_SUCCESS);
}