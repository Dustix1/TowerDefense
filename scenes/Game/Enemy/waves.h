#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

extern int currentWave;
extern bool waveRunning;

void loadNextWave();
void readWave();
void runWave();
void startWave();
void endWave();
void resetWaves();