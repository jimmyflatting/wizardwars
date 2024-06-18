#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <tmx.h>

void *SDL_tex_loader(const char *path);
void render_map(tmx_map *map);

SDL_Rect src, dest;

SDL_Texture *dirt;
SDL_Texture *grass;
SDL_Texture *water;

#endif /* MAP_H */