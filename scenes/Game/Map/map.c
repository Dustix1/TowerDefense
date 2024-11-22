#include "map.h"

struct Map map;

void createMap(SelectedMap selectedMap, SDL_Renderer* renderer) {
    map.mapRect = createRect(0, 0, gameStatus.windowSizeX, gameStatus.windowSizeY - 250);
    switch (selectedMap)
    {
    case WILLOW:
        SDL_Point points[12] = {{605, 850}, {605, 500}, {218, 500}, {218, 309}, {1106, 309}, {1106, 70}, {1759, 70}, {1759, 210}, {1310, 210},
                                {1310, 335}, {1500, 335}, {1500, 750}};
        map.mapTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/maps/WillowstreetTD.jpg");
        map.pointPath = malloc(12 * sizeof(SDL_Point));
        memcpy(map.pointPath, points, 12 * sizeof(SDL_Point));
        break;
    
    default:
        gameStatus.running = false;
        break;
    }
}