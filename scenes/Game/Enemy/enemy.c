#include "enemy.h"

#include <stdbool.h>
#include <math.h>

#include "../Map/map.h"
#include "../../../utils/gameStatus.h"
#include "../Friendly/player.h"

ENEMY* enemies;
unsigned int enemyCount = 0;

static int ghostSize = 100;

static SDL_Texture* spiritTexture;
static SDL_Texture* shadeTexture;
static SDL_Texture* goryoTexture;
static SDL_Texture* deogenTexture;
static SDL_Texture* missingTexture;

bool loadedTextures = false;

void loadGhostTextures(SDL_Renderer* renderer) {
    spiritTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Spirit.png");
    shadeTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Shade.png");
    goryoTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Goryo.png");
    deogenTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Deogen.png");
    missingTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/missingTexture.jpg");
}

void spawnNewEnemy(ENEMYTYPE type) {
    if (enemyCount == 0) {
        enemies = malloc((enemyCount + 1) * sizeof(ENEMY));
    } else {
        enemies = realloc(enemies, (enemyCount + 1) * sizeof(ENEMY));
    }
    enemyCount++;

    ENEMY buff;
    buff.type = type;
    buff.rect = createFRect((float)(map.mapPointsWithDirections.points[0].x - ghostSize / 2), (float)(map.mapPointsWithDirections.points[0].y - ghostSize / 2), ghostSize, ghostSize);
    switch (type)
    {
    case SPIRIT:
        buff.speed = 125;
        buff.texture = spiritTexture;
        break;
    case SHADE:
        buff.speed = 110;
        buff.texture = shadeTexture;
        break;
    case GORYO:
        buff.speed = 125;
        buff.texture = goryoTexture;
        break;
    case DEOGEN:
        buff.speed = 300;
        buff.texture = deogenTexture;
        break;
    default:
        buff.speed = 350;
        buff.texture = missingTexture;
        break;
    }
    buff.currPointIndex = 1;

    enemies[enemyCount - 1] = buff;
}

void reachedPlayerBase(int index) {
    ENEMY* buff = malloc((enemyCount - 1) * sizeof(ENEMY));
    bool wasSkipped = false;
    for (size_t i = 0; i < enemyCount; i++)
    {
        if (i != index && !wasSkipped) {
            buff[i] = enemies[i];
        } else if (i != index) {
            buff[i - 1] = enemies[i];
        } else {
            wasSkipped = true;
        }
    }

    free(enemies);
    enemies = malloc((--enemyCount) * sizeof(ENEMY));
    enemies = buff;

    damagePlayer(7.5f);
}

void moveEnemiesTowardsCurrPoint(SDL_Renderer* renderer) {
    if (enemyCount == 0) return;

    SDL_FRect result;
    for (size_t i = 0; i < enemyCount; i++)
    {
        SDL_FRect pointFRect = createFRect(map.mapPointsWithDirections.points[enemies[i].currPointIndex].x,
                                           map.mapPointsWithDirections.points[enemies[i].currPointIndex].y, 1, 1);
        SDL_FRect enemyHitboxFRect = createFRect(enemies[i].rect.x + ghostSize / 2 - 5,
                                                 enemies[i].rect.y + ghostSize / 2 - 5, 10, 10);
        if (SDL_IntersectFRect(&enemyHitboxFRect, &pointFRect, &result) != SDL_TRUE) {
            switch (map.mapPointsWithDirections.directions[enemies[i].currPointIndex])
            {
            case UP:
                enemies[i].rect.y -= enemies[i].speed * gameStatus.deltaTime;
                break;
            case DOWN:
                enemies[i].rect.y += enemies[i].speed * gameStatus.deltaTime;
                break;
            case RIGHT:
                enemies[i].rect.x += enemies[i].speed * gameStatus.deltaTime;
                break;
            case LEFT:
                enemies[i].rect.x -= enemies[i].speed * gameStatus.deltaTime;
                break;
            
            default:
                break;
            }
        } else {
            enemies[i].currPointIndex++;
            if (map.mapPointsWithDirections.directions[enemies[i].currPointIndex] == -1) reachedPlayerBase(i);
        }
    }
}

void renderEnemies(SDL_Renderer* renderer) {
    if (enemyCount == 0) return;
    
    for (size_t i = 0; i < enemyCount; i++)
    {
        SDL_Rect buff = createRect(enemies[i].rect.x, enemies[i].rect.y, enemies[i].rect.w, enemies[i].rect.h);
        SDL_RenderCopy(renderer, enemies[i].texture, NULL, &buff);
    }
}