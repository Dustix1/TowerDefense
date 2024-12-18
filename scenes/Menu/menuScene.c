#include "menuScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../utils/rectStuff.h"
#include "../../utils/text.h"
#include "../../interactions/buttons.h"
#include "../../interactions/mouse.h"
#include "../../utils/gameStatus.h"

static TTF_Font *font;
Text *title;
Text *startBtnText;
Text *quitBtnText;
SDL_Color quitColor;
SDL_Color startColor;
SDL_Color titleColor;

void initMenuScene()
{
    font = TTF_OpenFont("../fonts/Arial.ttf", 60);

    int w;
    title = malloc(sizeof(Text));
    title->text = "Epic Tower Defense";
    titleColor = createColor("FFFFFF", 255);
    title->color = titleColor;
    title->rect = createRect(25, 25, gameStatus.windowSizeX - 50, 125);
    title->font = font;

    startBtnText = malloc(sizeof(Text));
    startBtnText->text = "Start";
    startColor = createColor("FFA500", 255);
    startBtnText->color = startColor;
    w = 300;
    startBtnText->rect = createRect((gameStatus.windowSizeX - w) / 2, 225, w, 125);
    startBtnText->font = font;
    SDL_Color startHilightColor = createColor("00FF00", 255);
    makeButton(startBtnText, startBtnText->rect, &startHilightColor, "MenuStartBtn", TEXTBUTTON);

    quitBtnText = malloc(sizeof(Text));
    quitBtnText->text = "Quit";
    quitColor = createColor("DD8300", 255);
    quitBtnText->color = quitColor;
    w = 300;
    quitBtnText->rect = createRect((gameStatus.windowSizeX - w) / 2, startBtnText->rect.y + startBtnText->rect.h + 25, w, 125);
    quitBtnText->font = font;
    SDL_Color quitHilightColor = createColor("FF0000", 255);
    makeButton(quitBtnText, quitBtnText->rect, &quitHilightColor, "QuitBtn", TEXTBUTTON);
}

void renderMenu(SDL_Renderer* renderer)
{
    title->color = titleColor;
    startBtnText->color = startColor;
    quitBtnText->color = quitColor;
    highlightButtons();
    
    renderText(renderer, title);
    renderText(renderer, startBtnText);
    renderText(renderer, quitBtnText);
}

void freeMenuScene() {
    TTF_CloseFont(font);
}