#include "rectStuff.h"
#include <math.h>
#define M_PI 3.14159265358979323846

SDL_Rect createRect(int posX, int posY, int width, int height) {
    SDL_Rect buff = {
        .x = posX,
        .y = posY,
        .w = width,
        .h = height
    };
    return buff;
}

SDL_FRect createFRect(float posX, float posY, float width, float height) {
    SDL_FRect buff = {
        .x = posX,
        .y = posY,
        .w = width,
        .h = height
    };
    return buff;
}

SDL_Color createColor(char* hexColor, int alpha) {
    char r[3] = {0};
    strncpy(r, hexColor, 2);
    char g[3] = {0};
    strncpy(g, hexColor + 2, 2);
    char b[3] = {0};
    strncpy(b, hexColor + 4, 2);
    SDL_Color buff = {
        .r = strtoul(r, NULL, 16),
        .g = strtoul(g, NULL, 16),
        .b = strtoul(b, NULL, 16),
        .a = alpha
    };
    return buff;
}

double calculateRotationAngle(SDL_Rect r1, SDL_Rect r2) {
    return atan2((r2.y + r2.h / 2.0) - (r1.y + r1.h / 2.0), (r2.x + r2.w / 2.0) - (r1.x + r1.w / 2.0)) * (180.0 / M_PI) + 90;
}

double calculateRadiansFromAngle(double angle) {
    return angle * (M_PI / 180.0);
}