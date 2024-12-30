#include "gameStatus.h"

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL.h>

#include "../scenes/Game/gameScene.h"
#include "../scenes/End/endScene.h"
#include "../scenes/Menu/menuScene.h"

struct GameStatusStruct gameStatus;

static Uint64 last;
static Uint64 now;

void changeScene(CurrentSceneEnum scene) {
    gameStatus.currentScene = scene;
}

void initGame() {
    changeScene(MENU);
    gameStatus.running = true;
    last = SDL_GetPerformanceCounter();
}

void updateDeltaTime() {
    now = SDL_GetPerformanceCounter();
    
    // Výpočet delty, času od posledního provedení tohoto řádku (tj. délky iterace herní smyčky)
    gameStatus.deltaTime = (double)((now - last) / (double)SDL_GetPerformanceFrequency());

    // Uložení poslední hodnoty čítače
    last = now;
}

void freeCurrentScene() {
    switch (gameStatus.currentScene)
    {
    case GAME:
        freeGameScene();
        break;
    case MENU:
        freeMenuScene();
        break;
    case WIN:
    case LOSE:
        freeEndScene();
        break;
    case NONE:
        break;
    }
}