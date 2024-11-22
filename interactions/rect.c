#include "rect.h"

bool isRectOnRect(SDL_Rect r1, SDL_Rect r2) {
    SDL_Rect r;
    if (SDL_IntersectRect(&r1, &r2, &r) == SDL_TRUE)
    return true;
    else
    return false;
}