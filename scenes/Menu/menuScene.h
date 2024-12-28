#pragma once

#include <SDL2/SDL.h>

#include "../../utils/text.h"

extern int nickLength;
extern Text nicknameValue;

void renderMenu(SDL_Renderer* menuRenderer);
void initMenuScene();

void writeSymbol(int sym);
void delSymbol();
void saveNickname();

void freeMenuScene();