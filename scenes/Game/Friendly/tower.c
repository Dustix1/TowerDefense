#include "tower.h"

#include <SDL2/SDL_image.h>
#include <string.h>

#include "../../../utils/rectStuff.h"
#include "../../../utils/gameStatus.h"
#include "../../../interactions/buttons.h"
#include "../../../interactions/mouse.h"
#include "../../../utils/text.h"
#include "../Enemy/waves.h"
#include "../Map/map.h"
#include "player.h"

bool draggingTower = false;
static Tower* draggedTower;

static Tower** towers;
static int towerSize = 75;
static int towerCount = 0;

static Projectile** projectiles = NULL;
static int projectileCount = 0;

static int waterTowerCost = 15;
static int incenseTowerCost = 45;
static int crucifixTowerCost = 40;

static int animCrucifixAimTime = 0;

static SDL_Texture* holyWaterCannonTexture;
static SDL_Rect holyWaterCannonUIRect;
static SDL_Texture* incenseTexture;
static SDL_Rect incenseUIRect;
static SDL_Texture* crucifixTexture;
static SDL_Rect crucifixUIRect;

static Text holyWaterCannonCostText;
static Text incenseCostText;
static Text crucifixCostText;

static SDL_Texture* waterProjectileTexture;

void loadTowers(SDL_Renderer* renderer, TTF_Font* font) {
    holyWaterCannonTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/HolyWaterCannon.png");
    incenseTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/IncenseTower.png");
    crucifixTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/CrucifixTower.png");
    waterProjectileTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/WaterProjectile.png");

    int wh = 200;

    holyWaterCannonUIRect = createRect(450, gameStatus.windowSizeY - 215, 150, wh);
    crucifixUIRect = createRect(holyWaterCannonUIRect.x + wh + 50, holyWaterCannonUIRect.y, 150, wh);
    incenseUIRect = createRect(crucifixUIRect.x + wh + 50, crucifixUIRect.y, wh, wh);

    makeButton(NULL, holyWaterCannonUIRect, holyWaterCannonTexture, NULL, "waterTower", RECTBUTTON);
    makeButton(NULL, incenseUIRect, incenseTexture, NULL, "incenseTower", RECTBUTTON);
    makeButton(NULL, crucifixUIRect, crucifixTexture, NULL, "crucifixTower", RECTBUTTON);

    holyWaterCannonCostText.font = font;
    holyWaterCannonCostText.color = createColor("FFFF00", 200);
    holyWaterCannonCostText.rect = createRect(holyWaterCannonUIRect.x + holyWaterCannonUIRect.w + 5,
                                                holyWaterCannonUIRect.y + holyWaterCannonUIRect.h - 50, 50, 50);
    holyWaterCannonCostText.text = malloc(12 * sizeof(char));
    sprintf(holyWaterCannonCostText.text, "%d", waterTowerCost);

    crucifixCostText.font = font;
    crucifixCostText.color = createColor("FFFF00", 200);
    crucifixCostText.rect = createRect(crucifixUIRect.x + crucifixUIRect.w + 5,
                                                crucifixUIRect.y + crucifixUIRect.h - 50, 50, 50);
    crucifixCostText.text = malloc(12 * sizeof(char));
    sprintf(crucifixCostText.text, "%d", crucifixTowerCost);

    incenseCostText.font = font;
    incenseCostText.color = createColor("FFFF00", 200);
    incenseCostText.rect = createRect(incenseUIRect.x + incenseUIRect.w + 5,
                                                incenseUIRect.y + incenseUIRect.h - 50, 50, 50);
    incenseCostText.text = malloc(12 * sizeof(char));
    sprintf(incenseCostText.text, "%d", incenseTowerCost);
}

