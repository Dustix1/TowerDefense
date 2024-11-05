#ifndef GAMESTATUS_H
#define GAMESTATUS_H

typedef enum {
    MENU,
    GAME
} currentSceneEnum;

struct GameStatusStruct{
    currentSceneEnum currentScene;
};

extern struct GameStatusStruct gameStatus;

// Change the scene possible options: MENU, GAME
void changeScene(currentSceneEnum scene);

// Initializes the game on start
void initGame();

#endif