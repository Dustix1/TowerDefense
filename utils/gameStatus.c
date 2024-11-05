#include "gameStatus.h"

struct GameStatusStruct gameStatus;

void changeScene(currentSceneEnum scene) {
    gameStatus.currentScene = scene;
}

void initGame() {
    changeScene(MENU);
}