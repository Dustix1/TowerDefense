#include "text.h"

void renderText(SDL_Renderer *renderer, Text* text)
{
    // Vykreslení textu se zadaným fontem a barvou do obrázku (surface)
    SDL_Surface *surface = TTF_RenderText_Blended(text->font, text->text, text->color);
    // Převod surface na hardwarovou texturu
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Vykreslení obrázku
    SDL_RenderCopy(renderer, texture, NULL, &text->rect);

    // Uvolnění textury a surface
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}