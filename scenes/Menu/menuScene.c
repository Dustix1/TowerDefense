#include "menuScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../utils/rectStuff.h"
#include "../../utils/text.h"
#include "../../interactions/mouse.h"

TTF_Font *font;

void initMenuScene()
{
    font = TTF_OpenFont("../fonts/Arial.ttf", 60);
}

void renderMenu(SDL_Renderer* menuRenderer, int windowSizeX, int windowSizeY)
{
    sdl_draw_text(menuRenderer, font, createColor("FFFFFF", 255), createRect(25, 25, windowSizeX - 50, 125), "Epic Tower Defense");

    SDL_Rect startBtnRect = createRect(windowSizeX / 2 - 125, 225, 300, 125);
    char startBtnColor[7] = {0};
    if (!isMouseOnRect(startBtnRect))
    {
        strcpy(startBtnColor, "FFA500");
    }
    else
    {
        strcpy(startBtnColor, "00FF00");
    }

    SDL_Rect quitBtnRect = createRect(windowSizeX / 2 - 125, windowSizeY / 2 - 75, 300, 125);
    char quitBtnColor[7] = {0};
    if (!isMouseOnRect(quitBtnRect))
    {
        strcpy(quitBtnColor, "DD8300");
    }
    else
    {
        strcpy(quitBtnColor, "FF0000");
    }
    sdl_draw_text(menuRenderer, font, createColor(startBtnColor,    255), startBtnRect, "Start");
    sdl_draw_text(menuRenderer, font, createColor(quitBtnColor, 255), quitBtnRect, "Quit");
}

void freeMenuScene() { // DON'T FORGET TO USE THIS !!!
    TTF_CloseFont(font);
}