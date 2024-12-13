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
#include "Map/map.h"
#include "Enemy/enemy.h"

static TTF_Font* font;

Text pointFinder;
Text spawnEnemy;
SDL_Color spawnEnemyColor;

void initGameScene(SDL_Renderer* renderer, SelectedMap selectedMap) {
    createMap(selectedMap, renderer);

    font = TTF_OpenFont("../fonts/Arial.ttf", 60);

    pointFinder.color = createColor("FF0000", 255);
    pointFinder.font = font;
    pointFinder.rect = createRect(20, gameStatus.windowSizeY - 100, 350, 100);
    
    spawnEnemyColor = createColor("0022FF", 255);
    spawnEnemy.color = spawnEnemyColor;
    spawnEnemy.font = font;
    spawnEnemy.rect = createRect(20, 20, 300, 100);
    spawnEnemy.text = "Spawn Random Enemy";
    makeButton(&spawnEnemy, createColor("00FF22", 255), "spwnRndEne");
}

void renderGame(SDL_Renderer* renderer) {
    updateDeltaTime();
    /*int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    double angle = atan2(temp.h / 2 - mouseY, temp.w / 2 - mouseX) * (180 / M_PI) - 90;
    SDL_RenderCopyEx(renderer, background, NULL, &temp, angle, NULL, SDL_FLIP_NONE);*/
    SDL_RenderCopy(renderer, map.mapTexture, NULL, &map.mapRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLines(renderer, map.mapPointsWithDirections.points, 12);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    char txt[50];
    snprintf(txt, sizeof(txt), "%d X %d", mouseX, mouseY);
    pointFinder.text = txt;
    renderText(renderer, &pointFinder);

    moveEnemiesTowardsCurrPoint(renderer);
    renderEnemies(renderer);
    
    renderText(renderer, &spawnEnemy);
    spawnEnemy.color = spawnEnemyColor;

    highlightButtons();
}

void freeGameScene() {
    SDL_DestroyTexture(map.mapTexture);
    TTF_CloseFont(font);
}