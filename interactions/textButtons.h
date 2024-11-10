#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../utils/text.h"

typedef struct {
    Text* text;
    SDL_Color hilightColor;
    char* ID;
} Button;


void hilightButtons();
void makeButton(Text* text, SDL_Color hilightColor, char* id);
void freeTextButtons();

bool isMouseOnButton();
void makeButtonsDoSomething(SDL_Renderer* renderer);