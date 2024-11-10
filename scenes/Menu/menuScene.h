#pragma once

#include <SDL2/SDL.h>

void renderMenu(SDL_Renderer* menuRenderer, int windowSizeX, int windowSizeY);
void initMenuScene();
void freeMenuScene();