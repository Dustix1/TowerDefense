#include "tower.h"

#include <SDL2/SDL_image.h>

#include "../../../utils/rectStuff.h"
#include "../../../utils/gameStatus.h"
#include "../../../interactions/buttons.h"
#include "../../../interactions/mouse.h"
#include "../Enemy/waves.h"
#include "../Map/map.h"
#include "player.h"

bool draggingTower = false;
static Tower* draggedTower;

static Tower** towers;
static int towerSize = 75;
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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
        if (towers[i]->shouldShowRange) SDL_RenderDrawRect(renderer, &towers[i]->range);
        SDL_RenderCopy(renderer, towers[i]->texture, NULL, &towers[i]->rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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

    Tower* temp = malloc(sizeof(Tower));

    int x, y;
    SDL_GetMouseState(&x, &y);
    if (strcmp(id, "waterTower") == 0) {
        temp->type = HOLYWATERCANNON;
        temp->cost = waterTowerCost;
        temp->damage = 2;
        temp->texture = holyWaterCannonTexture;
        int range = 325;
        temp->range = createRect(x - range / 2, y - range / 2, range, range);
    } else if (strcmp(id, "incenseTower") == 0){
        temp->type = INCENSE;
        temp->cost = incenseTowerCost;
        temp->damage = 0.75f;
        temp->texture = incenseTexture;
        int range = 250;
        temp->range = createRect(x - range / 2, y - range / 2, range, range);
    }
    temp->rect = createRect(x - towerSize / 2, y - towerSize / 2, towerSize, towerSize);
    temp->shouldShowRange = true;

    towers[towerCount] = temp;
    draggedTower = temp;
    towerCount++;
    draggingTower = true;
}

void destroyTower(Tower* tower) {
    if (tower == NULL) {
        free(towers[towerCount - 1]);
        towers = realloc(towers, (towerCount - 1) * sizeof(Tower*));
    } else {
        Tower** buff = malloc((towerCount - 1) * sizeof(Tower*));
        bool wasSkipped = false;
        for (size_t i = 0; i < towerCount; i++)
        {
            if (towers[i] != tower && !wasSkipped) {
                buff[i] = towers[i];
            } else if (towers[i] != tower) {
                buff[i - 1] = towers[i];
            } else {
                wasSkipped = true;
            }
        }

        free(tower);
        free(towers);
        towers = buff;
    }
    towerCount--;
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
        if (x + towerSize / 2 > gameStatus.windowSizeX) draggedTower->rect.x = gameStatus.windowSizeX - towerSize;
        else if (x - towerSize / 2 < 0) draggedTower->rect.x = 0;
        else draggedTower->rect.x = x - towerSize / 2;

        if (y + towerSize / 2 > gameStatus.windowSizeY) draggedTower->rect.y = gameStatus.windowSizeY - towerSize;
        else if (y - towerSize / 2 < 0) draggedTower->rect.y = 0;
        else draggedTower->rect.y = y - towerSize / 2;
    }
}

void stopDragging() {
    SDL_Rect result;
    bool didIntersect = false;
    if (SDL_IntersectRect(&draggedTower->rect, &(SDL_Rect){0, gameStatus.windowSizeY - 250, gameStatus.windowSizeX, 250},
        &result) == SDL_TRUE) didIntersect = true;
    if (!didIntersect) {
        for (size_t i = 0; i < towerCount; i++)
        {
            if (SDL_IntersectRect(&draggedTower->rect, &towers[i]->rect, &result) == SDL_TRUE &&
                draggedTower != towers[i]) didIntersect = true;
        }
    }
    if (!didIntersect) {
        int i = 0;
        while (map.mapPointsWithDirections.directions[i] != -1)
        {
            if (didIntersect) break;
            if (SDL_IntersectRect(&draggedTower->rect, &map.pathRects[i], &result) == SDL_TRUE) didIntersect = true;
            i++;
        }
    }
    if (!didIntersect) {
        if (SDL_IntersectRect(&draggedTower->rect, &map.vanRect, &result) == SDL_TRUE) didIntersect = true;
    }
    

    if (didIntersect) {
        draggingTower = false;
        draggedTower = NULL;
        return destroyTower(NULL);
    }
    
    player.money -= draggedTower->cost;
    draggedTower->shouldShowRange = false;
    draggingTower = false;
    draggedTower = NULL;
    setTowerUIButtonsState(true);
}

void checkForMoney() {
    if (draggingTower) return;
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

void makeRangeFollowtower() {
    for (size_t i = 0; i < towerCount; i++)
    {
        towers[i]->range.x = (towers[i]->rect.x + towers[i]->rect.w / 2) - towers[i]->range.w / 2;
        towers[i]->range.y = (towers[i]->rect.y + towers[i]->rect.h / 2) - towers[i]->range.h / 2;
    }
}

void showTowerRange() {
    for (size_t i = 0; i < towerCount; i++)
    {
        if (isMouseOnRect(towers[i]->rect)) towers[i]->shouldShowRange = true;
        else towers[i]->shouldShowRange = false;
    }
    
}

void sellTower() {
    if (draggedTower != NULL) return;
    for (size_t i = 0; i < towerCount; i++)
    {
        if (isMouseOnRect(towers[i]->rect)) {
            addMoney(towers[i]->cost / 2);
            destroyTower(towers[i]);
        }
    }
    
}