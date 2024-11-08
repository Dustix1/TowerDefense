#ifndef RECTSTUFF_H
#define RECTSTUFF_H
#include <SDL2/SDL.h>

// Creates a Rect and returns it
SDL_Rect createRect(int posX, int posY, int width, int height);

// Creates a Color and returns it
SDL_Color createColor(char* hexColor, int alpha);

#endif