#include "scene.h"

int scene = 0;

void update_scene()
{
  switch (scene)
  {
  case 0:
    menu_init();
    break;
  case 1:
    host_init();
    break;
  case 2:
    client_init();
    break;
  default:
    break;
  }
};

void render_scene()
{
  switch (scene)
  {
  case 0:
    menu_loop();
    break;
  case 1:
    host_loop();
    break;
  case 2:
    client_loop();
    break;
  default:
    break;
  }
};