void renderInGameTowers(SDL_Renderer* renderer) {
    if (towerCount == 0) return;
    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
        if (draggedTower != NULL) towers[i]->shouldShowRange = true;

        switch (towers[i]->type)
        {
        case HOLYWATERCANNON:
            if (draggedTower == towers[i] || towers[i]->currentTarget == NULL) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 175);
                if (towers[i]->shouldShowRange) {
                    SDL_SetTextureColorMod(towers[i]->texture, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &towers[i]->range);
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &towers[i]->rect);
                } else SDL_SetTextureColorMod(towers[i]->texture, 200, 200, 200);
                SDL_RenderCopy(renderer, towers[i]->texture, NULL, &towers[i]->rect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 175);
                if (towers[i]->shouldShowRange) {
                    SDL_SetTextureColorMod(towers[i]->texture, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &towers[i]->range);
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &towers[i]->rect);
                } else SDL_SetTextureColorMod(towers[i]->texture, 200, 200, 200);
                SDL_FRect targetFRect = (*towers[i]->currentTarget)->rect;
                SDL_Rect targetRect = createRect(round(targetFRect.x), round(targetFRect.y), round(targetFRect.w), round(targetFRect.h));
                SDL_RenderCopyEx(renderer, towers[i]->texture, NULL, &towers[i]->rect, calculateRotationAngle(towers[i]->rect, targetRect), NULL, SDL_FLIP_NONE);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            break;
        
        case INCENSE:
        case CRUCIFIX:
            if (towers[i]->type == INCENSE) SDL_SetRenderDrawColor(renderer, 0, 0, 255, 175);
            else SDL_SetRenderDrawColor(renderer, 255, 255, 0, 175);
            if (towers[i]->shouldShowRange) {
                SDL_SetTextureColorMod(towers[i]->texture, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &towers[i]->range);
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &towers[i]->rect);
            } else SDL_SetTextureColorMod(towers[i]->texture, 200, 200, 200);
            SDL_RenderCopy(renderer, towers[i]->texture, NULL, &towers[i]->rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        break;
        }
    }
}

void renderUITowerSelection(SDL_Renderer* renderer) {
    if (searchForButton("waterTower")->active) SDL_SetTextureColorMod(holyWaterCannonTexture, 200, 200, 200);
    if (isMouseOnRect(holyWaterCannonUIRect) && searchForButton("waterTower")->active) SDL_SetTextureColorMod(holyWaterCannonTexture, 255, 255, 255);
    SDL_RenderCopy(renderer, holyWaterCannonTexture, NULL, &holyWaterCannonUIRect);

    if (searchForButton("crucifixTower")->active) SDL_SetTextureColorMod(crucifixTexture, 200, 200, 200);
    if (isMouseOnRect(crucifixUIRect) && searchForButton("crucifixTower")->active) SDL_SetTextureColorMod(crucifixTexture, 255, 255, 255);
    SDL_RenderCopy(renderer, crucifixTexture, NULL, &crucifixUIRect);

    if (searchForButton("incenseTower")->active) SDL_SetTextureColorMod(incenseTexture, 200, 200, 200);
    if (isMouseOnRect(incenseUIRect) && searchForButton("incenseTower")->active) SDL_SetTextureColorMod(incenseTexture, 255, 255, 255);
    SDL_RenderCopy(renderer, incenseTexture, NULL, &incenseUIRect);

    renderText(renderer, &holyWaterCannonCostText);
    renderText(renderer, &crucifixCostText);
    renderText(renderer, &incenseCostText);
}

void renderProjectiles(SDL_Renderer* renderer) {
    for (size_t i = 0; i < projectileCount; i++)
    {
        if (projectiles[i] == NULL) continue;
        SDL_FRect projectileFRect = projectiles[i]->rect;
        SDL_Rect projectileRect = createRect(round(projectileFRect.x), round(projectileFRect.y),
                                            round(projectileFRect.w), round(projectileFRect.h));
        SDL_RenderCopyEx(renderer, projectiles[i]->texture, NULL, &projectileRect, projectiles[i]->rotationAngle, NULL, SDL_FLIP_VERTICAL);  
    }
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
        temp->baseDamage = 1;
        temp->damage = temp->baseDamage;
        temp->texture = holyWaterCannonTexture;
        int range = 250;
        temp->range = createRect(x - range / 2, y - range / 2, range, range);
        temp->rect = createRect(x - towerSize / 2, y - towerSize / 2, towerSize - 10, towerSize + 7);
    } else if (strcmp(id, "incenseTower") == 0) {
        temp->type = INCENSE;
        temp->cost = incenseTowerCost;
        temp->baseDamage = 0;
        temp->damage = temp->baseDamage;
        temp->texture = incenseTexture;
        int range = 200;
        temp->range = createRect(x - range / 2, y - range / 2, range, range);
        temp->rect = createRect(x - towerSize / 2, y - towerSize / 2, towerSize, towerSize);
    }else if (strcmp(id, "crucifixTower") == 0) {
        temp->type = CRUCIFIX;
        temp->cost = crucifixTowerCost;
        temp->baseDamage = 1;
        temp->damage = temp->baseDamage;
        temp->texture = crucifixTexture;
        int range = 217;
        temp->range = createRect(x - range / 2, y - range / 2, range, range);
        temp->rect = createRect(x - towerSize / 2, y - towerSize / 2, towerSize - 15, towerSize + 7);
    }
    temp->currentTarget = NULL;
    temp->enemyTargets = NULL;
    temp->towerTargets = NULL;
    temp->nextTimeOfAttack = 0;
    temp->enemyTargetCount = 0;
    temp->towerTargetCount = 0;
    temp->shouldShowRange = true;
    temp->buffed = 0;

    towers[towerCount] = temp;
    draggedTower = temp;
    towerCount++;
    draggingTower = true;
}

