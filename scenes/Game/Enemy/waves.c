#include "waves.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../../../utils/gameStatus.h"
#include "enemy.h"

static FILE *waveFile;
static char wave[1000];
int currentWave;

static int waveData[200];
static int waveDataIndex;
static int waveDataEndIndex;

static bool waveRunning = false;
static int nextIterTime = 0;

void loadNextWave() {
    char filePath[50];
    switch (gameStatus.selectedMap)
    {
    case WILLOW:
        sprintf(filePath, "../scenes/Game/waves/Willow/%d.wave", currentWave);
        break;
    default:
        gameStatus.running = 0;
        break;
    }


    waveFile = fopen(filePath, "r");
    if (waveFile == NULL) {
        // GAME COMPLETE - DO SOMETHING :3
        //gameStatus.running = 0;
    }

    fgets(wave, 1000, waveFile);
    fclose(waveFile);
    readWave();
}

void readWave() {
    waveDataIndex = 0;

    char *token = strtok(wave, "; ");
    if (token == NULL) return;

    waveData[waveDataIndex++] = atoi(token);

    while ((token = strtok(NULL, "; ")) != NULL)
    {
        if (waveDataIndex >= 200) break;

        if (waveDataIndex % 2 == 0) {
            waveData[waveDataIndex++] = atoi(token);
        } else {
            if (strcmp(token, "Spirit") == 0) {
                waveData[waveDataIndex++] = SPIRIT;
            }
            if (strcmp(token, "Shade") == 0) {
                waveData[waveDataIndex++] = SHADE;
            }
            if (strcmp(token, "Goryo") == 0) {
                waveData[waveDataIndex++] = GORYO;
            }
            if (strcmp(token, "Deogen") == 0) {
                waveData[waveDataIndex++] = DEOGEN;
            }
        }
    }
    waveDataEndIndex = waveDataIndex;
    waveDataIndex = 0;
}

void runWave(SDL_Renderer* renderer) {
    if (waveRunning) {
        if (waveDataIndex >= waveDataEndIndex) {
            if (enemyCount == 0) endWave();
            return;
        }

        if (SDL_GetTicks64() >= nextIterTime) {
            spawnNewEnemy(waveData[waveDataIndex++]);
            nextIterTime = SDL_GetTicks64() + waveData[waveDataIndex++];
        }
    }
}

void startWave() {
    nextIterTime = SDL_GetTicks64() + waveData[waveDataIndex++];
    waveRunning = true;
}

void endWave() {
    waveRunning = false;
    currentWave++;
    loadNextWave();
}