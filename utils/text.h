#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    char* text;
    SDL_Color color;
    SDL_Rect rect;
    TTF_Font* font;
} Text;


void renderText(SDL_Renderer *renderer, Text* text);