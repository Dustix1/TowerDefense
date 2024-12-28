#pragma once

#include <stdbool.h>

extern double deltaTime;

typedef enum {
    MENU,
    GAME,
    WIN,
    LOSE,
    NONE
} CurrentSceneEnum;

typedef enum {
    WILLOW,
} SelectedMap;

struct GameStatusStruct{
    CurrentSceneEnum currentScene;
    SelectedMap selectedMap;
    bool running;
    double deltaTime;
    int windowSizeX;
    int windowSizeY;
};

extern struct GameStatusStruct gameStatus;

// Change the scene possible options: MENU, GAME
void changeScene(CurrentSceneEnum scene);

// Initializes the game on start
void initGame();

void updateDeltaTime();

void freeCurrentScene();