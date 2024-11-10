#pragma once

#include <SDL2/SDL.h>

void renderGame(SDL_Renderer* renderer, int windowSizeX, int windowSizeY);
void initGameScene(SDL_Renderer* renderer);
void freeGameScene();