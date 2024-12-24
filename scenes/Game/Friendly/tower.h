#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../Enemy/enemy.h"

extern bool draggingTower;

typedef enum {
    HOLYWATERCANNON,
    INCENSE,
    CRUCIFIX
} TowerType;

typedef struct Tower Tower;

struct Tower {
    TowerType type;
    SDL_Rect rect;
    SDL_Rect range;
    SDL_Texture* texture;
    ENEMY** currentTarget;
    ENEMY** enemyTargets;
    Tower** towerTargets;
    int enemyTargetCount;
    int towerTargetCount;
    int nextTimeOfAttack;
    int cost;
    float baseDamage;
    float damage;
    bool shouldShowRange;
};

void loadTowers(SDL_Renderer* renderer);
void renderInGameTowers(SDL_Renderer* renderer);
void renderUITowerSelection(SDL_Renderer* renderer);

void setTowerUIButtonsState(bool state);

void createTower(char* id);
void destroyTower(Tower* tower);
void dragTower();
void stopDragging();
void checkForMoney();
void makeRangeFollowtower();
void showTowerRange();
void sellTower();

void makeTowersDoSomething(SDL_Renderer* renderer);

void getTarget();
void setNextTarget();

void freeTowers();