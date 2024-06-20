#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>

// Local includes
#include "gfx/window.hpp"
#include "gfx/renderer.hpp"
#include "network/network.hpp"
#include "scene/scene_manager.hpp"
#include "scene/scene.hpp"

// Scenes
#include "scene/main_menu.hpp"

// Utils
#include "utils/framerate.hpp"

// Declare the global variables
extern SceneManager scene_manager;
extern MainMenu main_menu;
extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const int TILE_SIZE = 32;
const std::string GAME_NAME = "Wizard Wars";

#endif // GLOBAL_HPP
