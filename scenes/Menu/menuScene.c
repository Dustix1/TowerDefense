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
    font = TTF_OpenFont("../fonts/lazy_dog.ttf", 120);

    int w;
    title = malloc(sizeof(Text));
    title->text = "Phasmophobia Tower Defense";
    titleColor = createColor("FFFFFF", 255);
    title->color = titleColor;
    title->rect = createRect(125, 50, gameStatus.windowSizeX - 275, 125);
    title->font = font;

    startBtnText = malloc(sizeof(Text));
    startBtnText->text = "Start";
    startColor = createColor("00CC00", 255);
    startBtnText->color = startColor;
    w = 300;
    startBtnText->rect = createRect((gameStatus.windowSizeX - w) / 2, 225, w, 125);
    startBtnText->font = font;
    SDL_Color startHighlightColor = createColor("00FF00", 255);
    makeButton(startBtnText, startBtnText->rect, NULL, &startHighlightColor, "MenuStartBtn", TEXTBUTTON);

    quitBtnText = malloc(sizeof(Text));
    quitBtnText->text = "Quit";
    quitColor = createColor("BB0000", 255);
    quitBtnText->color = quitColor;
    w = 300;
    quitBtnText->rect = createRect((gameStatus.windowSizeX - w) / 2, startBtnText->rect.y + startBtnText->rect.h + 25, w, 125);
    quitBtnText->font = font;
    SDL_Color quitHighlightColor = createColor("FF0000", 255);
    makeButton(quitBtnText, quitBtnText->rect, NULL, &quitHighlightColor, "QuitBtn", TEXTBUTTON);
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