#pragma once

#include <SDL2/SDL.h>

#include "../../utils/gameStatus.h"

void renderGame(SDL_Renderer* renderer);
void initGameScene(SDL_Renderer* renderer, SelectedMap SelectedMap);
void flashVan();

void freeGameScene();