#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL_net.h>

typedef struct
{
    int id;
    int x, y;
    int direction;
    TCPsocket socket;
} Player;

extern Player player;
extern Player otherPlayers[];
extern int numOtherPlayers;

#endif /* PLAYER_H */
