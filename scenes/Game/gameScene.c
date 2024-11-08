#include "gameScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../utils/rectStuff.h"
#include "../../utils/text.h"
#include "../../interactions/textButtons.h"

SDL_Texture* background;
static TTF_Font* font;

void initGameScene(SDL_Renderer* renderer) {
    background = IMG_LoadTexture(renderer, "../scenes/Game/images/cat-test.jpg");
    font = TTF_OpenFont("../fonts/Arial.ttf", 60);
}

void renderGame(SDL_Renderer* renderer, int windowSizeX, int windowSizeY) {
    SDL_Rect temp = createRect(0, 0, windowSizeX, windowSizeY);
    SDL_RenderCopy(renderer, background, NULL, &temp);

    Text *quitBtnText = malloc(sizeof(Text));
    quitBtnText->text = "Quit";
    quitBtnText->color = createColor("DD8300", 255);
    int w = 300;
    quitBtnText->rect = createRect((windowSizeX - w) / 2, 50, w, 125);
    quitBtnText->font = font;
    makeButton(quitBtnText, createColor("FF0000", 255), "QuitBtn");


    hilightButtons();
    sdl_draw_text(renderer, quitBtnText);
}

void freeGameScene() {
    SDL_DestroyTexture(background);
}