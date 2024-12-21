#pragma once

#include "SDL2/SDL.h"

extern int currentWave;

void loadNextWave();
void readWave();
void runWave(SDL_Renderer* renderer);
void startWave();
void endWave();
void resetWaves();