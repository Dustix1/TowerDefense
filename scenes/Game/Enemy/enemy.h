#pragma once

#include <SDL2/SDL.h>

typedef enum {
    SPIRIT,
    SHADE,
    GORYO,
    DEOGEN
} ENEMYTYPE;

typedef enum {
    UP,
    LEFT,
    RIGHT,
    DOWN
} DIRECTION;

typedef struct {
    SDL_Texture* texture;
    ENEMYTYPE type;
    SDL_FRect rect;
    unsigned int currPointIndex;
    float speed;
} ENEMY;

extern ENEMY* enemies;

void spawnNewEnemy(ENEMYTYPE type, SDL_Renderer* renderer);
void moveEnemiesTowardsCurrPoint();
void renderEnemies(SDL_Renderer* renderer);
void reachedPlayerBase(int index);