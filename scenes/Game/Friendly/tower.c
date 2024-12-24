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
static int crucifixTowerCost = 30;

static SDL_Texture* holyWaterCannonTexture;
static SDL_Rect holyWaterCannonUIRect;
static SDL_Texture* incenseTexture;
static SDL_Rect incenseUIRect;
static SDL_Texture* crucifixTexture;
static SDL_Rect crucifixUIRect;

static SDL_Texture* waterProjectileTexture;

void loadTowers(SDL_Renderer* renderer) {
    holyWaterCannonTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/cat.png");
    incenseTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/missingTexture.jpg");
    crucifixTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/maps/path.png");
    waterProjectileTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/WaterProjectile.png");

    int wh = 200;

    holyWaterCannonUIRect = createRect(450, gameStatus.windowSizeY - 215, wh, wh);
    crucifixUIRect = createRect(holyWaterCannonUIRect.x + wh + 50, holyWaterCannonUIRect.y, wh, wh); 
    incenseUIRect = createRect(crucifixUIRect.x + wh + 50, crucifixUIRect.y, wh, wh);

    makeButton(NULL, holyWaterCannonUIRect, holyWaterCannonTexture, NULL, "waterTower", RECTBUTTON);
    makeButton(NULL, incenseUIRect, incenseTexture, NULL, "incenseTower", RECTBUTTON);
    makeButton(NULL, crucifixUIRect, crucifixTexture, NULL, "crucifixTower", RECTBUTTON);
}

