#pragma once

#include <SDL2/SDL.h>

typedef enum {
    SPIRIT,
    SHADE,
    GORYO,
    DEOGEN
} ENEMYTYPE;

typedef struct {
    SDL_Texture* texture;
    ENEMYTYPE type;
    SDL_Rect rect;
    unsigned int currPointIndex;
} ENEMY;

extern ENEMY* enemies;

void spawnNewEnemy(ENEMYTYPE type, SDL_Renderer* renderer);
void moveEnemiesTowardsCurrPoint();
void renderEnemies(SDL_Renderer* renderer);