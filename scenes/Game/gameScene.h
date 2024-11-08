#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <SDL2/SDL.h>

void renderGame(SDL_Renderer* renderer, int windowSizeX, int windowSizeY);
void initGameScene(SDL_Renderer* renderer);
void freeGameScene();

#endif