void renderInGameTowers(SDL_Renderer* renderer) {
    if (towerCount == 0) return;
    for (size_t i = 0; i < towerCount; i++)
    {
        switch (towers[i]->type)
        {
        case HOLYWATERCANNON:
            if (draggedTower == towers[i] || towers[i]->currentTarget == NULL) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                if (towers[i]->shouldShowRange) {
                    SDL_SetTextureColorMod(towers[i]->texture, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &towers[i]->range);
                } else SDL_SetTextureColorMod(towers[i]->texture, 200, 200, 200);
                SDL_RenderCopy(renderer, towers[i]->texture, NULL, &towers[i]->rect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                if (towers[i]->shouldShowRange) {
                    SDL_SetTextureColorMod(towers[i]->texture, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &towers[i]->range);
                } else SDL_SetTextureColorMod(towers[i]->texture, 200, 200, 200);
                SDL_FRect targetFRect = (*towers[i]->currentTarget)->rect;
                SDL_Rect targetRect = createRect(round(targetFRect.x), round(targetFRect.y), round(targetFRect.w), round(targetFRect.h));
                SDL_RenderCopyEx(renderer, towers[i]->texture, NULL, &towers[i]->rect, calculateRotationAngle(towers[i]->rect, targetRect), NULL, SDL_FLIP_NONE);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            break;
        
        case INCENSE:
        case CRUCIFIX:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
            if (towers[i]->shouldShowRange) {
                SDL_SetTextureColorMod(towers[i]->texture, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &towers[i]->range);
            } else SDL_SetTextureColorMod(towers[i]->texture, 200, 200, 200);
            SDL_RenderCopy(renderer, towers[i]->texture, NULL, &towers[i]->rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        break;
        }
    }
}

void renderUITowerSelection(SDL_Renderer* renderer) {
    if (isMouseOnRect(holyWaterCannonUIRect) && searchForButton("waterTower")->active) SDL_SetTextureColorMod(holyWaterCannonTexture, 255, 255, 255);
    else  SDL_SetTextureColorMod(holyWaterCannonTexture, 200, 200, 200);
    SDL_RenderCopy(renderer, holyWaterCannonTexture, NULL, &holyWaterCannonUIRect);

    if (isMouseOnRect(crucifixUIRect) && searchForButton("crucifixTower")->active) SDL_SetTextureColorMod(crucifixTexture, 255, 255, 255);
    else SDL_SetTextureColorMod(crucifixTexture, 200, 200, 200);
    SDL_RenderCopy(renderer, crucifixTexture, NULL, &crucifixUIRect);

    if (isMouseOnRect(incenseUIRect) && searchForButton("incenseTower")->active) SDL_SetTextureColorMod(incenseTexture, 255, 255, 255);
    else SDL_SetTextureColorMod(incenseTexture, 200, 200, 200);
    SDL_RenderCopy(renderer, incenseTexture, NULL, &incenseUIRect);
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
        temp->baseDamage = 2;
        temp->damage = 2;
        temp->texture = holyWaterCannonTexture;
        int range = 325;
        temp->range = createRect(x - range / 2, y - range / 2, range, range);
    } else if (strcmp(id, "incenseTower") == 0) {
        temp->type = INCENSE;
        temp->cost = incenseTowerCost;
        temp->baseDamage = 0;
        temp->damage = 0;
        temp->texture = incenseTexture;
        int range = 300;
        temp->range = createRect(x - range / 2, y - range / 2, range, range);
    }else if (strcmp(id, "crucifixTower") == 0) {
        temp->type = CRUCIFIX;
        temp->cost = crucifixTowerCost;
        temp->baseDamage = 0.75f;
        temp->damage = 0.75f;
        temp->texture = crucifixTexture;
        int range = 250;
        temp->range = createRect(x - range / 2, y - range / 2, range, range);
    }
    temp->currentTarget = NULL;
    temp->enemyTargets = NULL;
    temp->towerTargets = NULL;
    temp->nextTimeOfAttack = 0;
    temp->enemyTargetCount = 0;
    temp->towerTargetCount = 0;

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

void getTarget() {
    for (size_t i = 0; i < towerCount; i++)
    {
        Tower* tower = towers[i];
        if (tower == draggedTower) continue;
        tower->currentTarget = NULL;
        if (tower->enemyTargetCount > 0) free(tower->enemyTargets);
        if (tower->towerTargetCount > 0) free(tower->towerTargets);
        tower->enemyTargets = NULL;
        tower->towerTargets = NULL;
        tower->towerTargetCount = 0;
        tower->enemyTargetCount = 0;

        SDL_Rect result;

        switch (tower->type)
        {
        case HOLYWATERCANNON:
            for (size_t j = 0; j < enemyCount; j++)
            {
                if (tower->currentTarget != NULL) break;
                SDL_FRect enemyFRect = enemies[j]->rect;
                SDL_Rect enemyRect = createRect(round(enemyFRect.x), round(enemyFRect.y), round(enemyFRect.w), round(enemyFRect.h));
                if(SDL_IntersectRect(&tower->range, &enemyRect, &result) == SDL_TRUE) {
                    tower->currentTarget = &enemies[j];
                }
            }
            break;
        
        case INCENSE:
            for (size_t j = 0; j < towerCount; j++)
            {
                if (towers[j] == draggedTower) continue;
                if (SDL_IntersectRect(&tower->range, &towers[j]->rect, &result) == SDL_TRUE && tower != towers[j]) {
                    tower->towerTargets = realloc(tower->towerTargets, (tower->towerTargetCount + 1) * sizeof(Tower*));
                    tower->towerTargets[tower->towerTargetCount] = towers[j];
                    tower->towerTargetCount++; 
                }
            }
            break;

        case CRUCIFIX:
            for (size_t j = 0; j < enemyCount; j++)
            {
                SDL_FRect enemyFRect = enemies[j]->rect;
                SDL_Rect enemyRect = createRect(round(enemyFRect.x), round(enemyFRect.y), round(enemyFRect.w), round(enemyFRect.h));
                if (SDL_IntersectRect(&tower->range, &enemyRect, &result) == SDL_TRUE) {
                    tower->enemyTargets = realloc(tower->enemyTargets, (tower->enemyTargetCount + 1) * sizeof(Tower*));
                    tower->enemyTargets[tower->enemyTargetCount] = enemies[j];
                    tower->enemyTargetCount++;
                }
            }
            break;
        }
    }
}

void makeTowersDoSomething() {
    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == draggedTower) continue;
        switch (towers[i]->type)
        {
        case CRUCIFIX:
            if (SDL_GetTicks64() >= towers[i]->nextTimeOfAttack) {
                printf("%d\n", towers[i]->enemyTargetCount);
                for (size_t j = 0; j < towers[i]->enemyTargetCount; j++)
                {
                    damageEnemy(towers[i]->enemyTargets[j], towers[i]->damage);
                }
                towers[i]->nextTimeOfAttack = SDL_GetTicks64() + 450;
            }
            break;
        
        case INCENSE:
            for (size_t j = 0; j < towers[i]->towerTargetCount; j++)
            {
                towers[i]->towerTargets[j]->damage = towers[i]->towerTargets[j]->baseDamage + 1.25f;
            }
            break;

        case HOLYWATERCANNON:
            break;
        default:
            break;
        }
    }
    
}

void freeTowers() {
    draggingTower = false;
    for (size_t i = 0; i < towerCount; i++)
    {
        free(towers[i]);
    }
    towerCount = 0;
    free(towers);
    towers = NULL;
    draggedTower = NULL;
}