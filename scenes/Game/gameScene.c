#include "gameScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#define M_PI 3.14159265358979323846

#include "../../utils/rectStuff.h"
#include "../../utils/text.h"
#include "../../interactions/textButtons.h"
#include "../../interactions/rect.h"
#include "../../utils/gameStatus.h"
#include "../../utils/rectStuff.h"
#include "Friendly/player.h"
#include "Map/map.h"
#include "Enemy/enemy.h"

static TTF_Font* font;

Text pointFinder;
Text spawnEnemy;
SDL_Color spawnEnemyColor;

SDL_Texture* bottomUIBG;
SDL_Rect bottomUIBGRect;
SDL_Texture* woodBorder;
SDL_Rect woodBorderRectUp;
SDL_Rect woodBorderSeparatorRect;

SDL_Texture* van;
SDL_Rect vanRect;

Text hpLabel;
Text hpValue;
Text moneyLabel;
Text moneyValue;

void initGameScene(SDL_Renderer* renderer, SelectedMap selectedMap) {
    player.hp = 500;
    player.money = 50;

    createMap(selectedMap, renderer);

    font = TTF_OpenFont("../fonts/Arial.ttf", 60);

    pointFinder.color = createColor("FF0000", 255);
    pointFinder.font = font;
    pointFinder.rect = createRect(5, gameStatus.windowSizeY - 100, 350, 100);

    van = IMG_LoadTexture(renderer, "../scenes/Game/images/UI/van.png");
    vanRect = createRect(gameStatus.windowSizeX - 550, gameStatus.windowSizeY - 450, 350, 200);
    
    spawnEnemyColor = createColor("0022FF", 255);
    spawnEnemy.color = spawnEnemyColor;
    spawnEnemy.font = font;
    spawnEnemy.rect = createRect(20, 20, 300, 100);
    spawnEnemy.text = "Spawn Random Enemy";
    makeButton(&spawnEnemy, createColor("00FF22", 255), "spwnRndEne");

    bottomUIBG = IMG_LoadTexture(renderer, "../scenes/Game/images/UI/woodbg.jpg");
    bottomUIBGRect = createRect(0, gameStatus.windowSizeY - 225, gameStatus.windowSizeX, 225);
    woodBorder = IMG_LoadTexture(renderer, "../scenes/Game/images/UI/woodbgborder.jpg");
    woodBorderRectUp = createRect(0, gameStatus.windowSizeY - 250, gameStatus.windowSizeX, 25);
    woodBorderSeparatorRect = createRect(400, gameStatus.windowSizeY - 225, 225, 40);

    hpLabel.font = font;
    hpLabel.color = createColor("FFFFFF", 255);
    hpLabel.rect = createRect(20, gameStatus.windowSizeY - 215, 160, 50);
    hpLabel.text = "Tower Health";

    hpValue.font = font;
    hpValue.color = createColor("FF3333", 255);
    hpValue.rect = createRect(60, gameStatus.windowSizeY - 170, 75, 50);
    hpValue.text = malloc(5 * sizeof(char));

    moneyLabel.font = font;
    moneyLabel.color = createColor("FFFFFF", 255); // 168118
    moneyLabel.rect = createRect(225, gameStatus.windowSizeY - 215, 100, 50);
    moneyLabel.text = "Money";

    moneyValue.font = font;
    moneyValue.color = createColor("168118", 255);
    moneyValue.rect = createRect(230, gameStatus.windowSizeY - 170, 75, 50);
    moneyValue.text = malloc(5 * sizeof(char));
}

void renderGame(SDL_Renderer* renderer) {
    updateDeltaTime();
    /*int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    double angle = atan2(temp.h / 2 - mouseY, temp.w / 2 - mouseX) * (180 / M_PI) - 90;
    SDL_RenderCopyEx(renderer, background, NULL, &temp, angle, NULL, SDL_FLIP_NONE);*/
    SDL_RenderCopy(renderer, map.mapTexture, NULL, &map.mapRect);

    /*SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLines(renderer, map.mapPointsWithDirections.points, 12);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);*/

    moveEnemiesTowardsCurrPoint(renderer);
    renderEnemies(renderer);

    SDL_RenderCopy(renderer, van, NULL, &vanRect);
    
    // UI RENDER
    
    SDL_RenderCopy(renderer, bottomUIBG, NULL, &bottomUIBGRect);
    SDL_RenderCopy(renderer, woodBorder, NULL, &woodBorderRectUp);
    SDL_RenderCopyEx(renderer, woodBorder, NULL, &woodBorderSeparatorRect, 90, &(SDL_Point){0, 0}, SDL_FLIP_NONE);

    // TEXT & BUTTON RENDER

    renderText(renderer, &hpLabel);
    sprintf(hpValue.text, "%4.1f", player.hp);
    renderText(renderer, &hpValue);
    renderText(renderer, &moneyLabel);
    sprintf(moneyValue.text, "%4d", player.money);
    renderText(renderer, &moneyValue);







    renderText(renderer, &spawnEnemy);
    spawnEnemy.color = spawnEnemyColor;

    highlightButtons();




    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    char txt[50];
    snprintf(txt, sizeof(txt), "%d X %d", mouseX, mouseY);
    pointFinder.text = txt;
    renderText(renderer, &pointFinder);
}

void freeGameScene() {
    free(hpValue.text);
    free(moneyValue.text);
    SDL_DestroyTexture(map.mapTexture);
    TTF_CloseFont(font);
}