void destroyTower(Tower* tower) {
    for (size_t i = 0; i < towerCount; i++)
    {
        if (tower == NULL) {
            free(towers[towerCount - 1]);
            towers[towerCount - 1] = NULL;
        } else {
            if (towers[i] == NULL) continue;
            if (towers[i] == tower) {
                free(towers[i]);
                towers[i] = NULL;
            }
        }
    }
}

void setTowerUIButtonsState(bool state) {
    Button* waterTowerButton = searchForButton("waterTower");
    if (waterTowerButton != NULL) waterTowerButton->active = state;
    Button* incenseTowerButton = searchForButton("incenseTower");
    if (incenseTowerButton != NULL) incenseTowerButton->active = state;
    Button* crucifixTowerButton = searchForButton("crucifixTower");
    if (crucifixTowerButton != NULL) crucifixTowerButton->active = state;
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
            if (towers[i] == NULL) continue;
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
    Button* crucifixTowerButton = searchForButton("crucifixTower");

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

    if (player.money < crucifixTowerCost) {
        if (crucifixTowerButton != NULL) crucifixTowerButton->active = false;
    } else {
        if (crucifixTowerButton != NULL) crucifixTowerButton->active = true;
    }
}

void makeRangeFollowtower() {
    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
        towers[i]->range.x = (towers[i]->rect.x + towers[i]->rect.w / 2) - towers[i]->range.w / 2;
        towers[i]->range.y = (towers[i]->rect.y + towers[i]->rect.h / 2) - towers[i]->range.h / 2;
    }
}

void showTowerRange() {
    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
        if (isMouseOnRect(towers[i]->rect)) towers[i]->shouldShowRange = true;
        else towers[i]->shouldShowRange = false;
    }
    
}

void sellTower() {
    if (draggedTower != NULL) return;
    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
        if (isMouseOnRect(towers[i]->rect)) {
            addMoney(towers[i]->cost / 2);
            destroyTower(towers[i]);
        }
    }
    
}

void getTarget() {
    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
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
                if (enemies[j] == NULL) continue;
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
                if (towers[j] == NULL) continue;
                if (towers[j] == draggedTower || towers[j]->type == INCENSE) continue;
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
                if (enemies[j] == NULL) continue;
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

void buffTowers(SDL_Renderer* renderer) {
    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
        if (towers[i] == draggedTower) continue;
        if (towers[i]->type == INCENSE) {
            for (size_t j = 0; j < towers[i]->towerTargetCount; j++)
            {
                towers[i]->towerTargets[j]->buffed++;
            }
        }
    }

    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
        if (towers[i] == draggedTower) continue;
        if (towers[i]->type == INCENSE) {
            for (size_t j = 0; j < towers[i]->towerTargetCount; j++)
            {
                if (towers[i]->towerTargets[j]->buffed > 1) towers[i]->towerTargets[j]->damage = towers[i]->towerTargets[j]->baseDamage * (0.75f * towers[i]->towerTargets[j]->buffed);
                else if (towers[i]->towerTargets[j]->buffed == 1) towers[i]->towerTargets[j]->damage = towers[i]->towerTargets[j]->baseDamage + 0.5f;
                else towers[i]->towerTargets[j]->damage = towers[i]->towerTargets[j]->baseDamage;
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawLine(renderer, towers[i]->rect.x + towers[i]->rect.w / 2, towers[i]->rect.y + towers[i]->rect.h / 2,
                towers[i]->towerTargets[j]->rect.x + towers[i]->towerTargets[j]->rect.w / 2, towers[i]->towerTargets[j]->rect.y + towers[i]->towerTargets[j]->rect.h / 2);
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
    }

    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
        if (towers[i] == draggedTower) continue;
        if (towers[i]->type == INCENSE) {
            for (size_t j = 0; j < towers[i]->towerTargetCount; j++)
            {
                towers[i]->towerTargets[j]->buffed = 0;
            }
        }
    }
}

