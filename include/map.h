#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <stdio.h>

void init_map();
void update_map();
void load_map(int arr[20][25]);
void render_map();

SDL_Rect src, dest;

SDL_Texture *dirt;
SDL_Texture *grass;
SDL_Texture *water;

#endif