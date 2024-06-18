#ifndef SCENE_H
#define SCENE_H

// includes
#include "main_menu.h"
#include "host.h"
#include "client.h"

// defines

// variables
struct Scene
{
  int scene;
};

// functions
void change_scene(int new_scene);

#endif // SCENE_H