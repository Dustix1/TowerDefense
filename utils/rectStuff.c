#include "rectStuff.h"

SDL_Rect createRect(int posX, int posY, int width, int height) {
    SDL_Rect buff = {
        .x = posX,
        .y = posY,
        .w = width,
        .h = height
    };
    return buff;
}

SDL_Color createColor(char* hexColor, int alpha) {
    char r[3] = {0};
    strncpy(r, hexColor, 2);
    char g[3] = {0};
    strncpy(g, hexColor + 2, 2);
    char b[3] = {0};
    strncpy(b, hexColor + 4, 2);
    SDL_Color buff = {
        .r = strtoul(r, NULL, 16),
        .g = strtoul(g, NULL, 16),
        .b = strtoul(b, NULL, 16),
        .a = alpha
    };
    return buff;
}