void makeTowersDoSomething(SDL_Renderer* renderer) {
    for (size_t i = 0; i < towerCount; i++)
    {
        if (towers[i] == NULL) continue;
        if (towers[i] == draggedTower) continue;
        switch (towers[i]->type)
        {
        case CRUCIFIX:
            for (size_t j = 0; j < towers[i]->enemyTargetCount; j++)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                    if (SDL_GetTicks64() >= towers[i]->nextTimeOfAttack) {
                        damageEnemy(towers[i]->enemyTargets[j], towers[i]->damage);
                        towers[i]->nextTimeOfAttack = SDL_GetTicks64() + 500;
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    }
                    SDL_RenderDrawLine(renderer, towers[i]->rect.x + towers[i]->rect.w / 2, towers[i]->rect.y + towers[i]->rect.h / 2,
                    towers[i]->enemyTargets[j]->rect.x + towers[i]->enemyTargets[j]->rect.w / 2, towers[i]->enemyTargets[j]->rect.y + towers[i]->enemyTargets[j]->rect.h / 2);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            break;

        case HOLYWATERCANNON:
                if (SDL_GetTicks64() >= towers[i]->nextTimeOfAttack) {
                    towers[i]->nextTimeOfAttack = SDL_GetTicks64() + 250;
                    if (towers[i]->currentTarget == NULL) break;
                    projectiles = realloc(projectiles, (projectileCount + 1) * sizeof(Projectile*));
                    Projectile* buff = malloc(sizeof(Projectile));

                    buff->rect = createFRect(towers[i]->rect.x + towers[i]->rect.w / 2, towers[i]->rect.y + towers[i]->rect.h / 2, 25, 25);
                    buff->texture = waterProjectileTexture;
                    buff->type = NORMALPROJECTILE;
                    buff->damage = towers[i]->damage;
                    SDL_FRect targetFRect = (*towers[i]->currentTarget)->rect;
                    SDL_Rect targetRect = createRect(round(targetFRect.x), round(targetFRect.y), round(targetFRect.w), round(targetFRect.h));
                    buff->rotationAngle = calculateRotationAngle(towers[i]->rect, targetRect);
                    double radianAngle = calculateRadiansFromAngle(buff->rotationAngle - 90);
                    buff->directionVector = (SDL_FPoint){cos(radianAngle), sin(radianAngle)};

                    projectiles[projectileCount] = buff;
                    projectileCount++;
                }
            break;
        }
    }    
}

void moveProjectiles() {
    for (size_t i = 0; i < projectileCount; i++)
    {
        if (projectiles[i] == NULL) continue;
        SDL_FRect* projRect = &projectiles[i]->rect;
        projRect->x += (projectiles[i]->directionVector.x * 3000.0) * gameStatus.deltaTime;
        projRect->y += (projectiles[i]->directionVector.y * 3000.0) * gameStatus.deltaTime;
    }
}

void checkForProjectileCollision() {
    SDL_FRect result;
    for (size_t i = 0; i < projectileCount; i++)
    {
        if (projectiles[i] == NULL) continue;
        SDL_FRect projFRect = projectiles[i]->rect;
        if (projFRect.x >= gameStatus.windowSizeX || projFRect.x <= 0 ||
            projFRect.y >= gameStatus.windowSizeY || projFRect.y <= 0) {
                free(projectiles[i]);
                projectiles[i] = NULL;
                continue;
            }
        for (size_t j = 0; j < enemyCount; j++)
        {
            if (enemies[j] == NULL) continue;
            if (projectiles[i] == NULL) continue;
            if (SDL_IntersectFRect(&projectiles[i]->rect, &enemies[j]->rect, &result) == SDL_TRUE) {
                damageEnemy(enemies[j], projectiles[i]->damage);
                free(projectiles[i]);
                projectiles[i] = NULL;
            }
        }
    }
}


void freeTowers() {
    draggingTower = false;
    for (size_t i = 0; i < towerCount; i++)
    {   
        if (towers[i] == NULL) continue;
        free(towers[i]);
    }
    towerCount = 0;
    free(towers);
    towers = NULL;

    for (size_t i = 0; i < projectileCount; i++)
    {
        if (projectiles[i] == NULL) continue;
        free(projectiles[i]);
    }
    projectileCount = 0;
    free(projectiles);
    projectiles = NULL;
    draggedTower = NULL;

    free(holyWaterCannonCostText.text);
    free(crucifixCostText.text);
    free(incenseCostText.text);
    holyWaterCannonCostText.text = NULL;
    crucifixCostText.text = NULL;
    incenseCostText.text = NULL;

    SDL_DestroyTexture(holyWaterCannonTexture);
    SDL_DestroyTexture(crucifixTexture);
    SDL_DestroyTexture(incenseTexture);
    SDL_DestroyTexture(waterProjectileTexture);
}