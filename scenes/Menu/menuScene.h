#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <SDL2/SDL.h>

void renderMenu(SDL_Renderer* menuRenderer, int windowSizeX, int windowSizeY);
void initMenuScene();
void freeMenuScene();

#endif