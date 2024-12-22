#include "tower.h"

#include <SDL2/SDL_image.h>

#include "../../../utils/rectStuff.h"
#include "../../../utils/gameStatus.h"
#include "../../../interactions/buttons.h"
#include "../../../interactions/mouse.h"
#include "player.h"

bool draggingTower = false;
static Tower* draggedTower;

static Tower** towers;
static int towerCount = 0;

static int waterTowerCost = 20;
static int incenseTowerCost = 25;

static SDL_Texture* holyWaterCannonTexture;
static SDL_Rect holyWaterCannonUIRect;
static SDL_Texture* incenseTexture;
static SDL_Rect incenseUIRect;

void loadTowers(SDL_Renderer* renderer) {
    holyWaterCannonTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/cat.png");
    incenseTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/missingTexture.jpg");

    int wh = 200;

    holyWaterCannonUIRect = createRect(450, gameStatus.windowSizeY - 215, wh, wh);
    incenseUIRect = createRect(holyWaterCannonUIRect.x + wh + 50, holyWaterCannonUIRect.y, wh, wh);

    makeButton(NULL, holyWaterCannonUIRect, holyWaterCannonTexture, NULL, "waterTower", RECTBUTTON);
    makeButton(NULL, incenseUIRect, incenseTexture, NULL, "incenseTower", RECTBUTTON);
}

void renderInGameTowers(SDL_Renderer* renderer) {
    if (towerCount == 0) return;

    for (size_t i = 0; i < towerCount; i++)
    {
        SDL_RenderCopy(renderer, towers[i]->texture, NULL, &towers[i]->rect);
    }
}

void renderUITowerSelection(SDL_Renderer* renderer) {
    if (isMouseOnRect(holyWaterCannonUIRect) && searchForButton("waterTower")->active) SDL_SetTextureColorMod(holyWaterCannonTexture, 255, 255, 255);
    SDL_RenderCopy(renderer, holyWaterCannonTexture, NULL, &holyWaterCannonUIRect);
    SDL_SetTextureColorMod(holyWaterCannonTexture, 200, 200, 200);

    if (isMouseOnRect(incenseUIRect) && searchForButton("incenseTower")->active) SDL_SetTextureColorMod(incenseTexture, 255, 255, 255);
    SDL_RenderCopy(renderer, incenseTexture, NULL, &incenseUIRect);
    SDL_SetTextureColorMod(incenseTexture, 200, 200, 200);
}

void createTower(char* id) {
    Tower** tmp = realloc(towers, (towerCount + 1) * sizeof(Tower*));
    towers = tmp;

    Tower* temp = malloc(sizeof(Button));

    int x, y;
    SDL_GetMouseState(&x, &y);
    if (strcmp(id, "waterTower") == 0) {
        temp->type = HOLYWATERCANNON;
        temp->cost = waterTowerCost;
        temp->damage = 2;
        temp->texture = holyWaterCannonTexture;
    } else if (strcmp(id, "incenseTower") == 0){
        temp->type = INCENSE;
        temp->cost = incenseTowerCost;
        temp->damage = 0.75f;
        temp->texture = incenseTexture;
    }
    temp->rect = createRect(x - 50, y - 50, 100, 100);

    towers[towerCount] = temp;
    draggedTower = temp;
    towerCount++;
    draggingTower = true;
    player.money -= temp->cost;
}

void setTowerUIButtonsState(bool state) {
    Button* waterTowerButton = searchForButton("waterTower");
    if (waterTowerButton != NULL) waterTowerButton->active = state;
    Button* incenseTowerButton = searchForButton("incenseTower");
    if (incenseTowerButton != NULL) incenseTowerButton->active = state;
}

void dragTower() {
    if (draggedTower != NULL) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        draggedTower->rect.x = x - 50;
        draggedTower->rect.y = y - 50;
    }
}

void stopDragging() {
    draggingTower = false;
    draggedTower = NULL;
    setTowerUIButtonsState(true);
}

void checkForMoney() {
    Button* waterTowerButton = searchForButton("waterTower");
    Button* incenseTowerButton = searchForButton("incenseTower");

    if (player.money < waterTowerCost) {
        if (waterTowerButton != NULL) waterTowerButton->active = false;
    } else {
        if (waterTowerButton != NULL) waterTowerButton->active = true;
    }

    if (player.money < incenseTowerCost) {
        if (incenseTowerButton != NULL) incenseTowerButton->active = false;
    } else {
        if (incenseTowerButton != NULL) incenseTowerButton->active = true;
    }
}