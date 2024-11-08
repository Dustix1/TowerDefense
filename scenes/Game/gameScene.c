#include "gameScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../utils/rectStuff.h"
#include "../../utils/text.h"

SDL_Texture* background;
static TTF_Font* font;

void initGameScene(SDL_Renderer* renderer) {
    background = IMG_LoadTexture(renderer, "../scenes/Game/images/cat-test.jpg");
    font = TTF_OpenFont("../fonts/Arial.ttf", 60);
}

void renderGame(SDL_Renderer* renderer, int windowSizeX, int windowSizeY) {
    SDL_Rect temp = createRect(0, 0, windowSizeX, windowSizeY);
    SDL_RenderCopy(renderer, background, NULL, &temp);
}

void freeGameScene() {
    SDL_DestroyTexture(background);
}