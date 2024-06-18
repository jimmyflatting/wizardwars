#include "buttons.h"
#include "window.h"

void init_button(Button *button, int x, int y, int w, int h, SDL_Color color, char *text, void (*callback)(int))
{
    button->rect = (SDL_Rect){x, y, w, h};
    button->color = color;
    button->text = text;
    button->callback = callback;
}

void render_button(Button *button)
{
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_RenderFillRect(renderer, &button->rect);

    SDL_Color textColor = {255, 255, 255, 255};
    TTF_Font *Sans = TTF_OpenFont("res/fonts/OpenSans-Regular.ttf", 24); // Load a font (you should provide your own)
    if (Sans == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(Sans, button->text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.x = button->rect.x + button->rect.w / 2 - textSurface->w / 2;
    textRect.y = button->rect.y + button->rect.h / 2 - textSurface->h / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(Sans);
}

void handle_button_click(Button *button, int x, int y, int argument)
{
    if (x >= button->rect.x && x <= (button->rect.x + button->rect.w) &&
        y >= button->rect.y && y <= (button->rect.y + button->rect.h))
    {
        button->callback(argument);
    }
}

void destroy_button(Button *button)
{
    free(button->text);
    free(button);
}