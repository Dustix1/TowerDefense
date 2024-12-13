#include "enemy.h"

#include <stdbool.h>
#include <math.h>

#include "../Map/map.h"
#include "../../../utils/gameStatus.h"

ENEMY* enemies;
unsigned int enemyCount = 0;

static int offsetX = 70;
static int offsetY = 70;

static SDL_Texture* spiritTexture;
static SDL_Texture* shadeTexture;
static SDL_Texture* goryoTexture;
static SDL_Texture* deogenTexture;
static SDL_Texture* missingTexture;

bool loadedTextures = false;

void spawnNewEnemy(ENEMYTYPE type, SDL_Renderer* renderer) {

    if (!loadedTextures) {
        spiritTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Spirit.png");
        shadeTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Shade.png");
        goryoTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Goryo.png");
        deogenTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Deogen.png");
        missingTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/missingTexture.jpg");

        loadedTextures = true;
    }

    if (enemyCount == 0) {
        enemies = malloc((enemyCount + 1) * sizeof(ENEMY));
    } else {
        enemies = realloc(enemies, (enemyCount + 1) * sizeof(ENEMY));
    }
    enemyCount++;

    int size = 125;
    ENEMY buff;
    buff.type = type;
    buff.rect = createFRect((float)(map.mapPointsWithDirections.points[0].x - offsetX), (float)(map.mapPointsWithDirections.points[0].y - offsetY), size, size);
    switch (type)
    {
    case SPIRIT:
        buff.speed = 100;
        buff.texture = spiritTexture;
        break;
    case SHADE:
        buff.speed = 100;
        buff.texture = shadeTexture;
        break;
    case GORYO:
        buff.speed = 100;
        buff.texture = goryoTexture;
        break;
    case DEOGEN:
        buff.speed = 200;
        buff.texture = deogenTexture;
        break;
    default:
        buff.speed = 150;
        buff.texture = missingTexture;
        break;
    }
    buff.currPointIndex = 1;

    enemies[enemyCount - 1] = buff;
}

void moveEnemiesTowardsCurrPoint(SDL_Renderer* renderer) {
    SDL_FRect result;
    for (size_t i = 0; i < enemyCount; i++)
    {
        SDL_FRect pointFRect = createFRect(map.mapPointsWithDirections.points[enemies[i].currPointIndex].x,
                                           map.mapPointsWithDirections.points[enemies[i].currPointIndex].y, 1, 1);
        SDL_FRect enemyHitboxFRect = createFRect(enemies[i].rect.x + offsetX - 2,
                                                 enemies[i].rect.y + offsetY - 2, 4, 4);
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
        }
    }
    
}

void renderEnemies(SDL_Renderer* renderer) {
    for (size_t i = 0; i < enemyCount; i++)
    {
        SDL_Rect buff = createRect(enemies[i].rect.x, enemies[i].rect.y, enemies[i].rect.w, enemies[i].rect.h);
        SDL_RenderCopy(renderer, enemies[i].texture, NULL, &buff);
    }
}