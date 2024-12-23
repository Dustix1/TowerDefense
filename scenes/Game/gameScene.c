#include "gameScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#define M_PI 3.14159265358979323846

#include "../../utils/rectStuff.h"
#include "../../utils/text.h"
#include "../../utils/gameStatus.h"
#include "../../utils/rectStuff.h"
#include "../../interactions/buttons.h"
#include "../../interactions/rect.h"
#include "../../interactions/mouse.h"
#include "Friendly/tower.h"
#include "../End/endScene.h"
#include "Friendly/player.h"
#include "Map/map.h"
#include "Enemy/enemy.h"
#include "Enemy/waves.h"

static TTF_Font* font;
static unsigned int vanDamageAnimEndTime = 0;

Text pointFinder;
Text spawnEnemy;
SDL_Color spawnEnemyColor;

SDL_Texture* bottomUIBG;
SDL_Rect bottomUIBGRect;
SDL_Texture* woodBorder;
SDL_Rect woodBorderRectUp;
SDL_Rect woodBorderSeparatorRect;

SDL_Texture* van;

Text hpLabel;
Text hpValue;
Text moneyLabel;
Text moneyValue;
Text waveNum;

SDL_Texture* startWaveButton;
SDL_Rect startWaveButtonRect;
SDL_Color startWaveButtonColor;
Text startWaveButtonText;

void initGameScene(SDL_Renderer* renderer, SelectedMap selectedMap) {
    font = TTF_OpenFont("../fonts/lazy_dog.ttf", 120);

    loadPathTexture(renderer);
    loadGhostTextures(renderer);
    loadTowers(renderer);

    player.hp = 100;
    player.money = 99999;
    player.score = 0;

    currentWave = 0;
    loadNextWave();

    createMap(selectedMap, renderer);

    van = IMG_LoadTexture(renderer, "../scenes/Game/images/UI/van.png");

    /*pointFinder.color = createColor("FF0000", 255);
    pointFinder.font = font;
    pointFinder.rect = createRect(5, gameStatus.windowSizeY - 100, 350, 100);
    
    spawnEnemyColor = createColor("0022FF", 255);
    spawnEnemy.color = spawnEnemyColor;
    spawnEnemy.font = font;
    spawnEnemy.rect = createRect(20, 20, 300, 100);
    spawnEnemy.text = "Spawn Random Enemy";
    SDL_Color spawnEnemyHilightColor = createColor("00FF22", 255);
    makeButton(&spawnEnemy, spawnEnemy.rect, NULL, &spawnEnemyHilightColor, "spwnRndEne", TEXTBUTTON);*/

    bottomUIBG = IMG_LoadTexture(renderer, "../scenes/Game/images/UI/woodbg.jpg");
    bottomUIBGRect = createRect(0, gameStatus.windowSizeY - 225, gameStatus.windowSizeX, 225);
    woodBorder = IMG_LoadTexture(renderer, "../scenes/Game/images/UI/woodbgborder.jpg");
    woodBorderRectUp = createRect(0, gameStatus.windowSizeY - 250, gameStatus.windowSizeX, 25);
    woodBorderSeparatorRect = createRect(400, gameStatus.windowSizeY - 225, 225, 40);

    hpLabel.font = font;
    hpLabel.color = createColor("FFFFFF", 255);
    hpLabel.rect = createRect(20, gameStatus.windowSizeY - 215, 160, 50);
    hpLabel.text = "Truck Health";

    hpValue.font = font;
    hpValue.color = createColor("FF3333", 255);
    hpValue.rect = createRect(60, gameStatus.windowSizeY - 170, 75, 50);
    hpValue.text = malloc(10 * sizeof(char));

    moneyLabel.font = font;
    moneyLabel.color = createColor("FFFFFF", 255);
    moneyLabel.rect = createRect(225, gameStatus.windowSizeY - 215, 100, 50);
    moneyLabel.text = "Money";

    moneyValue.font = font;
    moneyValue.color = createColor("168118", 255);
    moneyValue.rect = createRect(230, gameStatus.windowSizeY - 170, 75, 50);
    moneyValue.text = malloc(10 * sizeof(char));

    waveNum.font = font;
    waveNum.color = createColor("FFFFFF", 255);
    waveNum.rect = createRect(30, gameStatus.windowSizeY - 105, 300, 100);
    waveNum.text = malloc(10 * sizeof(char));

    startWaveButton = IMG_LoadTexture(renderer, "../scenes/Game/images/UI/startWaveButton.png");
    startWaveButtonRect = createRect(gameStatus.windowSizeX - 215, gameStatus.windowSizeY - 215, 200, 200);
    makeButton(NULL, startWaveButtonRect, startWaveButton, NULL, "startWave", RECTBUTTON);
}

