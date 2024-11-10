#pragma once

#include <stdbool.h>

extern double deltaTime;

typedef enum {
    MENU,
    GAME
} currentSceneEnum;

struct GameStatusStruct{
    currentSceneEnum currentScene;
    bool running;
    double deltaTime;
};

extern struct GameStatusStruct gameStatus;

// Change the scene possible options: MENU, GAME
void changeScene(currentSceneEnum scene);

// Initializes the game on start
void initGame();

void updateDeltaTime();