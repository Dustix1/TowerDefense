#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

extern int currentWave;
extern bool waveRunning;

void loadNextWave();
void readWave();
void runWave(SDL_Renderer* renderer);
void startWave();
void endWave();
void resetWaves();