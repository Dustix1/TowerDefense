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
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Color highlightColor;
    ButtonType type;
    char* ID;
    bool active;
} Button;


void highlightButtons();
void makeButtonsLookInActive();
void makeButton(Text* text, SDL_Rect rect, SDL_Texture* texture, SDL_Color* highlightColor, char* id, ButtonType type);
void freeButtons();
Button* searchForButton(char* buttonId);

bool isMouseOnButton();
void makeButtonsDoSomething(SDL_Renderer* renderer);