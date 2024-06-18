#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Rect rect;
    SDL_Color color;
    char *text;
    void (*callback)(int);
} Button;

void render_button(Button *button);
void init_button(Button *button, int x, int y, int w, int h, SDL_Color color, char *text, void (*callback)(int));
void destroy_button(Button *button);
void handle_button_click(Button *button, int x, int y, int argument);

#endif // BUTTONS_H