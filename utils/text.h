#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    char* text;
    SDL_Color color;
    SDL_Rect rect;
    TTF_Font* font;
} Text;


void sdl_draw_text(SDL_Renderer *renderer, Text* text);

#endif