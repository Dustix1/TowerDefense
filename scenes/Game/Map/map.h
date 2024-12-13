#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../../../utils/gameStatus.h"
#include "../../../utils/rectStuff.h"
#include "../Enemy/enemy.h"

struct MapPointsWithDirections{
    SDL_Point* points;
    DIRECTION* directions;
};

struct Map{
    SDL_Texture* mapTexture;
    SDL_Rect mapRect;
    struct MapPointsWithDirections mapPointsWithDirections;
};

extern struct Map map;

void createMap(SelectedMap selectedMap, SDL_Renderer* renderer);