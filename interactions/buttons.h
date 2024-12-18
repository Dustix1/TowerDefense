#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../utils/text.h"

typedef enum {
    TEXTBUTTON,
    RECTBUTTON
} ButtonType;

typedef struct {
    Text* text;
    SDL_Rect rect;
    SDL_Color hilightColor;
    ButtonType type;
    char* ID;
    bool active;
} Button;


void highlightButtons();
void makeButton(Text* text, SDL_Rect rect, SDL_Color* hilightColor, char* id, ButtonType type);
void freeButtons();

bool isMouseOnButton();
void makeButtonsDoSomething(SDL_Renderer* renderer);