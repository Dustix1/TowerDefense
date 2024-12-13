#pragma once

#include <SDL2/SDL.h>

// Creates a Rect and returns it
SDL_Rect createRect(int posX, int posY, int width, int height);

// Creates a Color and returns it
SDL_Color createColor(char* hexColor, int alpha);

SDL_FRect createFRect(float posX, float posY, float width, float height);