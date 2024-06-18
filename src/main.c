#include <init.h>
#include <string.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define TILE_SIZE 32
int numOtherPlayers;

tmx_map *background;
tmx_map *foreground;
tmx_map *breakable;

extern Player clients[MAX_CLIENTS];

int main(int argc, char **argv)
{

  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s -client or %s -server\n", argv[0], argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "-client") == 0)
  {
    start_client();
    return 0;
  }

  else if (strcmp(argv[1], "-server") == 0)
  {
    start_server();
    return 0;
  }
  else
  {
    fprintf(stderr, "Unknown argument: %s\n", argv[1]);
    fprintf(stderr, "Usage: %s -client or %s -server\n", argv[0], argv[0]);
    return 1;
  }
}