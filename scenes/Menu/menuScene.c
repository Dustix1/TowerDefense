#include "menuScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../utils/rectStuff.h"
#include "../../utils/text.h"
#include "../../interactions/textButtons.h"
#include "../../interactions/mouse.h"

static TTF_Font *font;

void initMenuScene()
{
    font = TTF_OpenFont("../fonts/Arial.ttf", 60);
}

void renderMenu(SDL_Renderer* renderer, int windowSizeX, int windowSizeY)
{
    int w;
    Text *title = malloc(sizeof(Text));
    title->text = "Epic Tower Defense";
    title->color = createColor("FFFFFF", 255);
    title->rect = createRect(25, 25, windowSizeX - 50, 125);
    title->font = font;

    Text *startBtnText = malloc(sizeof(Text));
    startBtnText->text = "Start";
    startBtnText->color = createColor("FFA500", 255);
    w = 300;
    startBtnText->rect = createRect((windowSizeX - w) / 2, 225, w, 125);
    startBtnText->font = font;
    makeButton(startBtnText, createColor("00FF00", 255), "MenuStartBtn");

    Text *quitBtnText = malloc(sizeof(Text));
    quitBtnText->text = "Quit";
    quitBtnText->color = createColor("DD8300", 255);
    w = 300;
    quitBtnText->rect = createRect((windowSizeX - w) / 2, startBtnText->rect.y + startBtnText->rect.h + 25, w, 125);
    quitBtnText->font = font;
    makeButton(quitBtnText, createColor("FF0000", 255), "MenuQuitBtn");


    hilightButtons();
    sdl_draw_text(renderer, title);
    sdl_draw_text(renderer, startBtnText);
    sdl_draw_text(renderer, quitBtnText);
}

void freeMenuScene() {
    TTF_CloseFont(font);
}