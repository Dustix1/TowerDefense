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

typedef enum {
    NORMALPROJECTILE,
    GORYOPROJECTILE
} ProjectileType;

typedef struct Tower Tower;

typedef struct {
    SDL_Texture* texture;
    ProjectileType type;
    SDL_FRect rect;
    SDL_FPoint directionVector;
    double rotationAngle;
    float damage;
} Projectile;

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
    int buffed;
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

void buffTowers(SDL_Renderer* renderer);
void makeTowersDoSomething(SDL_Renderer* renderer);

void renderProjectiles();
void moveProjectiles();
void checkForProjectileCollision();

void getTarget();
void setNextTarget();

void freeTowers();