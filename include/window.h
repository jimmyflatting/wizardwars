#ifndef WINDOW_H
#define WINDOW_H

// includes
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

// defines
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define TILE_SIZE 32

// variables
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern bool isRunning;

// functions
void window_init();
void window_loop();
void window_destroy();

#endif // WINDOW_H