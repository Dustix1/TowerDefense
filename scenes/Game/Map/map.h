#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../../../utils/gameStatus.h"
#include "../../../utils/rectStuff.h"


struct Map{
    SDL_Texture* mapTexture;
    SDL_Rect mapRect;
    SDL_Point* pointPath;
};

extern struct Map map;

void createMap(SelectedMap selectedMap, SDL_Renderer* renderer);