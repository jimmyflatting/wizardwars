#ifndef SCENE_H
#define SCENE_H

// includes
#include "menu.h"
#include "host.h"
#include "client.h"

// defines

// variables
struct Scene
{
  int scene;
};

// functions
void update_scene();
void render_scene();

#endif // SCENE_H