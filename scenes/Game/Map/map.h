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
    SDL_Rect* pathRects;
    SDL_Rect mapRect;
    SDL_Rect vanRect;
    struct MapPointsWithDirections mapPointsWithDirections;
};

extern struct Map map;

void loadPathTexture(SDL_Renderer* renderer);
void createMap(SelectedMap selectedMap, SDL_Renderer* renderer);
void generatePath(SDL_Renderer* renderer);
void renderPath(SDL_Renderer* renderer);