#include "enemy.h"

#include <stdbool.h>
#include <math.h>

#include "../Map/map.h"
#include "../../../utils/gameStatus.h"
#include "../Friendly/player.h"
#include "waves.h"

ENEMY** enemies = NULL;
unsigned int enemyCount = 0;

static int ghostSize = 75;

static SDL_Texture* spiritTexture;
static SDL_Texture* shadeTexture;
static SDL_Texture* goryoTexture;
static SDL_Texture* deogenTexture;

bool loadedTextures = false;

void loadGhostTextures(SDL_Renderer* renderer) {
    spiritTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Spirit.png");
    shadeTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Shade.png");
    goryoTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Goryo.png");
    deogenTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Deogen.png");
}

void spawnNewEnemy(ENEMYTYPE type) {
    enemies = realloc(enemies, (enemyCount + 1) * sizeof(ENEMY*));
    enemyCount++;

    ENEMY* buff = malloc(sizeof(ENEMY));
    buff->type = type;
    buff->rect = createFRect((float)(map.mapPointsWithDirections.points[0].x - ghostSize / 2), (float)(map.mapPointsWithDirections.points[0].y - ghostSize / 2), ghostSize, ghostSize);
    switch (type)
    {
    case SPIRIT:
        buff->speed = 120;
        buff->hp = 10;
        buff->texture = spiritTexture;
        break;
    case SHADE:
        buff->speed = 65;
        buff->hp = 13.5;
        buff->texture = shadeTexture;
        break;
    case GORYO:
        buff->speed = 120;
        buff->hp = 6;
        buff->texture = goryoTexture;
        break;
    case DEOGEN:
        buff->speed = 300;
        buff->hp = 10;
        buff->texture = deogenTexture;
        break;
    }
    buff->currPointIndex = 1;
    if (currentWave + 1 >= 5) buff->hp += 2.5;
    if (currentWave + 1 >= 7) buff->hp += 5;

    enemies[enemyCount - 1] = buff;
}

void reachedPlayerBase(int index) {
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i] == NULL) continue;
        if (i == index) {
            free(enemies[i]);
            enemies[i] = NULL;
            damagePlayer(7.5f);
        }
    }
}

void moveEnemiesTowardsCurrPoint() {
    if (enemyCount == 0) return;

    SDL_FRect result;
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i] == NULL) continue;
        SDL_FRect pointFRect = createFRect(map.mapPointsWithDirections.points[enemies[i]->currPointIndex].x,
                                        map.mapPointsWithDirections.points[enemies[i]->currPointIndex].y, 1, 1);
        SDL_FRect enemyHitboxFRect = createFRect(enemies[i]->rect.x + ghostSize / 2 - 5,
                                                enemies[i]->rect.y + ghostSize / 2 - 5, 10, 10);
        if (SDL_IntersectFRect(&enemyHitboxFRect, &pointFRect, &result) != SDL_TRUE) {
            switch (map.mapPointsWithDirections.directions[enemies[i]->currPointIndex])
            {
            case UP:
                enemies[i]->rect.y -= enemies[i]->speed * gameStatus.deltaTime;
                break;
            case DOWN:
                enemies[i]->rect.y += enemies[i]->speed * gameStatus.deltaTime;
                break;
            case RIGHT:
                enemies[i]->rect.x += enemies[i]->speed * gameStatus.deltaTime;
                break;
            case LEFT:
                enemies[i]->rect.x -= enemies[i]->speed * gameStatus.deltaTime;
                break;
            
            default:
                break;
            }
        } else {
            enemies[i]->currPointIndex++;
            if (map.mapPointsWithDirections.directions[enemies[i]->currPointIndex] == -1) reachedPlayerBase(i);
        }
    }
}

void renderEnemies(SDL_Renderer* renderer) {
    if (enemyCount == 0) return;
    
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i] == NULL) continue;
        SDL_Rect buff = createRect(enemies[i]->rect.x, enemies[i]->rect.y, enemies[i]->rect.w, enemies[i]->rect.h);
        SDL_RenderCopy(renderer, enemies[i]->texture, NULL, &buff);
    }
}

void checkForDeath() {
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i] == NULL) continue;

        if (enemies[i]->hp <= 0) {
            switch (enemies[i]->type)
            {
            case SHADE:
                player.score += 30;
                addMoney(3);
                break;
            case SPIRIT:
                player.score += 20;
                addMoney(2);
                break;
            case GORYO:
            case DEOGEN:
                player.score += 45;
                addMoney(4);
                break;
            }
            free(enemies[i]);
            enemies[i] = NULL;
        }
    }
}

void damageEnemy(ENEMY* enemy, float damage) {
    enemy->hp -= damage;
}

void freeEnemies() {
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i] == NULL) continue;
        free(enemies[i]);
        enemies[i] = NULL;
    }
    free(enemies);
    enemies = NULL;
    enemyCount = 0;

    SDL_DestroyTexture(spiritTexture);
    SDL_DestroyTexture(shadeTexture);
    SDL_DestroyTexture(deogenTexture);
    SDL_DestroyTexture(goryoTexture);
}