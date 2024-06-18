#include "texturemanager.h"
#include "client.h"

extern SDL_Renderer *renderer;

SDL_Texture *load_texture(const char *path)
{
    SDL_Surface *tmpSurface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return texture;
}

void draw_texture(SDL_Texture *texture, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(renderer, texture, &src, &dest);
}