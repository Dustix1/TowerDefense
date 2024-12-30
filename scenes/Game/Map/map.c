#include "map.h"

struct Map map;
static struct MapPointsWithDirections buff;

static SDL_Texture* pathTexture;

void loadPathTexture(SDL_Renderer* renderer) {
    pathTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/maps/path.jpg");
}

void generatePath() {
    int i = 0;
    while (map.mapPointsWithDirections.directions[i] != 999)
    {
        SDL_Point firstPoint = map.mapPointsWithDirections.points[i];
        SDL_Point secondPoint = map.mapPointsWithDirections.points[i + 1];

        map.pathRects = realloc(map.pathRects, (i + 1) * sizeof(SDL_Rect));
        int offset = 32;
        switch (map.mapPointsWithDirections.directions[i + 1])
        {
        case UP:
            map.pathRects[i] = createRect(secondPoint.x - offset, secondPoint.y - offset, offset * 2, firstPoint.y - secondPoint.y + offset * 2);
            break;
        case DOWN:
            map.pathRects[i] = createRect(firstPoint.x - offset, firstPoint.y - offset, offset * 2, secondPoint.y - firstPoint.y + offset * 2);
            break;
        case LEFT:
            map.pathRects[i] = createRect(secondPoint.x - offset, secondPoint.y - offset, firstPoint.x - secondPoint.x + offset * 2, offset * 2);
            break;
        case RIGHT:
            map.pathRects[i] = createRect(firstPoint.x - offset, firstPoint.y - offset, secondPoint.x - firstPoint.x + offset * 2, offset * 2);
            break;
        }
        i++;
    }
}

void createMap(SelectedMap selectedMap, SDL_Renderer* renderer) {
    map.mapRect = createRect(0, 0, gameStatus.windowSizeX, gameStatus.windowSizeY - 250);
    switch (selectedMap)
    {
    case WILLOW:
        map.vanRect = createRect(gameStatus.windowSizeX - 550, gameStatus.windowSizeY - 450, 350, 200);

        SDL_Point points[13] = {{605, 900}, {605, 500}, {218, 500}, {218, 309}, {1050, 309}, {1050, 70}, {1767, 70}, {1767, 300}, {1310, 300},
                                {1310, 500}, {1510, 500}, {1510, 750}, {0, 0}};
        DIRECTION directions[13] = {-2, UP, LEFT, UP, RIGHT, UP, RIGHT, DOWN, LEFT, DOWN, RIGHT, DOWN, 999};

        buff.points = malloc(13 * sizeof(SDL_Point));
        memcpy(buff.points, points, 13 * sizeof(SDL_Point));
        buff.directions = malloc(13 * sizeof(DIRECTION));
        memcpy(buff.directions, directions, 13 * sizeof(DIRECTION));

        map.mapPointsWithDirections = buff;

        map.mapTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/maps/WillowstreetTD.jpg");
        break;
    
    default:
        gameStatus.running = false;
        break;
    }

    generatePath(renderer);
}

void renderPath(SDL_Renderer* renderer) {
    int i = 0;
    while (map.mapPointsWithDirections.directions[i] != 999) {
        SDL_RenderCopy(renderer, pathTexture, NULL, &map.pathRects[i]);
        i++;
    }
}