void renderGame(SDL_Renderer* renderer) {
    updateDeltaTime();
    
    // MAP RENDER
    SDL_RenderCopy(renderer, map.mapTexture, NULL, &map.mapRect);
    renderPath(renderer);

    /*SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLines(renderer, map.mapPointsWithDirections.points, 12);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);*/

    moveEnemiesTowardsCurrPoint(renderer);
    renderEnemies(renderer);

    SDL_RenderCopy(renderer, van, NULL, &map.vanRect);
    if (SDL_GetTicks64() >= vanDamageAnimEndTime) SDL_SetTextureColorMod(van, 255, 255, 255);

    dragTower();
    makeRangeFollowtower();
    checkForMoney();
    showTowerRange();
    
    // UI RENDER
    
    SDL_RenderCopy(renderer, bottomUIBG, NULL, &bottomUIBGRect);
    SDL_RenderCopy(renderer, woodBorder, NULL, &woodBorderRectUp);
    SDL_RenderCopyEx(renderer, woodBorder, NULL, &woodBorderSeparatorRect, 90, &(SDL_Point){0, 0}, SDL_FLIP_NONE);

    // TEXT & BUTTON RENDER

    highlightButtons();
    makeButtonsLookInActive();

    renderUITowerSelection(renderer);

    renderText(renderer, &hpLabel);
    sprintf(hpValue.text, "%4.1f", player.hp);
    renderText(renderer, &hpValue);
    renderText(renderer, &moneyLabel);
    sprintf(moneyValue.text, "%4d", player.money);
    renderText(renderer, &moneyValue);

    sprintf(waveNum.text, "Wave %d", currentWave + 1);
    renderText(renderer, &waveNum);

    if (isMouseOnRect(startWaveButtonRect) && searchForButton("startWave")->active) SDL_SetTextureColorMod(startWaveButton, 225, 225, 255);
    SDL_RenderCopy(renderer, startWaveButton, NULL, &startWaveButtonRect);
    SDL_SetTextureColorMod(startWaveButton, 255, 255, 255);

    // TOWER RENDER
    renderInGameTowers(renderer);

    if (player.hp <= 0) {
        freeGameScene();
        resetWaves();
        initEndScene();
        return changeScene(LOSE);
    }

    runWave(renderer);


    /*int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    double angle = atan2(temp.h / 2 - mouseY, temp.w / 2 - mouseX) * (180 / M_PI) - 90;
    SDL_RenderCopyEx(renderer, background, NULL, &temp, angle, NULL, SDL_FLIP_NONE);*/

    /*renderText(renderer, &spawnEnemy);
    spawnEnemy.color = spawnEnemyColor;




    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    char txt[50];
    snprintf(txt, sizeof(txt), "%d X %d", mouseX, mouseY);
    pointFinder.text = txt;
    renderText(renderer, &pointFinder);*/
}

void flashVan() {
    SDL_SetTextureColorMod(van, 255, 150, 150);
    vanDamageAnimEndTime = SDL_GetTicks64() + 75;
}

void freeGameScene() {
    free(hpValue.text);
    free(moneyValue.text);
    free(waveNum.text);
    freeButtons();
    SDL_DestroyTexture(map.mapTexture);
    TTF_CloseFont(font);
}