#include "mouse.h"

bool isMouseOnRect(SDL_Rect rect) {
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    return mouseX >= rect.x && mouseX <= rect.x + rect.w
        && mouseY >= rect.y && mouseY <= rect.y + rect.h;
}