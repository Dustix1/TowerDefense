#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>

extern bool draggingTower;

typedef enum {
    HOLYWATERCANNON,
    INCENSE
} TowerType;

typedef struct {
    TowerType type;
    SDL_Rect rect;
    SDL_Rect range;
    SDL_Texture* texture;
    float damage;
    int cost;
    bool shouldShowRange;
} Tower;

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