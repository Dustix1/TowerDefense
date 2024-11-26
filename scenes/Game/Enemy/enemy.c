#include "enemy.h"

#include "../Map/map.h"
#include "../../../utils/gameStatus.h"

ENEMY* enemies;
unsigned int enemyCount = 0;

static SDL_Texture* spiritTexture;
static SDL_Texture* shadeTexture;
static SDL_Texture* goryoTexture;
static SDL_Texture* deogenTexture;
static SDL_Texture* missingTexture;

void spawnNewEnemy(ENEMYTYPE type, SDL_Renderer* renderer) {

    spiritTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Spirit.png");
    shadeTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Shade.png");
    goryoTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Goryo.png");
    deogenTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/Deogen.png");
    missingTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/enemies/missingTexture.jpg");

    if (enemyCount == 0) {
        enemies = malloc((enemyCount + 1) * sizeof(ENEMY));
    } else {
        enemies = realloc(enemies, (enemyCount + 1) * sizeof(ENEMY));
    }
    enemyCount++;

    ENEMY buff;
    buff.type = type;
    buff.rect = createRect(map.pointPath[0].x, map.pointPath[0].y, 125, 125);
    switch (type)
    {
    case SPIRIT:
        buff.texture = spiritTexture;
        break;
    case SHADE:
        buff.texture = shadeTexture;
        break;
    case GORYO:
        buff.texture = goryoTexture;
        break;
    case DEOGEN:
        buff.texture = deogenTexture;
        break;
    default:
        buff.texture = missingTexture;
        break;
    }
    buff.currPointIndex = 1;

    enemies[enemyCount - 1] = buff;
}

void moveEnemiesTowardsCurrPoint() {
    for (size_t i = 0; i < enemyCount; i++)
    {
        if (enemies[i].rect.x == map.pointPath[enemies[i].currPointIndex].x && enemies[i].rect.y == map.pointPath[enemies[i].currPointIndex].y) {
            enemies[i].currPointIndex++;
        }

        if (enemies[i].rect.x > map.pointPath[enemies[i].currPointIndex].x) enemies[i].rect.x -= 1 * gameStatus.deltaTime; 
        if (enemies[i].rect.x < map.pointPath[enemies[i].currPointIndex].x) enemies[i].rect.x += 1 * gameStatus.deltaTime; 
        if (enemies[i].rect.y > map.pointPath[enemies[i].currPointIndex].y) enemies[i].rect.y -= 1 * gameStatus.deltaTime; 
        if (enemies[i].rect.y < map.pointPath[enemies[i].currPointIndex].y) enemies[i].rect.y += 1 * gameStatus.deltaTime; 
    }
}

void renderEnemies(SDL_Renderer* renderer) {
    for (size_t i = 0; i < enemyCount; i++)
    {
        SDL_RenderCopy(renderer, enemies[i].texture, NULL, &enemies[i].rect);
    }
}