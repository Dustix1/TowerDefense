#include <SDL2/SDL.h>

SDL_Rect createRect(int posX, int posY, int width, int height) {
    SDL_Rect buff = {
        .x = posX,
        .y = posY,
        .w = width,
        .h = height
    };
    return buff;
}

SDL_Color createColor(int r, int g, int b, int a) {
    SDL_Color buff = {
        .b = b,
        .g = g,
        .r = r,
        .a = a
    };
    return buff;
}