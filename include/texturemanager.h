#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *load_texture(const char *path);
void draw_texture(SDL_Texture *texture, SDL_Rect src, SDL_Rect dest);

#endif
