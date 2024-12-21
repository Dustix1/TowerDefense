#include "map.h"

struct Map map;
static struct MapPointsWithDirections buff;

void createMap(SelectedMap selectedMap, SDL_Renderer* renderer) {
    map.mapRect = createRect(0, 0, gameStatus.windowSizeX, gameStatus.windowSizeY - 250);
    switch (selectedMap)
    {
    case WILLOW:
        SDL_Point points[13] = {{605, 900}, {605, 500}, {218, 500}, {218, 309}, {1110, 309}, {1110, 70}, {1767, 70}, {1767, 210}, {1310, 210},
                                {1310, 335}, {1510, 335}, {1510, 750}, {0, 0}};
        DIRECTION directions[13] = {-1, UP, LEFT, UP, RIGHT, UP, RIGHT, DOWN, LEFT, DOWN, RIGHT, DOWN, -1};

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
}